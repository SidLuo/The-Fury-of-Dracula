// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Queue.h"
     
struct hunterView {
    GameView gameView;
};
     

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[]) {
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->gameView = newGameView(pastPlays, messages);
    return hunterView;
}
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted) {
    disposeGameView(toBeDeleted->gameView);
    free(toBeDeleted);
}

// Get the current round
Round giveMeTheRound(HunterView currentView) {
    return getRound(currentView->gameView);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView) {
    return getCurrentPlayer(currentView->gameView);
}

// Get the current score
int giveMeTheScore(HunterView currentView) {
    return getScore(currentView->gameView);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player) {
    return getHealth(currentView->gameView, player);
}

LocationID whereIs(HunterView currentView, PlayerID player) {
    return getRealPosition(currentView->gameView, giveMeTheRound(currentView) - (whoAmI(currentView) <= player), player, FALSE);
}

// Get past location ids of a given player
LocationID whereWas(HunterView currentView, int round, PlayerID player, int shouldTrace) {
    return getRealPosition(currentView->gameView, round, player, shouldTrace);
}

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]) {
    getHistory(currentView->gameView, player, trail);
}

// What are the specified player's possible moves from a given location
LocationID *whereCouldTheyGoFrom(HunterView currentView, int *numLocations, LocationID from, PlayerID player, Round round, int road, int rail, int sea) {
    return connectedLocations(currentView->gameView, numLocations, from, player, round, road, rail, sea);
}

LocationID *whereCanTheyGo(HunterView currentView, int *numLocations, PlayerID player, int road, int rail, int sea) {
    return connectedLocations(currentView->gameView, numLocations, whereIs(currentView, player), player, giveMeTheRound(currentView), road, rail, sea);
}

// finds a shortest path for hunters and returns distance
int findPath(HunterView view, LocationID src, LocationID dest, LocationID *path, PlayerID player, int road, int rail, int sea) {
    return getPath(view->gameView, src, dest, path, NULL, player, road, rail, sea);
}

//Gets number of encounters for player in round matching char type
int didFindEncounter(HunterView currentView, PlayerID player, int round, char enounterType) {
    return getRoundEncounters(currentView->gameView, player, round, enounterType);
}

int findInTrail(HunterView currentView, int search) {
    return searchHistory(currentView->gameView, PLAYER_DRACULA, search, search, FALSE);
}

char *getMessage (HunterView currentView, PlayerID player, Round round) {
    return roundMessage(currentView->gameView, player, round);
}