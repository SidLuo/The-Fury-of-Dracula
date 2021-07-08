// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Queue.h"
#include "Set.h"

#define OUT_OF_RANGE INT_MAX

static void calculateHealth(GameView g);  

struct gameView {
	char *pastPlays;
	PlayerMessage *messages;
	int plays;
	int *playerHealth;
	int hospitallisations;
	int *lastHospitallisation;
	Map map;
};

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]) {
	assert (pastPlays != NULL);
    GameView g = malloc(sizeof(struct gameView));
	assert (g != NULL);
	g->pastPlays = pastPlays;
	g->messages = messages;
	g->map = newMap();
	g->plays = ((strlen(g->pastPlays)+1)/PLAY_CHARS);
	g->playerHealth = malloc(sizeof(int) * NUM_PLAYERS);
	g->lastHospitallisation = calloc(NUM_PLAYERS-1, sizeof(int));
	calculateHealth(g);
	return g;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView g) {
    disposeMap(g->map);
	free(g->playerHealth);
	free(g->lastHospitallisation);
    free(g);
}

// Get the current round
Round getRound(GameView g) {
    return g->plays/NUM_PLAYERS;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView g) {
    return g->plays%NUM_PLAYERS;
}

// Get the current score
int getScore(GameView g) {
    int score = GAME_START_SCORE - getRound(g)*SCORE_LOSS_DRACULA_TURN - g->hospitallisations*SCORE_LOSS_HUNTER_HOSPITAL;
	for(int round = 0; round<getRound(g); round++) {
		int playIndex = PLAY_CHARS*NUM_PLAYERS*round + PLAYER_DRACULA*PLAY_CHARS;
		if (g->pastPlays[playIndex+5] == 'V') {
			score -= SCORE_LOSS_VAMPIRE_MATURES;
		}
	}
    return score;
}

//Fills array of player's healths and determines number of hospitallisations
static void calculateHealth(GameView g) {
	g->hospitallisations = 0;
	int draculaInjury = 0;
	for(int player = 0; player<NUM_PLAYERS; player++) {
		int hunterInjury = 0;
		int roundOfLastMove = getRound(g) - (getCurrentPlayer(g) <= player);

		for(int round = 0; round<=roundOfLastMove; round++) {
			int playIndex = PLAY_CHARS*NUM_PLAYERS*round + player*PLAY_CHARS;
			int pos = getRealPosition(g, round, player, TRUE);

			//Encounter modifiers
			for(int k = ENCOUNTER_START; k <= ENCOUNTER_END; k++) {
					draculaInjury += (g->pastPlays[playIndex+k] == 'D')*LIFE_LOSS_HUNTER_ENCOUNTER;
					hunterInjury += (g->pastPlays[playIndex+k] == 'D')*LIFE_LOSS_DRACULA_ENCOUNTER ;
					hunterInjury += (g->pastPlays[playIndex+k] == 'T')*LIFE_LOSS_TRAP_ENCOUNTER;	
			}
			if(player == PLAYER_DRACULA) {
				//Dracula rest and sea penalty
				draculaInjury += ((idToType(pos)==SEA)||pos==SEA_UNKNOWN)*LIFE_LOSS_SEA - (pos==CASTLE_DRACULA)*LIFE_GAIN_CASTLE_DRACULA;
			} else {				
				//Reduce injury if we just recovered, otherwise hospitallise if injury severe enough
				if((g->lastHospitallisation[player] && g->lastHospitallisation[player]==round-1)) {
					hunterInjury = 0;
				} else if(hunterInjury >= GAME_START_HUNTER_LIFE_POINTS) {
					g->hospitallisations++;
					g->lastHospitallisation[player] = round;
				}

				////Apply rest if we remained in position and have low health
				if((round>0 && pos==(getRealPosition(g, round-1, player, TRUE)))) hunterInjury -= LIFE_GAIN_REST;
				if(hunterInjury <0 ) hunterInjury = 0;
			}
		}
		g->playerHealth[player] = GAME_START_HUNTER_LIFE_POINTS - hunterInjury;
	}
	g->playerHealth[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS - draculaInjury;
}

// Get the current health points for a given player
int getHealth(GameView g, PlayerID player) {
	return g->playerHealth[player];
}

//Get position of player during given round and decipher HIDES and DOUBLE_BACKS if requested
LocationID getRealPosition(GameView g, int round, PlayerID player, int shouldTrace) {    
	if(player != PLAYER_DRACULA && (g->lastHospitallisation[player] && g->lastHospitallisation[player]==round)) return ST_JOSEPH_AND_ST_MARYS;
    int i = PLAY_CHARS*NUM_PLAYERS*round + player*PLAY_CHARS;
    if(i >= g->plays*PLAY_CHARS) return NOWHERE;

    //Find actual position using offset if current round is hide or double back
    int offset = 0;
    while(shouldTrace && player == PLAYER_DRACULA) {
        if(!strncmp("HI", &g->pastPlays[i+1-offset], ABBREV_CHARS)) {
            offset += PLAY_CHARS*NUM_PLAYERS;
        } else if(g->pastPlays[i+1-offset] == 'D' && g->pastPlays[i+2-offset] <= '5') {
            offset += PLAY_CHARS*NUM_PLAYERS * ((g->pastPlays[i+2-offset]-'0'));
        } else break;
    }

    //Determine discovered locations's region identifier and correct for teleport
    char abbrev[] = {g->pastPlays[i+1-offset], g->pastPlays[i+2-offset], '\0'};
    if(shouldTrace && strcmp(abbrev, "TP")==0) strcpy(abbrev, "CD");
    return abbrevToID(abbrev);
}

LocationID getLocation(GameView g, PlayerID player) {
	return getRealPosition(g, getRound(g) - (getCurrentPlayer(g) <= player), player, FALSE); 
}

// Fills the trail array with the location ids of the last SIX turns
void getHistory(GameView g, PlayerID player, LocationID trail[TRAIL_SIZE]) {
	int roundOfLastMove = getRound(g) - (getCurrentPlayer(g) <= player);
	for (int i = 0; i < TRAIL_SIZE; i++) {
		if((roundOfLastMove-i)>=0) trail[i] = getRealPosition(g, (roundOfLastMove-i), player, FALSE);
		else trail[i] = NOWHERE;
	}
}

LocationID *connectedLocations(GameView g, int *numLocations, LocationID from, PlayerID player, Round round, int road, int rail, int sea) {
	assert(from != UNKNOWN_LOCATION);
	rail *= ((round + player) % 4);
	Set s = newSet();
	SetInsert(s, from);
	if (sea) insertConnections (g -> map, s, from, BOAT);
	if (road) insertConnections (g -> map, s, from, ROAD);
	if (player == PLAYER_DRACULA) {
		//Remove inaccessible location
		SetDelete(s, ST_JOSEPH_AND_ST_MARYS);

		//Remove innaccessible parts of trail
		int roundOfLastMove = getRound(g) - (getCurrentPlayer(g) <= PLAYER_DRACULA);
		int latestPos = getRealPosition(g, roundOfLastMove, PLAYER_DRACULA, TRUE);
		int canDoubleBack = (searchHistory(g, PLAYER_DRACULA, DOUBLE_BACK_1, DOUBLE_BACK_5, FALSE)<0);		
		int canHide = (searchHistory(g, PLAYER_DRACULA, HIDE, HIDE, FALSE)<0) && (latestPos==CITY_UNKNOWN || idToType(latestPos)==LAND);
		Set t = newSet();

		//Two sets required as wanted things get deleted if they are disguised as DN/H
		for(int i=0; i<(TRAIL_SIZE-1) && roundOfLastMove-i >= 0; i++) {
			int pos = getRealPosition(g, roundOfLastMove-i, PLAYER_DRACULA, FALSE);
			int realPos = getRealPosition(g, roundOfLastMove-i, PLAYER_DRACULA, TRUE);
			if(i >= (canHide && !canDoubleBack) + canDoubleBack*(TRAIL_SIZE-1)) SetDelete(s, pos);
			else if(SetMember(s, realPos)) SetInsert(t, realPos);
		}
		s = SetUnion(s, t);
		
	//Add rail locations if permitted
	} else if (rail) {
		Set r = newSet();
	    insertConnections (g->map, r, from, RAIL);
	    while (--rail) {
            int *railConnections = Settoa(r);
			int num = SetCard(r);
            for (int i = 0; i < num; i++) insertConnections (g -> map, r, railConnections[i], RAIL);
            free (railConnections);
    	}
    	s = SetUnion(s, r);
	}
	(*numLocations) = SetCard(s);
	LocationID *locations = Settoa(s);
	dropSet(s);
	return locations;
}



//Returns move required to access a VALID next position
LocationID connectedMove(GameView g, LocationID pos) {
	int index = searchHistory(g, PLAYER_DRACULA, pos, pos, FALSE);
    if(index >=0) {
        if(searchHistory(g, PLAYER_DRACULA, pos, pos, TRUE)==0 && searchHistory(g, PLAYER_DRACULA, DOUBLE_BACK_1, DOUBLE_BACK_5, FALSE)>=0) return HIDE;
        else return DOUBLE_BACK_1 + index;
    }
	return pos;
}

//Finds shortest path
int getPath(GameView g, LocationID src, LocationID dest, LocationID *path, int *weight, PlayerID player, int road, int rail, int sea) {
	int *prev = malloc(NUM_MAP_LOCATIONS * sizeof(int));
	int *dist = getDistanceMap(g, src, prev, weight, player, road, rail, sea);
	int min = 0;
	if(dist[dest] == OUT_OF_RANGE) min = NOWHERE;
	else {
		int reversed[NUM_MAP_LOCATIONS];
		for(int i = dest; prev[i] != NOWHERE; i = prev[i]) reversed[min++] = i;
		if(path != NULL) for(int i = 0, j = min-1; i<min; i++, j--) path[i] = reversed[j];
	}
	free(dist);
	free(prev);
	
	return min;
}

//Generates an array containing the minimum distances from a sources to each location
int *getDistanceMap(GameView g, LocationID src, int *prev, int *weight, PlayerID player, int road, int rail, int sea) {
	//Each city's optimal previous and cumulative distance from start
	int round = getRound(g);
	int *dist = malloc(NUM_MAP_LOCATIONS * sizeof(int));
	//Ensure starting distance and previous values are overwritable
	for(int i=0; i<NUM_MAP_LOCATIONS; i++) {
		if(prev != NULL) prev[i] = NOWHERE;
		dist[i] = OUT_OF_RANGE;
	}
	dist[src] = 0;
	//Add connected cities to queue and iterate until all potential routes exhasted
	Queue q = newQueue();
	enqueue(q, src);
	while(!isEmpty(q)) {
		int numEdges = 0;
		LocationID curr = dequeue(q);
		LocationID *currEdges = connectedLocations(g, &numEdges, curr, player, round+dist[curr], road, rail, sea);
		for(int i=0; i<numEdges; i++) {
			//Branch (distance + weight) less than whats there
			int edgePenalty = ((weight!=NULL)?(weight[currEdges[i]]):1);
			if(currEdges[i] != UNKNOWN_LOCATION && (dist[curr]+edgePenalty) < dist[currEdges[i]]) {
				dist[currEdges[i]] = dist[curr]+edgePenalty;;
				if(prev != NULL) prev[currEdges[i]] = curr;
				enqueue(q, currEdges[i]);
			}
		}
	}
	return dist;
}

//Returns number of player encounters matching search char in given round
int getRoundEncounters(GameView g, PlayerID player, int round, char encounterType) {
	int playIndex = PLAY_CHARS*NUM_PLAYERS*round + player*PLAY_CHARS;
    int encounters = 0;
    for(int k = ENCOUNTER_START; k <= ENCOUNTER_END; k++) {
        encounters += (g->pastPlays[playIndex+k] == encounterType);	
    }
    return encounters;
}

//Returns true if item is in array and saves index
int searchHistory(GameView g, PlayerID player, int searchMin, int searchMax, int shouldTrace) {
	int roundOfLastMove = getRound(g) - (getCurrentPlayer(g) <= player);
	for (int i = 0; i < TRAIL_SIZE-1 && (roundOfLastMove-i)>=0; i++) {
		int pos = getRealPosition(g, (roundOfLastMove-i), player, shouldTrace);
		if(pos >= searchMin && pos <= searchMax) return i;
	}
	return NOWHERE;
}


//Returns true if a given move will be available in x rounds
int willBeAvailable(GameView g, LocationID searchMin, LocationID searchMax, int roundOffset) {
    int index = searchHistory(g, PLAYER_DRACULA, searchMin, searchMax, FALSE);
    if(index >=0) return (TRAIL_SIZE-1 - index - roundOffset <= 0);
    else return TRUE;
}

char *roundMessage(GameView g, PlayerID player, Round round) {
	return g->messages[round * NUM_PLAYERS + player];
}