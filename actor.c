// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "actor.h"
#include "map.h"

#include <stdlib.h>

extern char *map, *dot_map; // global map array
extern int width;
extern int height;


char sees_player(int playerRow, int playerCol, int ghostRow, int ghostCol) {
    // if the ghost is on the same position as the player
    if (playerRow == ghostRow && playerCol == ghostCol) {
        // ghost is eating the player
        return EATING_PLAYER;
    }

    // if the ghost and player are on the same row
    if (playerRow == ghostRow) {
        // this is the smaller column value
        int smallerColumn;
        if (playerCol < ghostCol) {
            smallerColumn = playerCol;
        } else {
            smallerColumn = ghostCol;
        }

        // this is the bigger column value
        int biggerColumn;
        if (playerCol > ghostCol) {
            biggerColumn = playerCol;
        } else {
            biggerColumn = ghostCol;
        }

        // check every column between them
        for (int i = smallerColumn + 1; i < biggerColumn; i++) {
            // calculate the map index
            int mapIndex = playerRow * width + i;

            // if there is a wall in the way
            if (map[mapIndex] == WALL) {
                return SEES_NOTHING;
            }
        }

        // return direction depending on position
        if (playerCol < ghostCol) {
            return LEFT;
        } else {
            return RIGHT;
        }
    }

    // if they are on the same column
    if (playerCol == ghostCol) {
        // find the smaller row
        int smallerRow;
        if (playerRow < ghostRow) {
            smallerRow = playerRow;
        } else {
            smallerRow = ghostRow;
        }

        // find the bigger row
        int biggerRow;
        if (playerRow > ghostRow) {
            biggerRow = playerRow;
        } else {
            biggerRow = ghostRow;
        }

        // check for wall in vertical direction
        for (int i = smallerRow + 1; i < biggerRow; i++) {
            int mapIndex = i * width + playerCol;
            if (map[mapIndex] == WALL) {
                return SEES_NOTHING;
            }
        }

        // return vertical direction
        if (playerRow < ghostRow) {
            return UP;
        } else {
            return DOWN;
        }
    }

    // if not in same row or column
    return SEES_NOTHING;
}



int move_player(int *player_y, int *player_x, char direction) {
    // copy current player location
    int new_y = *player_y;
    int new_x = *player_x;

    // move based on input key
    if (direction == UP) {
        new_y = new_y - 1;
    } else if (direction == DOWN) {
        new_y = new_y + 1;
    } else if (direction == LEFT) {
        new_x = new_x - 1;
    } else if (direction == RIGHT) {
        new_x = new_x + 1;
    } else {
        return MOVED_INVALID_DIRECTION; // key is not valid
    }

    // stop player if it's a wall
    if (is_wall(new_y, new_x)) {
        return MOVED_WALL;
    }

    // figure out where the player was before
    int old_index = (*player_y) * width + (*player_x);

    // figure out where player wants to go
    int new_index = new_y * width + new_x;

    // put back what was under player (like a dot or empty)
    map[old_index] = dot_map[old_index];

    // if the new spot is a dot, remove it from dot map
    if (dot_map[new_index] == DOT) {
        dot_map[new_index] = EMPTY;
    }

    // put player on new spot in the map
    map[new_index] = PLAYER;

    // update player's current coordinates
    *player_y = new_y;
    *player_x = new_x;

    // player moved successfully
    return MOVED_OKAY;
}



int move_ghost(int *ghost_y, int *ghost_x, char direction) {
    // make copies of ghost's current spot
    int new_y = *ghost_y;
    int new_x = *ghost_x;

    // decide where to move
    if (direction == UP) {
        new_y = new_y - 1;
    } else if (direction == DOWN) {
        new_y = new_y + 1;
    } else if (direction == LEFT) {
        new_x = new_x - 1;
    } else if (direction == RIGHT) {
        new_x = new_x + 1;
    } else {
        return MOVED_INVALID_DIRECTION; // bad input key
    }

    // stop if it's a wall
    if (is_wall(new_y, new_x)) {
        return MOVED_WALL;
    }

    // calculate index of ghost's old and new tile
    int old_index = (*ghost_y) * width + (*ghost_x);
    int new_index = new_y * width + new_x;

    // restore the dot (or empty) on the tile ghost is leaving
    map[old_index] = dot_map[old_index];

    // move ghost to new tile
    map[new_index] = GHOST;

    // update ghost's current coordinates
    *ghost_y = new_y;
    *ghost_x = new_x;

    // moved correctly
    return MOVED_OKAY;
}


// this function updates the ghost positions
// each ghost looks to see if it can see pacman
// if it can, it tries to move toward him
// if not, it moves randomly

void update_ghost(int playerRowPosition, int playerColumnPosition, int * ghostRowPositions, int * ghostColumnPositions) {
    // loop through every ghost
    for (int i = 0; i < NUM_GHOSTS; i++) {

        // store ghost's row and column
        int currentGhostRow = ghostRowPositions[i];
        int currentGhostColumn = ghostColumnPositions[i];

        // check what the ghost sees
        char ghostVision = sees_player(playerRowPosition, playerColumnPosition, currentGhostRow, currentGhostColumn);

        // create a variable to store direction
        char chosenDirection;

        // if ghost sees player
        if (ghostVision == UP || ghostVision == DOWN || ghostVision == LEFT || ghostVision == RIGHT) {
            // use that direction
            chosenDirection = ghostVision;
        } else {
            // otherwise choose a random direction from 0 to 3
            int randomNumber = rand() % 4;

            // convert number into a direction character
            if (randomNumber == 0) {
                chosenDirection = UP;
            } else if (randomNumber == 1) {
                chosenDirection = DOWN;
            } else if (randomNumber == 2) {
                chosenDirection = LEFT;
            } else {
                chosenDirection = RIGHT;
            }
        }

        // try to move the ghost using the chosen direction
        // the ghost's row and column will be updated by move_ghost()
        move_ghost(&ghostRowPositions[i], &ghostColumnPositions[i], chosenDirection);
    }
}
