// GameView.h
// Official Interface for GameViewADT
// COMP1927 14s2

#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "Globals.h"
#include "Game.h"
#include "Places.h"
#include "Map.h"

// Used for navigating plays
#define PLAY_CHARS                      8
#define ABBREV_CHARS                    2
#define ENCOUNTER_START                 3
#define ENCOUNTER_END                   6

typedef struct gameView *GameView;

//Data functions

GameView newGameView(char *pastPlays, PlayerMessage messages[]);

void disposeGameView(GameView toBeDeleted);

Round getRound(GameView currentView);

PlayerID getCurrentPlayer(GameView currentView);

int getScore(GameView currentView);

int getHealth(GameView currentView, PlayerID player);

//Spacial functions

LocationID getRealPosition(GameView currentView, int round, PlayerID player, int shouldTrace);

LocationID getLocation(GameView currentView, PlayerID player);

void getHistory(GameView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]);

LocationID *connectedLocations(GameView currentView, int *numLocations, LocationID from, PlayerID player, Round round, int road, int rail, int sea);

int getPath(GameView g, LocationID src, LocationID dest, LocationID *path, int *weight, PlayerID player, int road, int rail, int sea);

int *getDistanceMap(GameView g, LocationID src, int *prev, int *weight, PlayerID player, int road, int rail, int sea);

//Helper functions

int getRoundEncounters(GameView currentView, PlayerID player, int round, char encounterType);

int searchHistory(GameView g, PlayerID player, int searchMin, int searchMax, int shouldTrace);

int willBeAvailable(GameView g, LocationID searchMin, LocationID searchMax, int roundOffset);

LocationID connectedMove(GameView g, LocationID pos);

char *roundMessage(GameView currentView, PlayerID player, Round round);

#endif

//Finds shortest path
/*
int getPath(GameView g, LocationID src, LocationID dest, LocationID *path, PlayerID player, int road, int rail, int sea) {
	assert(src != UNKNOWN_LOCATION);
	//Each city's optimal previous and cumulative distance from start
	int *prev = malloc(NUM_MAP_LOCATIONS * sizeof(int));
	int *dist = malloc(NUM_MAP_LOCATIONS * sizeof(int));
	for(int i=0; i<NUM_MAP_LOCATIONS; i++) {
		prev[i] = NOWHERE;
		dist[i] = NUM_MAP_LOCATIONS-1;
	}
	dist[src] = 0;
	int round = getRound(g);
	//Add connected cities to queue and iterate until all potential routes exhasted
	Queue q = newQueue();
	enqueue(q, src);
	while(!isEmpty(q)) {
		int numEdges = 0;
		LocationID curr = dequeue(q);
		LocationID *currEdges = connectedLocations(g, &numEdges, curr, player, round+dist[curr], road, rail, sea);
		for(int i=0; i<numEdges; i++) {
			//will curr edges be out of dist range [0-70]
			if(currEdges[i] != UNKNOWN_LOCATION && (dist[curr]+1) < dist[currEdges[i]]) {
				dist[currEdges[i]] = dist[curr]+1;
				prev[currEdges[i]] = curr;
				enqueue(q, currEdges[i]);
			}
		}
	}
	//No path was found
	if(dist[dest] == NUM_MAP_LOCATIONS-1) return -1;
	//Trace previous array to find route and save its reverse to path
	if(path != NULL) for(int i = dest, j = dist[dest]-1; prev[i] != NOWHERE; i = prev[i], j--) path[j] = i;
	return dist[dest];
}*/