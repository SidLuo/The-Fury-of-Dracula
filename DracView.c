// DracView.c ... DracView ADT implementation
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Places.h"

#define PLAYER_ABBREV {'G', 'S','H', 'M', 'D'}

static void editPastPlays (char * pastPlays, DracView g, LocationID location);

struct dracView {
    GameView gameView;
    char *pastPlays;
};

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[]) {
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->gameView = newGameView(pastPlays, messages);
    dracView->pastPlays = strdup(pastPlays);
    return dracView;
}
       
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted) {
    disposeGameView(toBeDeleted->gameView);
    free(toBeDeleted->pastPlays);
    free(toBeDeleted);
}

// Get the current round
Round giveMeTheRound(DracView g) {
     return getRound(g->gameView);
}

// Get the current score
int giveMeTheScore(DracView g) {
    return getScore(g->gameView);
}

// Get the current health points for a given player
int howHealthyIs(DracView g, PlayerID player) {
    return getHealth(g->gameView, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView g, PlayerID player) {
    return getRealPosition(g->gameView, giveMeTheRound(g) - (getCurrentPlayer(g->gameView) <= player), player, FALSE);
}

// Get the most recent move of a given player
void lastMove(DracView g, PlayerID player, LocationID *start, LocationID *end) {
    int roundOfLastMove = giveMeTheRound(g) - (getCurrentPlayer(g->gameView) <= player);
    *end = whereWas(g, roundOfLastMove, player, TRUE);
    if(roundOfLastMove == 0) *start = NOWHERE;
    else *start = whereWas(g, roundOfLastMove-1, player, TRUE);
}

//Starting at previous round, search Dracula's trail (Assumes it's Dracula's turn)
void whatsThere(DracView g, LocationID where, int *numTraps, int *numVamps) { 
    *numTraps = 0; *numVamps = 0;
    for(int i = 0; i<TRAIL_SIZE; i++) {
        if(whereWas(g, (giveMeTheRound(g)-1-i), PLAYER_DRACULA, TRUE) == where) {
            //Determine if this location is in any hunter's trails
            int encounterFound = FALSE;
            for(int j=0; j<TRAIL_SIZE; j++) {
                for(int k=0; k<NUM_PLAYERS-1; k++) {
                    if(whereWas(g, giveMeTheRound(g)-j, k, TRUE) == where) encounterFound = TRUE;
                }
            }

            //Incriment encounters
            if(!encounterFound) {
                int playIndex = PLAY_CHARS * NUM_PLAYERS * (giveMeTheRound(g)-1-i) + PLAYER_DRACULA * PLAY_CHARS;
                for(int l = ENCOUNTER_START; l <= ENCOUNTER_END; l++) {
                    (*numTraps) += (g->pastPlays[playIndex+l] == 'T');
                    (*numVamps) += (g->pastPlays[playIndex+l] == 'V');
                }
            }
        }
    }
}

// Fills the trail array with the location ids of the last 6 turns
int giveMeTheTrail(DracView g, PlayerID player, LocationID trail[TRAIL_SIZE]) {
    getHistory(g->gameView, player, trail);
    return (giveMeTheRound(g)-1) < TRAIL_SIZE ? (giveMeTheRound(g)-1) : TRAIL_SIZE;

}

// Get past location ids of a given player
LocationID whereWas(DracView g, int round, PlayerID player, int shouldTrace) {
    return getRealPosition(g->gameView, round, player, shouldTrace);
}

// What are my (Dracula's) possible next moves (locations), now includes HI/DN - requires whereWas
LocationID *whereCanIgo(DracView g, int *numLocations, int road, int sea) {
    return connectedLocations(g->gameView, numLocations, whereWas(g, giveMeTheRound(g)-1, PLAYER_DRACULA, TRUE), PLAYER_DRACULA, giveMeTheRound(g), road, FALSE, sea);
}

LocationID *whereCanTheyGo(DracView g, int *numLocations, PlayerID player, int road, int rail, int sea) {
    return connectedLocations(g->gameView, numLocations, whereIs(g, player), player, giveMeTheRound(g), road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCouldIGo(DracView g, int *numLocations, int from, PlayerID player, int road, int rail, int sea) {
    return connectedLocations(g->gameView, numLocations, from, player, giveMeTheRound(g), road, rail, sea);
}

int *giveMeDistanceMap(DracView g, LocationID src, int *prev, int *weight, PlayerID player, int road, int rail, int sea) {
    return getDistanceMap(g->gameView, src, prev, weight, player, road, rail, sea);
}

//Converts an ID to a move 
LocationID connectionToMove(DracView g, LocationID pos) {
    return connectedMove(g->gameView, pos);
}

int findPath(DracView g, LocationID src, LocationID dest, LocationID *path, int *weight, PlayerID player, int road, int rail, int sea) {
    return getPath(g->gameView, src, dest, path, weight, player, road, rail, sea);
}
//Returns true if a given move will be available in x rounds
int moveAvailable(DracView g, LocationID searchMin, LocationID searchMax, int roundOffset, int includeSpecials) {
    return willBeAvailable(g->gameView, searchMin, searchMax, roundOffset) + includeSpecials*(willBeAvailable(g->gameView, HIDE, HIDE, roundOffset) || willBeAvailable(g->gameView, DOUBLE_BACK_1, DOUBLE_BACK_5, roundOffset));
}

int monteCarlo (DracView g, int roundsToDo) {
    if (roundsToDo == 0) return howHealthyIs(g, PLAYER_DRACULA);
    srand(time(NULL));
    int size, num, *locations;
    
    int strPos = strlen(g -> pastPlays);
    PlayerMessage messages[] = {"Hello","Rubbish","Stuff",""};
    char pastPlays[strPos + PLAY_CHARS * NUM_PLAYERS + 1];
    strcpy (pastPlays,g -> pastPlays);
    LocationID * possibleMoves = whereCanIgo (g, &size, TRUE, TRUE);
    int *blood = calloc (size, sizeof (int));
    int *score = calloc (size, sizeof (int));
    
    for (int i = 0; i < size; i++) {
        editPastPlays(pastPlays, g, possibleMoves[i]);
        int total = 0;
        for (int j = 0; j < 3; j++) {
            DracView dv = newDracView(pastPlays, messages);
            for (int player = 0; player < PLAYER_DRACULA; player++) {
            
                locations = whereCanTheyGo (dv, &num, player, TRUE, TRUE, TRUE);
            
                editPastPlays(pastPlays, dv, locations[rand()%num]);
                free(locations);
                disposeDracView(dv);
                dv = newDracView(pastPlays, messages);
            }
            printf("I simulated %s\n\n", pastPlays);
            total += j;
            blood[i] += monteCarlo (dv, roundsToDo - 1);
            disposeDracView(dv);
        }
        blood[i] = blood[i]/total;
        score[i] = score[i]/total;
    }
    free(possibleMoves);
    free(blood);
    free(score);
    return 0;
}

static void editPastPlays (char * pastPlays, DracView g, LocationID location) {
    int numTraps, numVamps;
    int strPos = strlen(g -> pastPlays);
    int player = getCurrentPlayer(g -> gameView);
    char playerAbbrev[] = PLAYER_ABBREV;
    
    pastPlays[strPos] = ' ';
    pastPlays[strPos + 1] = playerAbbrev[player];
    pastPlays[strPos + 2] = '\0';
    
    Round round = getRound(g -> gameView);
    
    if (player == PLAYER_DRACULA) {
        strcat(pastPlays, idToAbbrev(connectionToMove(g, location)));
        if (idToType(location) != SEA) {
            if (round % 13 == 0) {pastPlays[strPos + 4] = '.'; pastPlays[strPos + 5] = 'V';}
            else {pastPlays[strPos + 4] = 'T'; pastPlays[strPos + 5] = '.';}
        } else {pastPlays[strPos + 4] = '.'; pastPlays[strPos + 5] = '.';}
        
        whatsThere(g, whereWas(g, round-6, PLAYER_DRACULA, FALSE), &numTraps, &numVamps);
        if (round % 13 == 6 && numVamps) pastPlays[strPos + 6] = 'V';
        else pastPlays[strPos + 6] = '.';
        pastPlays[strPos + 7] = '.';
        
    } else {
        strcat(pastPlays, idToAbbrev(location));
        whatsThere(g, location, &numTraps, &numVamps);
        for (int k = 0; k < numTraps; k++)
            pastPlays[strPos + 4 + k] = 'T';
        if (numVamps) pastPlays[strPos + 4 + numTraps] = 'V';
        
        int record = numTraps + numVamps;
        
        if (whereWas(g, round - 1, PLAYER_DRACULA, FALSE) == location) {
            pastPlays[strPos + 4 + record] = 'D';
            record++;
        }
        for (int k = record ; k < 4; k++) {
            pastPlays[strPos + 4 + record + k] = '.';
        }
    }
    pastPlays[strPos + PLAY_CHARS] = '\0';
}