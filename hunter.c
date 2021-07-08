// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Game.h"
#include "HunterView.h"
#include "Set.h"
#include "Stack.h"

#define HOME {CADIZ, BLACK_SEA, LIVERPOOL, ATHENS}
#define MAX_DEPTH 8

void decideHunterMove(HunterView gameState) {
    int me = whoAmI(gameState);
    srand(time(NULL));
    LocationID myLocation = whereIs(gameState, me);
    LocationID huntTarget = NOWHERE;
    int nextMove, num;
    int home[] = HOME;
    //Find most recent Dracula location within trail
    for(int i=giveMeTheRound(gameState)-1, j=0; i>=0 && j<=TRAIL_SIZE && (huntTarget > ZURICH || huntTarget == NOWHERE); i--, j++) {
        huntTarget = whereWas(gameState, i, PLAYER_DRACULA, TRUE);
    }

    //Decide move
    char * moveDescription = idToName(home[(me + 1)%4]);
    if(giveMeTheRound(gameState) == 0) {
        int startingPositions[] = HOME;
        nextMove = startingPositions[me];
    } else if (howHealthyIs(gameState, me) <= LIFE_LOSS_TRAP_ENCOUNTER) {
        nextMove = whereIs(gameState, me);
	    moveDescription = "Recovering";
    } else if (huntTarget >= 0 && huntTarget <= ZURICH) {
        moveDescription = "Hunting";
        LocationID path[NUM_MAP_LOCATIONS];
        num = findPath(gameState, myLocation, huntTarget, path, me, TRUE, TRUE, TRUE);
        if(num > 0) nextMove = path[0];
        else nextMove = whereIs(gameState, me);
    } else {
        //moveDescription = "Searching";
        /*
        char * name = getMessage(gameState, me, giveMeTheRound(gameState) - 1);
        if (nameToID(name) == NOWHERE)
            huntTarget = home[(me + 1)%4];
        else if (nameToID(name) != whereIs(gameState, me))
            huntTarget = nameToID(name);
        else {
            srand(time(NULL));
            while (huntTarget == whereIs(gameState, me))
                huntTarget = home[rand()%4];
        }*/
        
        huntTarget = rand()%(NUM_MAP_LOCATIONS/4) + (NUM_MAP_LOCATIONS/4) * me;
        LocationID path[NUM_MAP_LOCATIONS];
        num = findPath(gameState, myLocation, huntTarget, path, me, TRUE, TRUE, TRUE);
        if(num > 0) nextMove = path[0];
        else nextMove = huntTarget;
        moveDescription = idToName(huntTarget);
    }
    registerBestPlay(idToAbbrev(nextMove), moveDescription);
}
