**Assignment 2: The Fury of Dracula**

**Objectives**
* to give you experience in implementing Graph-based ADTs
* to give you practice in implementing a heuristic game strategy

**Background**
The aim for this part of the Assignment is to write AIs that can play the "Fury of Dracula" game. The interface for the AIs is very simple: the Game Engine invokes the AI, giving it an appropriate View, waits for a certain amount of time while the AI thinks about its move (possibly suggesting several moves), and then takes the last move suggested by the AI. The Game Engine then takes the move, records it, updates its own game state, and then move on to the next AI (in the order specified in the Rules). The interface for your AIs is very simple: they take a View as input, and send a new location and a message as their output.

**Setting Up**
Make a directory for working on this Assignment, change into that directory, and run the command:
$ unzip /home/jas/web/tmp/hunt.zip

This will place the following files in your directory:
Makefile	a set of dependencies used to control compilation
player.c	a very simple main program to drive your AIs
dracula.h	interface to the Dracula AI
dracula.c	skeleton implementation of the Dracula AI
hunter.h	interface to the Hunter AI
hunter.c	skeleton implementation of the Hunter AI
If you have Views from the first part of the assignment that you're happy with, or new ones that you've developed since then, you can replace the supplied versions immediately with your own versions. If you don't have working Views from the Phase 1 of the assignment, then you'll have to wait until after Monday 26th September, when we'll release simple working versions of the Views for any groups that couldn't get them working in Phase 1.
Note that many of above files are provided simply as a convenience, and you can replace them with your own versions if you want. The only interfaces that you need to honour precisely as given are those for the AIs (i.e. hunter.h and dracula.h. If you plan to include other ADTs with your code, you'll need to add dependencies to the Makefile to ensure that they're compiled.

If you run make after copying your own View ADTs into place you should see something like the following:
$ make
gcc -Wall -Werror   -c -o dracula.o dracula.c
gcc -Wall -Werror -DI_AM_DRACULA -c player.c -o dracPlayer.o
gcc -Wall -Werror   -c -o DracView.o DracView.c
gcc -Wall -Werror   -c -o GameView.o GameView.c
gcc -Wall -Werror   -c -o Map.o Map.c
gcc -Wall -Werror   -c -o Places.o Places.c
gcc   dracula.o dracPlayer.o DracView.o GameView.o Map.o Places.o   -o dracula
gcc -Wall -Werror   -c -o hunter.o hunter.c
gcc -Wall -Werror -c player.c -o hunterPlayer.o
gcc -Wall -Werror   -c -o HunterView.o HunterView.c
gcc   hunter.o hunterPlayer.o HunterView.o GameView.o Map.o Places.o   -o hunter
Ultimately, it produces two executable files: ./dracula, which contains the Dracula AI; and ./hunter, which contains the the Hunter AI. They both work similarly (not suprising, since they come from the same source code file): they create a View based on a pastPlays string, they then invoke the decide-a-move function, and finally print out the last move and message that that function produced. For example:

$ ./dracula
Move: CD, Message: Mwuhahahaha
$ ./hunter
Move: GE, Message: I'm on holiday in Geneva
Note that we supply our own player.c when we run your AIs in the tournament. It does essentially the same thing as above, but additionally knows how to communicate with the Game Engine, and enforces a time limit on the AI. You can modify the supplied player.c as much as you want. In particular, you could change the pastPlays and messages that are used to build the View to test out how your AI will respond.

**What to Do**
The interface you need to deal with is very simple: there are two functions to decide the next move that a player should make:
* void decideHunterMove(Hunterview gameState)
* void decideDraculaMove(DracView gameState)
along with a function to register potential moves
* void registerBestPlay (char *play, PlayerMessage message)
The registerBestPlay() function is already written for you, so all you need to do is implement the decideXxxMove() functions, along with any auxiliary functions and ADTs that they require. The play is the two-letter code for some place on the Map of Europe (e.g. LO for London); the message can be any text string up to length MESSAGE_SIZE-1 chars.

First, think about a strategy for each kind of player and what info is needed to use that strategy. If that information is already provided by your existing View ADTs, simply use them; otherwise implement new ones. Then, use these View ADTs as a basis for implementing the two decideXxxMove() functions. Finally, submit your work to The Hunt. Repeat the previous two steps until you are satisfied with your AIs' performance.

**The Hunt**
Periodically (at least daily), we will run a batch of Fury of Dracula games, using whatever AIs have been submitted (this is what we call a "Hunt"). These games will be monitored and marks awarded to the participating AIs based on (a) whether they work at all, (b) whether they follow the rules, (c) how well they play the game.

How each session works:
* we copy all submissions from the submissions directory (for give)
* we attempt to compile each submission, using:
  * all of your supplied files
  * our own copies of player.c, dracula.h, hunter.h, Globals.h, Game.h
  * your Makefile (modified by us to have one extra library required by our player.c)
* any ./dracula and ./hunter files are saved
* for each (hunter,dracula) pair do:
  * move the hunter and dracula programs into the "game arena"
  * start the Game Engine, invoke the AIs to determine moves, record game progress
  * when the game ends (see Rules), the game progress is analysed to determine marks
* we update tournament tables that show whose AI is performing best
Note that each game uses one ./hunter from one group to play all four Hunter roles (i.e. the same AI will be used to play Lord Godalming, Dr. Seward, Van Helsing, and Mina Harker). Also, we will ensure that your Hunter AI always plays against a Dracula AI from another group.

Since the marks are based on performance in the Hunts, it's in your best interest to submit working AIs as soon as possible. Even submitting an AI that simply moves to a random reachable location is worth something (not much admittedly, but better than zero, which is what no AI or a non-functioning AI will score). You can submit as many times as you like; the most recent submission will be chosen for each Hunt.
