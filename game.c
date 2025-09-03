// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "game.h"

#define NUM_GHOSTS 2

extern char * map, * dot_map;
extern int height;
extern int width;

int check_win(void) {
    // look through every tile
    for (int i = 0; i < width * height; i++) {
        // if there's a dot still on the board
        if (dot_map[i] == DOT) {
            return KEEP_GOING; // not done yet
        }
    }

    // no dots found = game won
    return YOU_WIN;
}



// Checks if the player has been caught by a ghost
/*
*player_y is the player's y coordinates (the height)
* player_x is the player's x coordinates(thewidth)
* ghosts_y is the ghost's y coordinates (the height)
* ghosts_x is the ghost's x coordinates (the width)
* YOU_LOSE 0 is when a ghost catches a player
* KEEP_GOING 1 allows the player to continue playing in the case they are not caught
*/

int check_loss(int playerRow, int playerColumn, int ghostRows[NUM_GHOSTS], int ghostColumns[NUM_GHOSTS]) {
    // go through every ghost
    for (int i = 0; i < NUM_GHOSTS; i++) {
        // get the current ghost's row and column
        int currentGhostRow = ghostRows[i];
        int currentGhostColumn = ghostColumns[i];

        // if ghost is in the same spot as the player
        if (currentGhostRow == playerRow && currentGhostColumn == playerColumn) {
            // the player has been caught
            return YOU_LOSE;
        }
    }

    // if no ghost is on the player, game continues
    return KEEP_GOING;
}


