// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;
    
    printf("Begin testing the supplied tests\n");
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);
	 
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
	
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
	
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    disposeGameView(gv);
	
    printf("passed the supplied tests\n");
	
	//Possibly flawed tests
	printf("Tee's possibly flawed tests\n");
	
    printf("Testing border and suspicious cities\n");
    PlayerMessage messages6[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GAS.... SLE.... HZU.... MLI.... DC?....", messages6);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == LE_HAVRE);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == LE_HAVRE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == LEIPZIG);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == CITY_UNKNOWN);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeGameView(gv);
	
    printf("Testing hunters in the same location and dracula starting at sea\n");
    PlayerMessage messages7[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GAS.... SAS.... HAS.... MAS.... DS?....", messages7);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ADRIATIC_SEA);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ADRIATIC_SEA);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == ADRIATIC_SEA);
    assert(getLocation(gv,PLAYER_DRACULA) == SEA_UNKNOWN);
	assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == SEA_UNKNOWN);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeGameView(gv);
	
    printf("Testing basic functionalities of game at end of second round\n");
    PlayerMessage messages8[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GZU.... SST.... HMR.... MCF.... DC?....", messages8);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 2);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == ZURICH);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == MARSEILLES);
    assert(getLocation(gv,PLAYER_MINA_HARKER) ==  CLERMONT_FERRAND);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
	assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[2] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == STRASBOURG);
	assert(history[1] == GENEVA);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == MARSEILLES);
	assert(history[1] == GENEVA);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == CLERMONT_FERRAND);
	assert(history[1] == GENEVA);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == CITY_UNKNOWN);
	assert(history[1] == SEA_UNKNOWN);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*2));
    printf("passed\n");
    disposeGameView(gv);
	
    printf("Testing basic functionalities of game at end of third round\n");
    PlayerMessage messages9[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GLS.... SAT.... HMN.... MBR.... DC?.... "
		             "GMA.... SVA.... HLV.... MLI.... DC?.... "
                     "GGR.... SSA.... HSW.... MFR.... DC?....", messages9);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 3);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GRANADA);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == SALONICA);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == SWANSEA);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == FRANKFURT);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
	assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == GRANADA);
	assert(history[1] == MADRID);
	assert(history[2] == LISBON);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == SALONICA);
	assert(history[1] == VALONA);
	assert(history[2] == ATHENS);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == SWANSEA);
	assert(history[1] == LIVERPOOL);
	assert(history[2] == MANCHESTER);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == FRANKFURT);
	assert(history[1] == LEIPZIG);
	assert(history[2] == BERLIN);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == CITY_UNKNOWN);
	assert(history[1] == CITY_UNKNOWN);
	assert(history[2] == CITY_UNKNOWN);
	assert(history[5] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*3));
    printf("passed\n");
    disposeGameView(gv);
	
	printf("Testing a hunter encountering Dracula\n");
    PlayerMessage messages10[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GAS.... SLE.... HZU.... MLI.... DVE.... GVED...", messages10);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == VENICE);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == LE_HAVRE);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(getLocation(gv,PLAYER_DRACULA) == VENICE);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == VENICE);
	assert(history[1] == ADRIATIC_SEA);
	assert(history[2] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == LE_HAVRE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == LEIPZIG);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == VENICE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeGameView(gv);
	
	printf("Testing two hunters encountering Dracula\n");
    PlayerMessage messages11[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DZU.... "
                     "GZUD... SZUD...", messages11);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == ZURICH);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ZURICH);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == GENEVA);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == GENEVA);
    assert(getLocation(gv,PLAYER_DRACULA) == ZURICH);
	assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - (LIFE_LOSS_HUNTER_ENCOUNTER*2));
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[2] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == GENEVA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == GENEVA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeGameView(gv);
	
	printf("Testing all four hunters encountering Dracula (Dracula gets healed to 50 health first)\n");
    PlayerMessage messages12[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GGA.... SKL.... HGA.... MKL.... DCD.... "
                     "GCDD... SCDD... HCDD... MCDD...", messages12);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == CASTLE_DRACULA);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == CASTLE_DRACULA);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == CASTLE_DRACULA);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == CASTLE_DRACULA);
    assert(getLocation(gv,PLAYER_DRACULA) == CASTLE_DRACULA);
	assert(getHealth(gv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(getHealth(gv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(getHealth(gv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA - (LIFE_LOSS_HUNTER_ENCOUNTER*4));
	getHistory(gv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == GALATZ);
	assert(history[2] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_DR_SEWARD,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == KLAUSENBURG);
	assert(history[3] == UNKNOWN_LOCATION);
	getHistory(gv,PLAYER_VAN_HELSING,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == GALATZ);
	assert(history[4] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_MINA_HARKER,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == KLAUSENBURG);
	assert(history[5] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DRACULA,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeGameView(gv);
	
	printf("Testing for Dracula doublebacking (2)\n");
    PlayerMessage messages18[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GST.... SST.... HST.... MST.... DC?.... "
					   "GGE.... SST.... HGE.... MST.... DD2....", messages18);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_2);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_2);
    assert(history[1] == CITY_UNKNOWN);
	assert(history[2] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    printf("passed\n");
    disposeGameView(gv);
	
	printf("Testing for score\n");
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GSTVD.. SST.... HST.... MST.... DC?.... "
					   "GGETVD. SST.... HGE.... MST.... DD2...."
					   , messages18);
	assert(getScore(gv) == GAME_START_SCORE - 3 * SCORE_LOSS_DRACULA_TURN 
	                        - SCORE_LOSS_HUNTER_HOSPITAL);	
	disposeGameView(gv);
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GSTVD.. SSTVD.. HST.... MST.... DC?.... "
					 "GGETVD. SGETVD. HGE.... MST.... DD2..V."
					   , messages18);
	assert(getScore(gv) == GAME_START_SCORE - 3 * SCORE_LOSS_DRACULA_TURN 
	                       - 2 * SCORE_LOSS_HUNTER_HOSPITAL- SCORE_LOSS_VAMPIRE_MATURES);
	disposeGameView(gv);
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DC?.... "
					 "GGET... SCD.... HGE.... MST.... DD2..V."
					   , messages18);
	assert(getScore(gv) == GAME_START_SCORE - 3 * SCORE_LOSS_DRACULA_TURN 
	                        - SCORE_LOSS_VAMPIRE_MATURES);
    disposeGameView(gv);

    printf("Testing getConnections\n");
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GSTVD.. SST.... HST.... MST.... DC?.... "
					   "GGETVD. SST.... HGE.... MST.... DD2...."
					   , messages18);
    int num = 0;
    LocationID *locations = connectedLocations(gv, &num, GENEVA, PLAYER_LORD_GODALMING, 0, TRUE, TRUE, TRUE);
    assert(num == 6); free(locations);
    locations = connectedLocations(gv, &num, GENEVA, PLAYER_LORD_GODALMING, 1, TRUE, TRUE, TRUE);
    assert(num == 7); free(locations);
    locations = connectedLocations(gv, &num, GENEVA, PLAYER_LORD_GODALMING, 2, TRUE, TRUE, TRUE);
    assert(num == 9); free(locations);
    locations = connectedLocations(gv, &num, GENEVA, PLAYER_LORD_GODALMING, 3, TRUE, TRUE, TRUE);
    assert(num == 10); free(locations);
    disposeGameView(gv);
    
    printf("Testing findPath\n");
    //LocationID path[NUM_MAP_LOCATIONS-1];
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GSTVD.. SST.... HST.... MST.... DC?.... "
					   "GGETVD. SST.... HGE.... MST.... DD2...."
					   , messages18);
    /*int getPath(GameView g, LocationID src, LocationID dest, LocationID *path, int *weight, PlayerID player, int road, int rail, int sea) */
    assert(getPath(gv, GENEVA, GENEVA, NULL, NULL, PLAYER_LORD_GODALMING, TRUE, TRUE, TRUE) == 0);
    assert(getPath(gv, CASTLE_DRACULA, GALATZ,NULL, NULL, PLAYER_LORD_GODALMING, TRUE, TRUE, TRUE) == 1);
    assert(getPath(gv, GENEVA, MANCHESTER, NULL, NULL, PLAYER_LORD_GODALMING, TRUE, TRUE, TRUE) == 5);
    assert(getRound(gv) == 3);
    assert(getPath(gv, MARSEILLES, PARIS,NULL, NULL, PLAYER_LORD_GODALMING, TRUE, TRUE, TRUE) == 1);
    assert(getPath(gv, MARSEILLES, PARIS, NULL,NULL, PLAYER_LORD_GODALMING, TRUE, FALSE, TRUE) == 2);
    assert(getPath(gv, CASTLE_DRACULA, VENICE, NULL, NULL, PLAYER_DRACULA, TRUE, FALSE, TRUE) == 5);
    disposeGameView(gv);

    printf("Double checking health\n");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GSTTD.. SST.... HST.... MST.... DC?.... "
					   "GGETVD. SST.... HGE.... MST.... DD2....", messages18);
    disposeGameView(gv);

    printf("Passed all tests\n");
	
    return 0;
}