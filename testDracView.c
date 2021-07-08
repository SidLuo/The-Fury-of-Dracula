// testDracView.c ... test the DracView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "DracView.h"

int main()
{
    int i;
    DracView dv;
	LocationID history[TRAIL_SIZE];

    printf("Begin testing the supplied tests\n");
    PlayerMessage messages1[] = {"Hello","Rubbish","Stuff",""};
    dv = newDracView("GCD.... SSZ.... HST.... MCO.... DGE.V.. GCD.... SSZ.... HST.... MCO.... DD1T... GCD.... SSZ.... HST.... MCO.... DHIT... GCD.... SSZ.... HST.... MCO.... DCFT... GCD.... SSZ.... HST.... MCO.... DBOT... GCD.... SSZ.... HST.... MCO.... DNAT... GCD.... SSZ.... HST.... MCO.... DLET.V. GCD.... SSZ.... HST.... MCO.... DBUT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DAMT.M. GCD.... SSZ.... HST.... MCO.... DCOT.M. GCD.... SSZ.... HST.... MCOTD.. DFRT.M. GCD.... SSZ.... HST.... MCO.... DLI.VM. GCD.... SSZ.... HST.... MCO.... DBRT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DHAT... GCD.... SSZ.... HST.... MCO.... DCOT.M. GCD.... SSZ.... HST.... MCOTD.. DAMT.V. GCD.... SSZ.... HST.... MCO.... DBUT.M. GCD.... SSZ.... HST.... MCO.... DLET.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DNAT... GCD.... SSZ.... HST.... MCO.... DBOT.M. GCD.... SSZ.... HST.... MCO.... DCF.VM. GCD.... SSZ.... HST.... MCO.... DGET.M. GCD.... SSZ.... HST.... MCO.... DMRT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DGOT.M. GCD.... SSZ.... HST.... MCO.... DFLT.V. GCD.... SSZ.... HST.... MCO.... DROT.M. GCD.... SSZ.... HST.... MCO.... DBIT.M. GCD.... SSZ.... HST.... MCO.... DNPT.M. GCD.... SSZ.... HST.... MCO.... DTS..M. GCD.... SSZ.... HST.... MCO.... DCG..M. GCD.... SSZ.... HST.... MCO...."
                    , messages1);
    monteCarlo(dv, 5);
    printf("passed\n");
    disposeDracView(dv);
    printf("Test for basic functions, just before Dracula's first move\n");
   
    dv = newDracView("GST.... SAO.... HZU.... MBB....", messages1);
    assert(giveMeTheRound(dv) == 0);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(whereIs(dv,PLAYER_DRACULA) == UNKNOWN_LOCATION);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeDracView(dv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","",""};
    dv = newDracView("GST.... SAO.... HCD.... MAO.... DGE.... "
                     "GGED... SAO.... HCD.... MAO....", messages2);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_DRACULA) == GENEVA);
    assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == 5);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == 30);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == GENEVA);
    
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == ATLANTIC_OCEAN);
    assert(history[2] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeDracView(dv);

    printf("Test for Dracula leaving minions\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Drop a V","Party in Strasbourg","Party","Party","Party"};
    dv = newDracView("GGE.... SGE.... HGE.... MGE.... DED.V.. "
                     "GST.... SST.... HST.... MST.... DMNT... "
                     "GST.... SST.... HST.... MST....", messages3);
    int nT, nV;
    whatsThere(dv,EDINBURGH,&nT,&nV);
    assert(nT == 0 && nV == 1);
    whatsThere(dv,MANCHESTER,&nT,&nV);
    assert(nT == 1 && nV == 0);
    assert(whereIs(dv,PLAYER_DRACULA) == MANCHESTER);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    assert(history[0] == MANCHESTER);
    assert(history[1] == EDINBURGH);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
    assert(history[0] == STRASBOURG);
    assert(history[1] == STRASBOURG);
    assert(history[2] == GENEVA);
    assert(history[3] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeDracView(dv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;

    printf("Checking Galatz road connections\n");
    PlayerMessage messages5[] = {"Gone to Galatz"};
    dv = newDracView("GGA....", messages5);
    edges = whereCanTheyGo(dv,&size,PLAYER_LORD_GODALMING,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    disposeDracView(dv);

    printf("Checking Ionian Sea sea connections\n");
    PlayerMessage messages6[] = {"Sailing the Ionian"};
    dv = newDracView("GIO....", messages6);
    edges = whereCanTheyGo(dv,&size,PLAYER_LORD_GODALMING,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    disposeDracView(dv);

    printf("Checking Athens rail connections (none)\n");
    PlayerMessage messages7[] = {"Leaving Athens by train"};
    dv = newDracView("GAT....", messages7);
    edges = whereCanTheyGo(dv,&size,PLAYER_LORD_GODALMING,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    disposeDracView(dv);

    printf("passed the supplied tests\n");
	
	//Possibly flawed tests
	printf("Tee's possibly flawed tests\n");
	
	//Dracula Exclusive Tests
	printf("Testing for Dracula hiding\n");
    PlayerMessage messages17[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    dv = newDracView("GGE.... SGE.... HGE.... MGE.... DMN.... "
                       "GST.... SST.... HST.... MST.... DHI....", messages17);
    assert(whereIs(dv,PLAYER_DRACULA) == HIDE);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    assert(history[0] == HIDE);
    assert(history[1] == MANCHESTER);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeDracView(dv);
	
    printf("Testing a hunter passing Dracula at sea\n");
    PlayerMessage messages20[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GAS.... SAS.... HAS.... MAS.... DAS....", messages20);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_DRACULA) == ADRIATIC_SEA);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing hunters encountering trap and immature vampire in different location\n");
    PlayerMessage messages13[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GMI.... SMU.... HMN.... MBR.... DZU.V.. "
		             "GZU.... SNU.... HLV.... MLI.... DSTT... "
                     "GMI.... SSTT... HSW.... MFR....", messages13);
    assert(giveMeTheRound(dv) == 2);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == MILAN);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == SWANSEA);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == FRANKFURT);
    assert(whereIs(dv,PLAYER_DRACULA) == STRASBOURG);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*2));
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing hunters encountering multiple traps and immature vampires in same location\n");
    PlayerMessage messages14[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GFR.... SNU.... HNA.... MNA.... DST.V.. "
				     "GNU.... SNU.... HLE.... MNA.... DPAT... "
				     "GFR.... SNU.... HNA.... MNA.... DHIT... "
					 "GNU.... SNU.... HLE.... MNA.... DD1T... "
					 "GFR.... SNU.... HNA.... MNA.... DBUT... "
					 "GST.... SST.... HPATTT. MPA....", messages14);
    assert(giveMeTheRound(dv) == 5);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == PARIS);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == PARIS);
	assert(whereIs(dv,PLAYER_DRACULA) == BRUSSELS);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS - (LIFE_LOSS_TRAP_ENCOUNTER) * 3);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*5));
	int nTT; int nVV;
    whatsThere(dv,BRUSSELS,&nTT,&nVV);
    assert(nTT == 1);
	assert(nVV == 0);
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing hunters encountering multiple traps, immature vampire and Dracula resulting in hunter dying\n");
    PlayerMessage messages15[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GFR.... SNU.... HNA.... MNA.... DST.V.. "
				     "GFR.... SNU.... HNA.... MNA.... DHIT... "
					 "GNU.... SNU.... HLE.... MNA.... DD1T... "
					 "GSTTTTD SST.... HPA.... MPA....", messages15);
    assert(giveMeTheRound(dv) == 3);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == PARIS);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == PARIS);
    assert(whereIs(dv,PLAYER_DRACULA) == DOUBLE_BACK_1);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER * 3 - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*3) - SCORE_LOSS_HUNTER_HOSPITAL);
    printf("passed\n");
    disposeDracView(dv);
	
	//THIS POINT
	
	printf("Testing hunters encountering trap and immature vampire in different location and resting to heal\n");
    PlayerMessage messages16[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GMI.... SNU.... HMN.... MBR.... DZU.V.. "
		             "GZU.... SNU.... HLV.... MLI.... DSTT... "
                     "GMI.... SSTTD.. HSW.... MFR.... DBUT... "
				     "GMI.... SST.... HLV.... MLI....", messages16);
    assert(giveMeTheRound(dv) == 3);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == MILAN);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == LIVERPOOL);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(whereIs(dv,PLAYER_DRACULA) == BRUSSELS);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER - LIFE_LOSS_DRACULA_ENCOUNTER + LIFE_GAIN_REST);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*3));
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing for when original hide, doubleback or teleport location falls off trail resulting in vampire maturing and traps expiring\n");
    PlayerMessage messages19[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    dv = newDracView("GFR.... SNU.... HNA.... MNA.... DST.V.. "
				     "GNU.... SNU.... HLE.... MNA.... DFRT... "
				     "GFR.... SNU.... HNA.... MNA.... DHIT... "
					 "GNU.... SNU.... HLE.... MNA.... DNUT... "
					 "GFR.... SNU.... HNA.... MNA.... DMUT... "
					 "GST.... SST.... HPA.... MPA.... DVET... "
					 "GPA.... SST.... HPA.... MPA.... DFLT.V. "
				     "GST.... SST.... HPA.... MPA.... DROT... "
				     "GPA.... SST.... HPA.... MPA....", messages19);
    //assert(whereIs(dv,PLAYER_DRACULA) == FLORENCE);
    
	assert(giveMeTheScore(dv) == GAME_START_SCORE - SCORE_LOSS_VAMPIRE_MATURES - (SCORE_LOSS_DRACULA_TURN*8));
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    //assert(history[5] == FRANKFURT);
    printf("passed all tests\n");
    disposeDracView(dv);
	
	//Dracula Non-Exclusive Tests
    printf("Testing border and suspicious cities\n");
    PlayerMessage messages30[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GAS.... SLE.... HZU.... MLI....", messages30);
    assert(giveMeTheRound(dv) == 0);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == LE_HAVRE);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == LE_HAVRE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == LEIPZIG);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE);
	int start; int end;
	lastMove(dv, PLAYER_LORD_GODALMING, &start, &end);
	printf("start is %d", start);
	assert(start == UNKNOWN_LOCATION && end == ADRIATIC_SEA);
    printf("passed\n");
    disposeDracView(dv);
	
    printf("Testing hunters in the same location and dracula starting at sea\n");
    PlayerMessage messages31[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GAS.... SAS.... HAS.... MAS.... DIR....", messages31);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == ADRIATIC_SEA);
    assert(whereIs(dv,PLAYER_DRACULA) == IRISH_SEA);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == ADRIATIC_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[0] == IRISH_SEA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeDracView(dv);
	
    printf("Testing basic functionalities of game at end of second round\n");
    PlayerMessage messages8[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GGE.... SGE.... HGE.... MGE.... DIR.... "
                     "GZU.... SST.... HMR.... MCF.... DDU....", messages8);
    assert(giveMeTheRound(dv) == 2);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == ZURICH);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == STRASBOURG);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == MARSEILLES);
    assert(whereIs(dv,PLAYER_MINA_HARKER) ==  CLERMONT_FERRAND);
    assert(whereIs(dv,PLAYER_DRACULA) == DUBLIN);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == STRASBOURG);
	assert(history[1] == GENEVA);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == MARSEILLES);
	assert(history[1] == GENEVA);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == CLERMONT_FERRAND);
	assert(history[1] == GENEVA);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[0] == DUBLIN);
	assert(history[1] == IRISH_SEA);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*2));
	lastMove(dv, PLAYER_MINA_HARKER, &start, &end);
	assert(start == GENEVA && end == CLERMONT_FERRAND);
    printf("passed\n");
    disposeDracView(dv);
	
    printf("Testing basic functionalities of game at end of third round\n");
    PlayerMessage messages9[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GLS.... SAT.... HMN.... MBR.... DVI.... "
		             "GMA.... SVA.... HLV.... MLI.... DZA.... "
                     "GGR.... SSA.... HSW.... MFR.... DMU....", messages9);
    assert(giveMeTheRound(dv) == 3);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == GRANADA);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == SALONICA);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == SWANSEA);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == FRANKFURT);
    assert(whereIs(dv,PLAYER_DRACULA) == MUNICH);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == GRANADA);
	assert(history[1] == MADRID);
	assert(history[2] == LISBON);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == SALONICA);
	assert(history[1] == VALONA);
	assert(history[2] == ATHENS);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == SWANSEA);
	assert(history[1] == LIVERPOOL);
	assert(history[2] == MANCHESTER);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == FRANKFURT);
	assert(history[1] == LEIPZIG);
	assert(history[2] == BERLIN);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[0] == MUNICH);
	assert(history[1] == ZAGREB);
	assert(history[2] == VIENNA);
	assert(history[5] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - (SCORE_LOSS_DRACULA_TURN*3));
	lastMove(dv, PLAYER_VAN_HELSING, &start, &end);
	assert(start == LIVERPOOL && end == SWANSEA);
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing a hunter encountering Dracula\n");
    PlayerMessage messages10[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GAS.... SLE.... HZU.... MLI.... DVE.... GVED...", messages10);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == VENICE);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == LE_HAVRE);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == LEIPZIG);
    assert(whereIs(dv,PLAYER_DRACULA) == VENICE);
    assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == VENICE);
	assert(history[1] == ADRIATIC_SEA);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == LE_HAVRE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == LEIPZIG);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[0] == VENICE);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing two hunters encountering Dracula\n");
    PlayerMessage messages11[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GGE.... SGE.... HGE.... MGE.... DZU.... "
                     "GZUD... SZUD...", messages11);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == ZURICH);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == ZURICH);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == GENEVA);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == GENEVA);
    assert(whereIs(dv,PLAYER_DRACULA) == ZURICH);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - (LIFE_LOSS_HUNTER_ENCOUNTER*2));
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == ZURICH);
	assert(history[1] == GENEVA);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == GENEVA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == GENEVA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[0] == ZURICH);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing all four hunters encountering Dracula (Dracula gets healed to 50 health first)\n");
    PlayerMessage messages12[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    dv = newDracView("GGA.... SKL.... HGA.... MKL.... DCD.... "
                     "GCDD... SCDD... HCDD... MCDD...", messages12);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == CASTLE_DRACULA);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == CASTLE_DRACULA);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == CASTLE_DRACULA);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == CASTLE_DRACULA);
    assert(whereIs(dv,PLAYER_DRACULA) == CASTLE_DRACULA);
	assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
	assert(howHealthyIs(dv,PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA - (LIFE_LOSS_HUNTER_ENCOUNTER*4));
	giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == GALATZ);
	assert(history[2] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == KLAUSENBURG);
	assert(history[3] == UNKNOWN_LOCATION);
	giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == GALATZ);
	assert(history[4] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == KLAUSENBURG);
	assert(history[5] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
	assert(history[0] == CASTLE_DRACULA);
	assert(history[1] == UNKNOWN_LOCATION);
	assert(history[2] == UNKNOWN_LOCATION);
	assert(giveMeTheScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
    printf("passed\n");
    disposeDracView(dv);
	
	printf("Testing for Dracula doublebacking (2)\n");
    PlayerMessage messages18[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    dv = newDracView("GGE.... SGE.... HGE.... MGE.... DIR.... "
                       "GST.... SST.... HST.... MST.... DDU.... "
					   "GGE.... SST.... HGE.... MST.... DD2....", messages18);
    assert(whereIs(dv,PLAYER_DRACULA) == DOUBLE_BACK_2);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_2);
    assert(history[1] == DUBLIN);
	assert(history[2] == IRISH_SEA);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    printf("passed\n");
    disposeDracView(dv);

    dv = newDracView("GCD.... SSZ.... HST.... MCO.... DGE.V.. GCD.... SSZ.... HST.... MCO.... DD1T... GCD.... SSZ.... HST.... MCO.... DHIT... GCD.... SSZ.... HST.... MCO.... DCFT... GCD.... SSZ.... HST.... MCO.... DBOT... GCD.... SSZ.... HST.... MCO.... DNAT... GCD.... SSZ.... HST.... MCO.... DLET.V. GCD.... SSZ.... HST.... MCO.... DBUT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DAMT.M. GCD.... SSZ.... HST.... MCO.... DCOT.M. GCD.... SSZ.... HST.... MCOTD.. DFRT.M. GCD.... SSZ.... HST.... MCO.... DLI.VM. GCD.... SSZ.... HST.... MCO.... DBRT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DHAT... GCD.... SSZ.... HST.... MCO.... DCOT.M. GCD.... SSZ.... HST.... MCOTD.. DAMT.V. GCD.... SSZ.... HST.... MCO.... DBUT.M. GCD.... SSZ.... HST.... MCO.... DLET.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DNAT... GCD.... SSZ.... HST.... MCO.... DBOT.M. GCD.... SSZ.... HST.... MCO.... DCF.VM. GCD.... SSZ.... HST.... MCO.... DGET.M. GCD.... SSZ.... HST.... MCO.... DMRT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DGOT.M. GCD.... SSZ.... HST.... MCO.... DFLT.V. GCD.... SSZ.... HST.... MCO.... DROT.M. GCD.... SSZ.... HST.... MCO.... DBIT.M. GCD.... SSZ.... HST.... MCO.... DNPT.M. GCD.... SSZ.... HST.... MCO.... DD1T.M. GCD.... SSZ.... HST.... MCO.... DHIT.M. GCD.... SSZ.... HST.... MCO.... DTS..M. GCD.... SSZ.... HST.... MCO.... DCG.VM. GCD.... SSZ.... HST.... MCO....", messages18);

    //BI NP D1 HI TS CG
    
    assert(moveAvailable(dv, CAGLIARI, CAGLIARI, 0, FALSE) == FALSE);
    assert(moveAvailable(dv, TYRRHENIAN_SEA, TYRRHENIAN_SEA,0, FALSE) == FALSE);
    assert(moveAvailable(dv, TYRRHENIAN_SEA,TYRRHENIAN_SEA, 1, FALSE) == FALSE);
    assert(moveAvailable(dv, TYRRHENIAN_SEA, TYRRHENIAN_SEA,2, FALSE) == FALSE);
    assert(moveAvailable(dv, TYRRHENIAN_SEA, TYRRHENIAN_SEA,3, FALSE) == FALSE);
    assert(moveAvailable(dv, TYRRHENIAN_SEA, TYRRHENIAN_SEA,4, FALSE) == TRUE);
    assert(moveAvailable(dv, NAPLES, NAPLES,0, FALSE) == FALSE);
    assert(moveAvailable(dv, NAPLES,NAPLES, 1, FALSE) == TRUE);
    assert(moveAvailable(dv, BARI, BARI,0, FALSE) == TRUE);

    disposeDracView(dv);

    dv = newDracView("GCD.... SSZ.... HST.... MCO.... DMA.V.. GCD.... SSZ.... HST.... MCO....", messages18);

    int path[NUM_MAP_LOCATIONS];
    int num = findPath(dv, MADRID, GRANADA, path, NULL, PLAYER_DRACULA, TRUE, FALSE, TRUE);
    for(int i=0; i<num; i++) printf("%s, ", idToName(path[i]));

	printf("Passed all tests\n");

    return 0;
}