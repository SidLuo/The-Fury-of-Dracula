// HunterView.h
// Official Interface for HunterViewADT
// COMP1927 14s2

#ifndef HUNTER_VIEW_H
#define HUNTER_VIEW_H

#include "Globals.h"
#include "Game.h"
#include "Places.h"

typedef struct hunterView *HunterView;

HunterView newHunterView(char *pastPlays, PlayerMessage messages[]);

void disposeHunterView(HunterView toBeDeleted);

Round giveMeTheRound(HunterView currentView);

PlayerID whoAmI(HunterView currentView);

int giveMeTheScore(HunterView currentView);

int howHealthyIs(HunterView currentView, PlayerID player);

LocationID whereIs(HunterView currentView, PlayerID player);

void giveMeTheTrail(HunterView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]);

LocationID *whereCouldTheyGoFrom(HunterView currentView, int *numLocations, LocationID from, PlayerID player, Round round, int road, int rail, int sea);

LocationID *whereCanTheyGo(HunterView currentView, int *numLocations, PlayerID player, int road, int rail, int sea);

int findPath(HunterView view, LocationID src, LocationID dest, LocationID *path, PlayerID player, int road, int rail, int sea);

int didFindEncounter(HunterView currentView, PlayerID player, int round, char enounterType);

LocationID whereWas(HunterView currentView, int round, PlayerID player, int shouldTrace);

int findInTrail(HunterView currentView, int search);

char *getMessage (HunterView currentView, PlayerID player, Round round);

#endif