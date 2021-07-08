// dracula.h
// Interface to your "Fury of Dracula" Dracula AI
// By: TeamDracula
// Date: 1 January 2012
// Version: 1.0

void decideDraculaMove(DracView gameState);

//Snookerer:
    //If snooker neighbour in trail and CD is safe
    //path = findSnooker 
    //if current path matches start of path and path is dist-safe, position = path[0]
    //IE. MOVE TO SAME CONNECTION IF C==N, MOVE TO CONNECTION, MOVE TO LOCATION IF NO OTHER CONNECTION ADJACENT

//Avoiderer:
    //Fill min dist map from any hunter (discounting weak ones if blood points available)
    //Choose maximum dist

    //Store hunter locations
    //LocationID hunterLocations[NUM_PLAYERS-1];
    //or(int i=0; i<NUM_PLAYERS-1; i++) hunterLocations[i] = whereIs(g, i);


//TRYING TO MOVE TO CG BECAUSE FINDPATH RETURNS -1 

//find highest weighted LocationID
/*
            int furthestLocation = 0;
            int furthestDist = dist[0];
            for(int i=1; i<NUM_MAP_LOCATIONS; i++) {
                if(dist[i] >= furthestDist) {
                    furthestDist = dist[i];
                    furthestLocation = i;
                }
            }
*/
