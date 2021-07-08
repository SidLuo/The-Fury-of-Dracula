// DracView.h
// Official Interface for DracViewADT
// COMP1927 14s2

#ifndef DRAC_VIEW_H
#define DRAC_VIEW_H

#include "Globals.h"
#include "Game.h"
#include "Places.h"
#include "GameView.h"

// Used for navigating plays
#define PLAY_CHARS                      8
#define ABBREV_CHARS                    2
#define ENCOUNTER_START                 3
#define ENCOUNTER_END                   6

typedef struct dracView *DracView;

DracView newDracView(char *pastPlays, PlayerMessage messages[]);

void disposeDracView(DracView toBeDeleted);

Round giveMeTheRound(DracView currentView);

int giveMeTheScore(DracView currentView);

int howHealthyIs(DracView currentView, PlayerID player);

LocationID whereIs(DracView currentView, PlayerID player);

void lastMove(DracView currentView, PlayerID player, LocationID *start, LocationID *end);

void whatsThere(DracView currentView, LocationID where, int *numTraps, int *numVamps);

int giveMeTheTrail(DracView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]);

LocationID whereWas(DracView currentView, int round, PlayerID player, int shouldTrace);

LocationID *whereCanIgo(DracView currentView, int *numMoves, int road, int sea);

LocationID *whereCouldIGo(DracView currentView, int *numLocations, int from, PlayerID player, int road, int rail, int sea);

LocationID *whereCanTheyGo(DracView currentView, int *numLocations, PlayerID player, int road, int rail, int sea);

LocationID connectionToMove(DracView currentView, LocationID pos);

int findPath(DracView g, LocationID src, LocationID dest, LocationID *path, int *weight, PlayerID player, int road, int rail, int sea);

int *giveMeDistanceMap(DracView g, LocationID src, int *prev, int *weight, PlayerID player, int road, int rail, int sea);

int moveAvailable(DracView g, LocationID searchMin, LocationID searchMax, int roundOffset, int includeSpecials);

int monteCarlo (DracView g, int roundsToDo);

#endif
