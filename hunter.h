// hunter.h
// Interface to your "Fury of Dracula" hunter AI
// By: TeamDracula
// Date: 1 January 2012
// Version: 1.0

void decideHunterMove(HunterView gameState);



//KW Plan:
//1. Determine who is in range of castle and select hunter 
    //with lowest health > (willDracDropTrap*2 + dracDistance*4) to be patrol
    //I.E hunter with lowest health still able to withstand immediate Drac encounter

    //Basic uncommunicative patrol hunter behavior (1/4):
        //1. If Dracula encountered, fight and follow (he is probably trying to recover from BIG fight lol)
        //2. Else if health below max or possibilities UNFOCUSED rest, otherwise random move 
        //always stay within Dracula's minimum distance from castle (when next teleport is possible or how physically near)
    //Basic uncommunicative search hunter behavior (3/4):
        //1. Determine dracula's last definite location 
        //2. Determine search tree of possibilities //DO THIS FOR PATROL/SEARCH ANYWAY
            //3. Move towards "depth" of current round avoiding other hunters if possibilities FOCUSED
            //4. Rest if possibilities UNFOCUSED
        //5. if exact location determined && ENOUGH hunters in range, attack

        /*

static LocationID guessTrail(HunterView gameState, Set bestBranch);
static void matchTypeSequence(HunterView gameState, Set branch, Set bestBranch, int *bestLeaf, Set leads, int src, int branchLeads, int maxBranchLeads, int *matches, int *onLand, int numMoves);


static LocationID guessTrail(HunterView gameState, Set bestBranch) {
    LocationID leadPos = UNKNOWN_LOCATION;
    int leadRound = giveMeTheRound(gameState)-1;
    
    //Determine latest known position and queue types from end to start
    Stack types = newStack();
    
    while(leadRound>=0 && ((leadPos = whereWas(gameState, leadRound, PLAYER_DRACULA, TRUE)) > ZURICH)) {
        pushOnto(types, leadPos);
        int moveType = whereWas(gameState, leadRound, PLAYER_DRACULA, FALSE);
        if(moveType >= DOUBLE_BACK_1 && moveType <= DOUBLE_BACK_5) {
            leadRound -= ((moveType - DOUBLE_BACK_1) + 1);
        } else if (moveType == HIDE) {
            leadRound -= 1;
        }
        leadRound--;
    }
    
    //If we found lead location that will produce useful results
    if(leadPos >=0 && leadPos <= ZURICH && giveMeTheRound(gameState)-leadRound <= MAX_DEPTH && leadRound >= 0) {
        return leadPos;
        //Insert type of real location we found
        pushOnto(types, idToType(leadPos));
        
        //Create set to store known trail locations
        Set leadTrail = newSet();

        //Find traps found after leadRound+(TRAIL_SIZE-1) that will help limit search (range is not saved but should be)
        for(int i=giveMeTheRound(gameState); i >= leadRound + (TRAIL_SIZE-1); i++) {
            for(int k=0; k<NUM_PLAYERS-1; k++) {
                if(i < (giveMeTheRound(gameState) - (whoAmI(gameState) <= k)) && didFindEncounter(gameState, i, k, 'T')) {
                    SetInsert(leadTrail, whereWas(gameState, i, k, TRUE));
                }
            }
        }

        //Create search set starting at leadPos
        Set startBranch = newSet();
        SetInsert(startBranch, leadPos);

        //Recursively determine best set, discarding sets along way
        int matches = 0;
        int bestLeaf = 0;
        printf("Calculating draculas position from %d\n", leadPos);
        matchTypeSequence(gameState, startBranch, bestBranch, &bestLeaf, leadTrail, leadPos, 0, 0, &matches, stacktoa(types), stackCard(types));
        dropStack(types);
        dropSet(bestBranch);
        return bestLeaf;
    } else {
        dropStack(types);
        return UNKNOWN_LOCATION;
    }
    
}

//Recursively fills bestBranch trail, returning total matches
static void matchTypeSequence(HunterView gameState, Set branch, Set bestBranch, int *bestLeaf, Set leads, int src, int branchLeads, int maxBranchLeads, int *matches, int *onLand, int numMoves) {
    int travelledByBoat = ((onLand[0] != onLand[1]) || (onLand[0] == SEA_UNKNOWN));
    int numConnections = 0;
    LocationID *possibleNext = whereCouldTheyGoFrom(gameState, &numConnections, src, PLAYER_DRACULA, giveMeTheRound(gameState) - numMoves, !travelledByBoat, FALSE, travelledByBoat);
    
    //For each connection check all sub-branches
    for(int i=0; i<numConnections; i++) {
        if(idToType(possibleNext[i]) == onLand[1] && !SetMember(branch, possibleNext[i])) {
            //Connection is of correct type and is not countained in branch
            Set newBranch = SetCopy(branch);
            SetInsert(newBranch, possibleNext[i]);
            branchLeads += SetMember(leads, possibleNext[i]);
            if(!numMoves) {
                //Found valid branch so if it is most valid, save it to bestBranchBranch
                (*matches)++;
                if(branchLeads > maxBranchLeads || !maxBranchLeads) {
                    maxBranchLeads = branchLeads;
                    bestBranch = newBranch;
                    *bestLeaf = possibleNext[i];
                } else {
                    dropSet(newBranch);
                }
            } else {
                matchTypeSequence(gameState, newBranch, bestBranch, bestLeaf, leads, possibleNext[i], branchLeads, maxBranchLeads, matches, onLand+1, numMoves-1);
            }
        }
    }
    free(possibleNext);
    dropSet(branch);
}*/
