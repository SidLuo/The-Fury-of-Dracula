// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "Game.h"
#include "DracView.h"
#include "Set.h"

#define NUM_SNOOKERS 8-1
#define MAX_HEALTH_LOSS 4*LIFE_LOSS_HUNTER_ENCOUNTER+LIFE_LOSS_SEA
#define SNOOKER_HEALTH_LOSS 3*LIFE_LOSS_SEA
#define SNOOKER_LENGTH 5

static int findSnooker(DracView g, LocationID *nextMove);
typedef struct _snooker {
    LocationID endpoint;
    Set connectedAdjacent;
    Set disconnectedAdjacent;
} Snooker;

void decideDraculaMove(DracView g) {
    char *message = "Starting";
    int currentRound = giveMeTheRound(g);
    LocationID nextMove = NOWHERE;
    if(currentRound == 0) {
        nextMove = EDINBURGH;
    } else {
        LocationID myLocation = whereWas(g, currentRound-1, PLAYER_DRACULA, TRUE);
        
        //Generate minima distance map and health map
        int dist[NUM_MAP_LOCATIONS];
        int health[NUM_MAP_LOCATIONS];
        for(int i=0; i<NUM_MAP_LOCATIONS; i++) {
            dist[i] = NUM_MAP_LOCATIONS;
            health[i] = (idToType(i)==SEA)*LIFE_LOSS_SEA;
        }
        for(int i=0; i<NUM_PLAYERS-1; i++) {
            int *hunterDist = giveMeDistanceMap(g, whereIs(g, i), NULL, NULL, i, TRUE, TRUE, TRUE);
            for(int j=0; j<NUM_MAP_LOCATIONS; j++) {
                if(hunterDist[j]==1) health[i] += LIFE_LOSS_HUNTER_ENCOUNTER;
                if(idToType(j) == SEA) {
                    dist[j] = 2;
                    health[j] = LIFE_LOSS_SEA;
                } else if(hunterDist[j] < dist[j]) dist[j] = hunterDist[j];
            }
            free(hunterDist);
        }

        //Find possible moves, path to castle and snooker
        int numMoves = 0;
        LocationID *possibleMoves = whereCanIgo(g, &numMoves, TRUE, TRUE);
        LocationID *possiblePath = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
        int healthToCastle = findPath(g, myLocation, CASTLE_DRACULA, possiblePath, health, PLAYER_DRACULA, TRUE, FALSE, TRUE);
        int distToSnooker = findSnooker(g, &nextMove);
        
        //Find nearest SEA
        int bestSea = ATLANTIC_OCEAN;
        int *myDist = giveMeDistanceMap(g, myLocation, NULL, NULL, PLAYER_DRACULA, TRUE, FALSE, TRUE);
        int bestSeaDist = myDist[bestSea];
        for(int i=0; i<numMoves; i++) if(idToType(possibleMoves[i])==SEA && myDist[possibleMoves[i]] < bestSeaDist) {
            bestSeaDist = myDist[possibleMoves[i]];
            bestSea = possibleMoves[i];
        }
        LocationID *possibleSeaPath = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
        
        printf(" [%d from me]  [%d from castle] ", dist[myLocation], dist[CASTLE_DRACULA]);

        //Sometimes snooker moves arnt legal (already in trail) - temporary fix
        int isLegalMove = FALSE;
        for(int i=0; i<numMoves; i++) if(possibleMoves[i] == nextMove) isLegalMove = TRUE;

        //Decide move
        if(!numMoves) {
            nextMove = TELEPORT;
            message = "Teleporting home";
        } else if (dist[myLocation]==1 && howHealthyIs(g, PLAYER_DRACULA) > LIFE_LOSS_SEA && findPath(g, myLocation, bestSea, possibleSeaPath, NULL, PLAYER_DRACULA, TRUE, FALSE, TRUE) > 0) {
            message = "Sneaking to sea";
            nextMove = possibleSeaPath[0];
        } else if (isLegalMove && distToSnooker >= 0 && dist[CASTLE_DRACULA] > 3 && dist[nextMove] > dist[myLocation]) {
            message = "Sneaking home";
        } else {
            message = "Evading";
            nextMove = possibleMoves[0];
            int bestDist = 1;
            for(int i=0; i<numMoves; i++) {
                if(dist[possibleMoves[i]] == 0 || dist[possibleMoves[i]] > bestDist) {
                    bestDist = dist[possibleMoves[i]];
                    nextMove = possibleMoves[i];
                }
            }
        }
        free(possiblePath);
        free(possibleMoves);
    }
    registerBestPlay(idToAbbrev(connectionToMove(g, nextMove)), message);
}

//Returns next move to bring about snooker
static int findSnooker(DracView g, LocationID *nextMove) {
    int numMoves = 0;
    Snooker potentialSnookers[] = {
        {BLACK_SEA, aToSet((int []){CONSTANTA, VARNA}, 2), aToSet((int []){IONIAN_SEA}, 1)},
        {CAGLIARI, aToSet((int []){MEDITERRANEAN_SEA, TYRRHENIAN_SEA}, 2), newSet()},
        {PLYMOUTH, aToSet((int []){ENGLISH_CHANNEL, LONDON}, 2), newSet()},
        {GALWAY, newSet(), aToSet((int []){ATLANTIC_OCEAN, DUBLIN}, 2)},
        {DUBLIN, newSet(), aToSet((int []){GALWAY, IRISH_SEA}, 2)},
        //{CASTLE_DRACULA, aToSet((int []){KLAUSENBURG, GALATZ}, 2), newSet()},
        {ATHENS, aToSet((int []){VALONA, IONIAN_SEA}, 2), newSet()},
        {EDINBURGH, newSet(), aToSet((int []){NORTH_SEA, MANCHESTER}, 2)}
    };

    LocationID *snookerPath = calloc(TRAIL_SIZE, sizeof(LocationID));
    int pathIndex=1;
    int trailEnd = (giveMeTheRound(g)-1);
    int trailStart = (trailEnd-(TRAIL_SIZE) > 0) ? trailEnd-(TRAIL_SIZE): 0;

    int hasFound = FALSE;
    int moveOfTeleport = 0;
    LocationID endpoint = 0;

    Set attempted = newSet();
    for(int i=trailStart; i<=trailEnd; i++, pathIndex++) {
        int pos = whereWas(g, i, PLAYER_DRACULA, FALSE);
        if(!hasFound && pos <= ZURICH) {
            for(int j = 0; j<NUM_SNOOKERS; j++) {
                if(!hasFound && moveAvailable(g, DOUBLE_BACK_1, DOUBLE_BACK_5, i+SNOOKER_LENGTH-1, FALSE) && (SetMember(potentialSnookers[j].connectedAdjacent, pos) || SetMember(potentialSnookers[j].disconnectedAdjacent, pos))) {
                    hasFound = TRUE;
                    SetInsert(attempted, j);
                    snookerPath[0] = pos;
                    pathIndex = 1;

                    endpoint = potentialSnookers[j].endpoint;
                    Set connected = SetCopy(potentialSnookers[j].connectedAdjacent);
                    Set disconnected = SetCopy(potentialSnookers[j].disconnectedAdjacent);
                    SetDelete(connected, pos); SetDelete(disconnected, pos);

                    for(int k=1; hasFound && k<TRAIL_SIZE; k++) {
                        //Add position to path
                        if(SetCard(connected) && (SetMember(potentialSnookers[j].connectedAdjacent, snookerPath[k-1]) || snookerPath[k-1] == potentialSnookers[j].endpoint)) {
                            snookerPath[k] = randomMember(connected);
                            SetDelete(connected, snookerPath[k]);
                        } else if(SetMember(potentialSnookers[j].disconnectedAdjacent, snookerPath[k-1]) || (!SetCard(connected) && SetMember(potentialSnookers[j].connectedAdjacent, snookerPath[k-1])) || (!SetCard(connected) && !SetCard(disconnected))) {
                            snookerPath[k] = potentialSnookers[j].endpoint;
                        } else {
                            snookerPath[k] = randomMember(disconnected);
                            SetDelete(disconnected, snookerPath[k]);
                        }

                        //If projected path is already within trail, make sure we followed plan
                        if(i+k <= trailEnd) {
                            if (whereWas(g, i+k, PLAYER_DRACULA, TRUE) != snookerPath[k]) {
                                hasFound = FALSE;
                               printf("[%s != %s]", idToAbbrev(whereWas(g, i+k, PLAYER_DRACULA, TRUE)), idToAbbrev(snookerPath[k]));
                            } else {
                                 printf("[%s == %s]", idToAbbrev(whereWas(g, i+k, PLAYER_DRACULA, TRUE)), idToAbbrev(snookerPath[k]));
                            }
                        }
                    }

                    dropSet(connected);
                    dropSet(disconnected);
                }
            }
        }
    }

    //[Following snooker plan MS TS CG CG CG ] but goes to CG (skips TS)
    if(hasFound) {
        printf(" [Following snooker plan ");
        //for(int i=0; i<SNOOKER_LENGTH; i++) printf("%s ", idToAbbrev(snookerPath[i])); printf("] ");
        *nextMove = snookerPath[pathIndex-1];
        numMoves = SNOOKER_LENGTH - pathIndex;
    } else {
        printf(" [Moving to new plan] ");
        *nextMove = NOWHERE;
        int numShortest = 0;
        LocationID *path = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
        int myLocation = whereWas(g, giveMeTheRound(g)-1, PLAYER_DRACULA, TRUE);
        for(int i=0, num = 0, currHealth; i<NUM_SNOOKERS; i++) {
            if(!SetMember(attempted, potentialSnookers[i].endpoint)) {
                num = findPath(g, myLocation, potentialSnookers[i].endpoint, path, NULL, PLAYER_DRACULA, TRUE, FALSE, TRUE);
                if((!numShortest) || (num != NOWHERE && num < numShortest)) {
                    *nextMove = path[0];
                    numShortest = num;
                }
            }
        }
        numMoves = numShortest + SNOOKER_LENGTH;
        free(path);
    }

    dropSet(attempted);
    free(snookerPath);
    for(int i=0; i<NUM_SNOOKERS; i++) {
        dropSet(potentialSnookers[i].connectedAdjacent);
        dropSet(potentialSnookers[i].disconnectedAdjacent);
    }
    return numMoves;
}
