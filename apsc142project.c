// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

// don't forget to update your project configuration to select "Emulate terminal in the output console"

// Make sure to include all relevant libraries
#include <stdio.h>
#include <stdlib.h>

// colours.h contains functions to change text colour and read single characters without requiring an enter
#include "colours.h"
// defines.h contains useful definitions to keep your code readable
#include "defines.h"
// map.h, game.h, and ghost.h contain prototypes of functions you must implement
#include "map.h"
#include "game.h"
#include "actor.h"

// These global variables must be used to store map information.
// Almost every function needs these variables, so keeping them as globals helps keep things organized.
// map is a pointer to a dynamically allocated map for displaying to the user
// dot_map is a pointer to a dynamically allocated map for keeping track of what dots are left
char *map = NULL, *dot_map = NULL;
// width and height store the width and height of map, NOT counting outer walls
int width, height;


/**
 * This is the hardcoded map used for labs 1-3.
 * Once you implement load_map in lab 4 you should remove this map, as it cannot be used for the
 * final version of the project.
 * The map's dimensions are 9x9
 */
#define HARDCODED_WIDTH 9
#define HARDCODED_HEIGHT 9
char hardcoded_map[] = {
        GHOST, DOT, DOT, DOT, WALL, DOT, DOT, DOT, EMPTY,
        DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT,
        DOT, WALL, DOT, DOT, DOT, DOT, DOT, WALL, DOT,
        DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT,
        DOT, DOT, DOT, DOT, PLAYER, DOT, DOT, DOT, DOT,
        DOT, WALL, DOT, WALL, WALL, WALL, DOT, WALL, DOT,
        DOT, WALL, DOT, DOT, DOT, DOT, DOT, WALL, DOT,
        DOT, WALL, WALL, DOT, WALL, DOT, WALL, WALL, DOT,
        EMPTY, DOT, DOT, DOT, WALL, DOT, DOT, DOT, GHOST
};



// You are NOT allowed to add more global variables!
// The globals that are included in the starter code provide a convenient way to share information
// between the many functions that all need access to the same data, but all other data must be
// passed using function arguments.  If you add new globals, the autograder tests will fail to run
// and you will not receive a good mark.


// define a return code for a memory error
#define ERR_MEMORY 4

/**
 * Main entry point into your program.
 * Make sure that main returns appropriate status codes depending on what
 * happens.  The codes you must use are:
 *   NO_ERROR when no error occurs
 *   ERR_NO_MAP when no map file is found
 *   ERR_NO_PLAYER when no player is found on the map
 *   ERR_NO_GHOSTS when fewer than 2 ghosts are found on the map
 *
 * Make sure that any allocated memory is freed before returning.
 * @return a status code
 */




int main(void) {
    // Disable output buffering for immediate display
    setbuf(stdout, NULL);

    // Load map data, updating global width and height
    map = load_map("map.txt", &height, &width);
    // Check if map loading failed
    if (map == NULL) {
        return ERR_NO_MAP;
    }

    // Allocate memory for the map tracking dots
    dot_map = malloc(width * height * sizeof(char));
    // Check if dot map allocation failed
    if (dot_map == NULL) {
        free(map);
        return ERR_MEMORY;
    }

    // Initialize the dot map based on the main map
    int i; // Loop counter variable
    for (i = 0; i < width * height; i++) {
        if (map[i] == DOT) {
            dot_map[i] = DOT;
        } else {
            dot_map[i] = EMPTY;
        }
    }

    int found_ghosts = 0;
    int ghosts_y[NUM_GHOSTS];
    int ghosts_x[NUM_GHOSTS];


    for (i = 0; i < width * height; i++) {
        if (map[i] == GHOST) {
            if (found_ghosts < NUM_GHOSTS) {
                ghosts_y[found_ghosts] = i / width;
                ghosts_x[found_ghosts] = i % width;
                found_ghosts++;
            } else {
                break;
            }
        }
    }


    if (found_ghosts < NUM_GHOSTS) {
        free(map);
        free(dot_map);
        return ERR_NO_GHOSTS;
    }

    int player_y = -1;
    int player_x = -1;
    // Search the map for the player character
    for (i = 0; i < width * height; i++) {
        if (map[i] == PLAYER) {
            player_y = i / width;
            player_x = i % width;
            break;
        }
    }

    // Check if the player was found
    if (player_y == -1) {
        free(map);
        free(dot_map);
        return ERR_NO_PLAYER;
    }



    char input = 0; // Variable to store user input


    while (input != EOF) {

        // Display the map
        print_map();

        // Get user input
        input = getch(); // Read a single character

        // Move the player based on input
        move_player(&player_y, &player_x, input); // Update player position

        int ghost_idx; // Loop counter for ghosts
        for (ghost_idx = 0; ghost_idx < NUM_GHOSTS; ghost_idx++) { // Loop through each ghost
            // Check if the ghost sees the player
            char direction = sees_player(player_y, player_x, ghosts_y[ghost_idx], ghosts_x[ghost_idx]);

            // Move ghost based on whether it sees the player
            if (direction == SEES_NOTHING) { // If ghost doesn't see player
                // Move randomly (simple approach)
                // Pick random direction index (0-3)
                int choice = rand() % 4;
                char random_dir;
                if (choice == 0) random_dir = UP;
                else if (choice == 1) random_dir = DOWN;
                else if (choice == 2) random_dir = LEFT;
                else random_dir = RIGHT; // choice == 3
                move_ghost(&ghosts_y[ghost_idx], &ghosts_x[ghost_idx], random_dir);
            } else if (direction != EATING_PLAYER) {
                // Move towards player
                move_ghost(&ghosts_y[ghost_idx], &ghosts_x[ghost_idx], direction);
            }
            // If direction is EATING_PLAYER, no move needed, loss check handles it
        }

        // Check for win condition (all dots eaten)
        if (check_win() == YOU_WIN) {
            print_map(); // Show final map
            printf("Congratulations! You win!"); // Win message
            break; // Exit game loop
        }

        // Check for loss condition (player caught by ghost)
        if (check_loss(player_y, player_x, ghosts_y, ghosts_x) == YOU_LOSE) {
            print_map(); // Show final map
            printf("Sorry, you lose."); // Loss message
            break; // Exit game loop
        }
    }
    // Free the main map memory
    free(map);
    // Free the dot map memory
    free(dot_map);

    // Set pointers to null
    map = NULL;
    dot_map = NULL;


    return NO_ERROR;
}

