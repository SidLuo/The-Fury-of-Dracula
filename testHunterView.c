// testHunterView.c ... test the HunterView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "HunterView.h"

int main()
{
    int i;
    HunterView hv;
    
    printf("Begin testing the supplied tests\n");
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    hv = newHunterView("", messages1);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 0);
    assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(giveMeTheScore(hv) == GAME_START_SCORE);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeHunterView(hv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(whereIs(hv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeHunterView(hv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    hv = newHunterView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(whereIs(hv,PLAYER_DRACULA) == GENEVA);
    assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == 5);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == 30);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeHunterView(hv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    hv = newHunterView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(whereIs(hv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(whoAmI(hv) == 0);
    printf("passed\n");
    disposeHunterView(hv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;

    printf("Checking Galatz road connections\n");
    PlayerMessage messages5[] = {"Gone to Galatz"};
    hv = newHunterView("GGA....", messages5);    
    edges = whereCanTheyGo(hv,&size,PLAYER_LORD_GODALMING,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    disposeHunterView(hv);

    printf("Checking Ionian Sea sea connections\n");
    PlayerMessage messages6[] = {"Sailing the Ionian"};
    hv = newHunterView("GIO....", messages6);
    edges = whereCanTheyGo(hv,&size,PLAYER_LORD_GODALMING,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    disposeHunterView(hv);

    printf("Checking Athens rail connections (none)\n");
    PlayerMessage messages7[] = {"Leaving Athens by train"};
    hv = newHunterView("GAT....", messages7);
    edges = whereCanTheyGo(hv,&size,PLAYER_LORD_GODALMING,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    disposeHunterView(hv);

    printf("passed the supplied tests\n");
	
	//Possibly flawed tests
	printf("Tee's possibly flawed tests\n");
	
    printf("Testing border and suspicious cities\n");
    PlayerMessage messages33[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GAS.... SLE.... HZU.... MLI.... DC?....", messages33);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == LE_HAVRE);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(whereIs(hv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == LE_HAVRE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == LEIPZIG);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == CITY_UNKNOWN);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeHunterView(hv);
	
    printf("Testing hunters in the same location and dracula starting at sea\n");
    PlayerMessage messages34[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GAS.... SAS.... HAS.... MAS.... DS?....", messages34);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == ADRIATIC_SEA);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == ADRIATIC_SEA);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == ADRIATIC_SEA);
    assert(whereIs(hv,PLAYER_DRACULA) == SEA_UNKNOWN);
	assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == SEA_UNKNOWN);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeHunterView(hv);
	
    printf("Testing basic functionalities of game at end of second round\n");
    PlayerMessage messages8[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GZU.... SST.... HMR.... MCF.... DC?....", messages8);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 2);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == ZURICH);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == MARSEILLES);
    assert(whereIs(hv,PLAYER_MINA_HARKER) ==  CLERMONT_FERRAND);
    assert(whereIs(hv,PLAYER_DRACULA) == CITY_UNKNOWN);
	assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == STRASBOURG);
	assert(history[1] == GENEVA);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == MARSEILLES);
	assert(history[1] == GENEVA);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == CLERMONT_FERRAND);
	assert(history[1] == GENEVA);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == CITY_UNKNOWN);
	assert(history[1] == SEA_UNKNOWN);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*2));
    printf("passed\n");
    disposeHunterView(hv);
	
    printf("Testing basic functionalities of game at end of third round\n");
    PlayerMessage messages9[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GLS.... SAT.... HMN.... MBR.... DC?.... "
		             "GMA.... SVA.... HLV.... MLI.... DC?.... "
                     "GGR.... SSA.... HSW.... MFR.... DC?....", messages9);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 3);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == GRANADA);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == SALONICA);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == SWANSEA);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == FRANKFURT);
    assert(whereIs(hv,PLAYER_DRACULA) == CITY_UNKNOWN);
	assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == GRANADA);
	assert(history[1] == MADRID);
	assert(history[2] == LISBON);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == SALONICA);
	assert(history[1] == VALONA);
	assert(history[2] == ATHENS);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == SWANSEA);
	assert(history[1] == LIVERPOOL);
	assert(history[2] == MANCHESTER);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == FRANKFURT);
	assert(history[1] == LEIPZIG);
	assert(history[2] == BERLIN);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == CITY_UNKNOWN);
	assert(history[1] == CITY_UNKNOWN);
	assert(history[2] == CITY_UNKNOWN);
	assert(history[5] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*3));
    printf("passed\n");
    disposeHunterView(hv);
	
	printf("Testing a hunter encountering Dracula\n");
    PlayerMessage messages10[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GAS.... SLE.... HZU.... MLI.... DVE.... GVED...", messages10);
    assert(whoAmI(hv) == PLAYER_DR_SEWARD);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == VENICE);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == LE_HAVRE);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(whereIs(hv,PLAYER_DRACULA) == VENICE);
    assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == VENICE);
	assert(history[1] == ADRIATIC_SEA);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == LE_HAVRE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == LEIPZIG);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == VENICE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeHunterView(hv);
	
	printf("Testing two hunters encountering Dracula\n");
    PlayerMessage messages11[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GGE.... SGE.... HGE.... MGE.... DZU.... "
                     "GZUD... SZUD...", messages11);
    assert(whoAmI(hv) == PLAYER_VAN_HELSING);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == ZURICH);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == ZURICH);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == GENEVA);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == GENEVA);
    assert(whereIs(hv,PLAYER_DRACULA) == ZURICH);
	assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - (LIFE_LOSS_HUNTER_ENCOUNTER*2));
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == GENEVA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == GENEVA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeHunterView(hv);
	
	printf("Testing all four hunters encountering Dracula (Dracula gets healed to 50 health first)\n");
    PlayerMessage messages12[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GGA.... SKL.... HGA.... MKL.... DCD.... "
                     "GCDD... SCDD... HCDD... MCDD...", messages12);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == CASTLE_DRACULA);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == CASTLE_DRACULA);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == CASTLE_DRACULA);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == CASTLE_DRACULA);
    assert(whereIs(hv,PLAYER_DRACULA) == CASTLE_DRACULA);
	assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(hv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(hv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA - (LIFE_LOSS_HUNTER_ENCOUNTER*4));
	giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == GALATZ);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == KLAUSENBURG);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(hv,PLAYER_VAN_HELSING,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == GALATZ);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_MINA_HARKER,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == KLAUSENBURG);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeHunterView(hv);
	
	printf("Testing for Dracula doublebacking (2)\n");
    PlayerMessage messages18[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    hv = newHunterView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GST.... SST.... HST.... MST.... DC?.... "
					   "GGE.... SST.... HGE.... MST.... DD2....", messages18);
    assert(whereIs(hv,PLAYER_DRACULA) == DOUBLE_BACK_2);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_2);
    assert(history[1] == CITY_UNKNOWN);
	assert(history[2] == SEA_UNKNOWN);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    printf("passed\n");
    disposeHunterView(hv);

    hv = newHunterView("GGE.... SSZ.... HST.... MCO.... DC?.V.. GST.... SBC.... HMI.... MLI.... DC?T... GFR.... SSO.... HZU.... MNU.... DS?.... GNU.... SVA.... HST.... MPR.... DS?.... GST.... SIO.... HFR.... MFR.... DC?T... GZU.... STS.... HNU.... MCO.... DD1T... GST.... SNP.... HST.... MLI.... DHIT.V. GMI.... STS.... HZU.... MNU.... DTPT.M. GZU.... SNP.... HST.... MST.... DC?T... GST.... SFL.... HMI.... MZU.... DC?T... GFR.... SST.... HZU.... MST.... DCDT.M. GNU.... SZU.... HST.... MFR.... DD1T.M. GST.... SST.... HFR.... MNU.... DHI..M. GZU.... SFR.... HNU.... MST.... DTP.VM. GST.... SNU.... HST.... MZU.... DC?T.M. GMI.... SST.... HZU.... MST.... DC?T.M. GZU.... SZU.... HST.... MMI.... DC?T.M. GST.... SGE.... HZU.... MGO.... DD2T.M. GFR.... SPA.... HGE.... MFL.... DS?.... GPR.... SST.... HZU.... MNP.... DS?..V. GVI.... SGE.... HGE.... MMI.... DC?T.M. GVE.... SCF.... HPA.... MGO.... DC?T.M. GFL.... STO.... HST.... MFL.... DHIT.M. GGO.... SBA.... HGE.... MNP.... DD2T.M. GFL.... SSR.... HCF.... MMI.... DTPT... GGO.... SBA.... HTO.... MGO.... DC?T... GFL.... SSR.... HBA.... MFL.... DC?.VM. GGO.... STO.... HSR.... MNP.... DC?T.M. GFL.... SBA.... HBA.... MMI.... DC?T.M. GGO.... SMS.... HSR.... MGO.... DD4T.M. GFL.... STS.... HTO.... MFL.... DCDT.M. GGO.... SRO.... HBA.... MNP.... DC?T.M. GFL.... SFL.... HMS.... MMI.... DC?T.V. GGO.... SNP.... HTS.... MGO.... DHIT.M. GFL.... SMI.... HRO.... MFL.... DS?..M. GGO.... SGO.... HFL.... MNP.... DS?..M. GFL.... SFL.... HNP.... MMI.... DS?..M. GGO.... SNP.... HMI.... MGO.... DS?..M. GFL.... SMI.... HGO.... MFL.... DD2..M. GGO.... SGO.... HFL.... MNP.... DC?.VM. GFL.... SFL.... HNP.... MMI.... DHIT... GGO.... SNP.... HMI.... MGO.... DTPT... GFL.... SMI.... HGO.... MFL.... DC?T... GGO.... SGO.... HFL.... MNP.... DC?T... GMR.... SMR.... HRO.... MFL.... DD2T... GCF.... SBU.... HGO.... MRO.... DC?T.V. GMR.... SBO.... HMR.... MTS.... DC?T.M. GMI.... SNA.... HCF.... MIO.... DC?T.M. GMR.... SCF.... HMR.... MBS.... DS?..M. GCF.... SMR.... HMI.... MIO.... DS?..M. GMR.... SMI.... HMR.... MBS.... DS?..M. GMI.... SMR.... HCF.... MIO.... DC?T.M. GMR.... SCF.... HMR.... MBS.... DC?.VM. GCF.... SMR.... HMI.... MIO.... DD1T.M. GMR.... SMI.... HMR.... MBS.... DC?T... GMI.... SMR.... HCF.... MIO.... DC?T... GMR.... SCF.... HMR.... MBS.... DC?T... GCF.... SMR.... HMI.... MIO.... DHIT.M. GMR.... SMI.... HMR.... MBS.... DS?..V. GMI.... SMR.... HCF.... MIO.... DS?..M. GMR.... SCF.... HMR.... MBS.... DC?T.M. GCF.... SMR.... HMI.... MIO.... DD1T.M. GMR.... SMI.... HMR.... MBS.... DTPT.M. GMI.... SMR.... HCF.... MIO.... DHIT.M. GMR.... SCF.... HMR.... MBS.... DC?T... GCF.... SMR.... HMI.... MIO.... DC?.V.. GMR.... SMI.... HMR.... MBS.... DC?T.M. GMI.... SMR.... HCF.... MIO.... DD2T.M. GMR.... SCF.... HMR.... MBS.... DC?T.M. GCF.... SMR.... HMI.... MIO.... DHIT.M. GMR.... SMI.... HMR.... MBS.... DC?T.M. GMI.... SMR.... HCF.... MIO.... DC?T.V. GMR.... SCF.... HMR.... MBS.... DC?T.M. GCF.... SMR.... HMI.... MIO.... DCNT.M. GMR.... SMI.... HMR.... MBS.... DS?..M. GMI.... SMR.... HCF.... MIO.... DVRT.M. GMR.... SCF.... HMR.... MBS.... DD3T.M. GCF.... SMR.... HCF.... MCNTTD. DC?T.M. GCF.... SCF.... HCF.... MVRT... DC?.VM. GCF.... SCF.... HCF....", messages18);

    //I am Mina at varna
    int possibilities = 0;
    LocationID *possibleNext = whereCanTheyGo(hv, &possibilities, whoAmI(hv), TRUE, TRUE, TRUE);
    for(int i=0; i<possibilities; i++) printf("%s, ", idToName(possibleNext[i]));
	printf("Mina has %d health\n", howHealthyIs(hv, whoAmI(hv)));

	printf("Passed all tests\n");
	
    return 0;
}