/**
 * Runs a player's game turn ...
 *
 * Can produce either a Hunter player or a Dracula player
 * depending on the setting of the I_AM_DRACULA #define
 *
 * This is a dummy version of the real player.c used when
 * you submit your AIs. It is provided so that you can
 * test whether your code is likely to compile ...
 *
 * Note that it used to drive both Hunter and Dracula AIs.
 * It first creates an appropriate view, and then invokes the
 * relevant decide-my-move function, which should use the 
 * registerBestPlay() function to send the move back.
 *
 * The real player.c applies a timeout, so that it will halt
 * decide-my-move after a fixed amount of time, if it doesn't
 * finish first. The last move registered will be the one
 * used by the game engine. This version of player.c won't stop
 * your decide-my-move function if it goes into an infinite
 * loop. Sort that out before you submit.
 *
 * Based on the program by David Collien, written in 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#ifdef I_AM_DRACULA
#include "DracView.h"
#include "dracula.h"
#else
#include "HunterView.h"
#include "hunter.h"
#endif

// moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

int main(int argc, char *argv[]) {
    if(argc < 1) {
        printf("Using custom past plays:\n");
        PlayerMessage msgs[3] = { "", "", "" };
        char *plays = argv[1];
        #ifdef I_AM_DRACULA
        DracView gameState;
        gameState = newDracView(plays,msgs);
        decideDraculaMove(gameState);
        disposeDracView(gameState);
        #else
        HunterView gameState;
        gameState = newHunterView(plays,msgs);
        decideHunterMove(gameState);
        disposeHunterView(gameState);
        #endif

    } else {
        #ifdef I_AM_DRACULA
        DracView gameState;
        char *plays = "GZA.... SED.... HZU.... MZU....";
        PlayerMessage msgs[3] = { "", "", "" };
        gameState = newDracView(plays,msgs);
        decideDraculaMove(gameState);
        disposeDracView(gameState);
        #else
        HunterView gameState;
        char *plays = "GZA.... SED.... HZU....";
        PlayerMessage msgs[3] = { "", "", "" };
        gameState = newHunterView(plays,msgs);
        decideHunterMove(gameState);
        disposeHunterView(gameState);
        printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
        
        // test 2
        HunterView gameState2;
        char *plays2 = "GGE.... SGE.... HGE.... MGE.... DS?.... "
                        "GST.... SST.... HST.... MST.... DD1.... "
                        "GCO.... SMU....";
        PlayerMessage newmsgs[12] = { "Hello","Rubbish","Stuff","","Mwahahah","","","","","","","" };
        gameState2 = newHunterView(plays2,newmsgs);
        decideHunterMove(gameState2);
        disposeHunterView(gameState2);
        printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
        
        // test 3
        PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
        gameState2 = newHunterView("GST.... SAO.... HCD.... MAO.... DC?.... GGED...", messages3);
        decideHunterMove(gameState2);
        disposeHunterView(gameState2);
        printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
        
        // test 4
        PlayerMessage messages4[] = {"Hello","holiday in atlantic ocean","Stuff","","Mwahahah","Aha!",
                                        "~MSMRGE","","","hehe","I'm going to travel in Paris"};
        gameState2 = newHunterView("GST.... SAO.... HCD.... MAO.... DC?.... GGED... "
                                        "SMS.... HGA.... MNS.... DC?.... GPA....", messages4);
        decideHunterMove(gameState2);
        disposeHunterView(gameState2);
        printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
        
        // test 5
        PlayerMessage messages5[] = {"Hello","holiday in atlantic ocean","Stuff","","Mwahahah","Aha!",
                                        "~MSMRGE","","","hehe"};
        gameState2 = newHunterView("GST.... SHA.... HCD.... MAO.... DC?.... GMA.... "
                                        "SBRD... HGA.... MNS.... DC?....", messages5);
        decideHunterMove(gameState2);
        disposeHunterView(gameState2);
        
        #endif 
   
    }
   printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
   return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay (char *play, PlayerMessage message) {
   strncpy(latestPlay, play, MOVE_SIZE-1);
   latestPlay[MOVE_SIZE-1] = '\0';
     
   strncpy(latestMessage, message, MESSAGE_SIZE);
   latestMessage[MESSAGE_SIZE-1] = '\0';
}
