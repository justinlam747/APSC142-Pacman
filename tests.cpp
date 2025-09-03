// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <stdio.h>

// make sure not to modify anything in this extern block
extern "C"{
#include "defines.h"
#include "map.h"
#include "actor.h"
#include "game.h"
char *map = NULL, *dot_map = NULL;
int width, height;
}

/**
 * This file is where you should put your tests for your code.
 * Your code must have tests that execute at least 85% of the code in
 * required functions for you to get full marks for the project.
 * Make sure to check out the course videos on automated testing for
 * more information about how to write tests.
 */

/// apsc 142 engineering programming project starter code



// start test suite for map functions
TEST_SUITE_BEGIN("map tests");

TEST_CASE("load map test") {
    // create a temporary file pointer
    FILE *temp_file_ptr;
    // open a file named "test_basic.txt" in write mode
    temp_file_ptr = fopen("test_basic.txt", "w");
    // write map data to the file
    fprintf(temp_file_ptr, "P.W\n"); // row 1
    fprintf(temp_file_ptr, ".GW\n"); // row 2
    fprintf(temp_file_ptr, "..G\n"); // row 3
    // close the file
    fclose(temp_file_ptr);

    // declare variables for map dimensions
    int map_h, map_w;
    // load the map using the function being tested
    char *loaded_data = load_map((char*)"test_basic.txt", &map_h, &map_w);

    // check if the loaded height is correct (should be 3)
    CHECK(map_h == 3);
    // check if the loaded width is correct (should be 3)
    CHECK(map_w == 3);
    // check the content of the first cell (player)
    CHECK(loaded_data[0] == PLAYER);
    // check the content of the second cell (dot)
    CHECK(loaded_data[1] == DOT);
    // check the content of the third cell (wall)
    CHECK(loaded_data[2] == WALL);
    // check the content of the fifth cell (ghost)
    CHECK(loaded_data[4] == GHOST);
    // check the content of the last cell (ghost)
    CHECK(loaded_data[8] == GHOST);

    // free the memory allocated by load_map
    free(loaded_data);
    // delete the temporary file
    remove("test_basic.txt");
}

// test case: load map with different kinds of empty spaces
TEST_CASE("load map with empty spaces") {
    // create a temporary file pointer
    FILE *temp_file_ptr;
    // open a file named "test_spaces.txt" in write mode
    temp_file_ptr = fopen("test_spaces.txt", "w");
    // write map data with spaces
    fprintf(temp_file_ptr, "P W\n"); // row 1 (space is important)
    fprintf(temp_file_ptr, ".G \n"); // row 2 (trailing space)
    // close the file
    fclose(temp_file_ptr);

    // declare variables for map dimensions
    int map_h, map_w;
    // load the map
    char *loaded_data = load_map((char*)"test_spaces.txt", &map_h, &map_w);

    // check if the loaded height is correct (should be 2)
    CHECK(map_h == 2);
    // check if the loaded width is correct (should be 3)
    CHECK(map_w == 3);
    // check the first cell (player)
    CHECK(loaded_data[0] == PLAYER);
    // check the second cell (should be empty space)
    CHECK(loaded_data[1] == EMPTY);
    // check the third cell (wall)
    CHECK(loaded_data[2] == WALL);
    // check the last cell (should be empty space)
    CHECK(loaded_data[5] == EMPTY);

    // free the allocated memory
    free(loaded_data);
    // delete the temporary file
    remove("test_spaces.txt");
}

// test case: attempt to load a file that doesn't exist
TEST_CASE("load map with a non-existent file") {
    // declare variables for map dimensions
    int map_h, map_w;
    // try loading a file that should not exist
    char *loaded_data = load_map((char*)"this_file_should_not_exist.txt", &map_h, &map_w);
    // check that the function returned null, indicating failure
    CHECK(loaded_data == NULL);
    // no memory to free since it should be null
}

// test case: load an empty file
TEST_CASE("load map with a empty file") {
    // create a temporary file pointer
    FILE *temp_file_ptr;
    // open an empty file named "empty.txt" in write mode
    temp_file_ptr = fopen("empty.txt", "w");
    // close the file immediately (it's empty)
    fclose(temp_file_ptr);

    // declare variables for map dimensions
    int map_h, map_w;
    // load the empty file
    char *loaded_data = load_map((char*)"empty.txt", &map_h, &map_w);

    // check that loading an empty file doesn't return null (it should return an empty map representation)
    CHECK(loaded_data != NULL);
    // check that the height is reported as 0
    CHECK(map_h == 0);
    // check that the width is reported as 0
    CHECK(map_w == 0);

    // free the memory if it was allocated (even for an empty map)
    if (loaded_data != NULL) {
        free(loaded_data);
    }
    // delete the temporary file
    remove("empty.txt");
}

// test case: load a map where lines have different lengths
TEST_CASE("load map with varying line lengths") {
    // create a temporary file pointer
    FILE *temp_file_ptr;
    // open a file named "test_varying.txt" in write mode
    temp_file_ptr = fopen("test_varying.txt", "w");
    // write lines of different lengths
    fprintf(temp_file_ptr, "P.G\n");     // length 3
    fprintf(temp_file_ptr, "WG\n");      // length 2
    fprintf(temp_file_ptr, ".GW.P\n");   // length 5
    // close the file
    fclose(temp_file_ptr);

    // declare variables for map dimensions
    int map_h, map_w;
    // load the map with varying line lengths
    char *loaded_data = load_map((char*)"test_varying.txt", &map_h, &map_w);

    // check the height is correct (should be 3 lines)
    CHECK(map_h == 3);
    // check the width is correct (should be the length of the longest line, 5)
    CHECK(map_w == 5);

    // free the allocated memory
    free(loaded_data);
    // delete the temporary file
    remove("test_varying.txt");
}


// test case: check walls inside and outside map bounds
TEST_CASE("is wall testing by checking walls and bounds") {
    // set global map dimensions for the test
    width = 3;
    // set global map height
    height = 3;
    // allocate memory for a temporary map
    map = (char*)malloc(width * height * sizeof(char));
    // fill the temporary map with empty spaces initially
    for (int i = 0; i < width * height; i++) {
        map[i] = EMPTY;
    }
    // place a wall at coordinate (1, 1) -> index 1*3 + 1 = 4
    map[4] = WALL;

    // check if the function correctly identifies the wall at (1, 1)
    CHECK(is_wall(1, 1) == YES_WALL);
    // check if the function correctly identifies an empty space at (0, 0) as not a wall
    CHECK(is_wall(0, 0) == NOT_WALL);
    // check if coordinate (-1, 1) (out of bounds vertically) is treated as a wall
    CHECK(is_wall(-1, 1) == YES_WALL);
    // check if coordinate (1, -1) (out of bounds horizontally) is treated as a wall
    CHECK(is_wall(1, -1) == YES_WALL);
    // check if coordinate (3, 1) (out of bounds vertically) is treated as a wall
    CHECK(is_wall(3, 1) == YES_WALL);
    // check if coordinate (1, 3) (out of bounds horizontally) is treated as a wall
    CHECK(is_wall(1, 3) == YES_WALL);

    // free the memory allocated for the temporary map
    free(map);
    // reset the global map pointer to null
    map = NULL;
}

// end test suite for map functions
TEST_SUITE_END();



// start test suite for actor functions
TEST_SUITE_BEGIN("actor tests");

// test case: ghost and player are on the same spot
TEST_CASE("sees player if they are in the same spot") {
    // set map dimensions
    width = 3;
    height = 3;
    // allocate temporary map (content doesn't matter for this check)
    map = (char*)malloc(width * height * sizeof(char));
    // define player coordinates
    int player_y = 1;
    int player_x = 1;
    // define ghost coordinates (same as player)
    int ghost_y = 1;
    int ghost_x = 1;
    // check if the function returns eating player
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == EATING_PLAYER);
    // free the temporary map memory
    free(map);
    map = NULL;
}

// test case: player is directly right of the ghost, no walls
TEST_CASE("sees player if they are right") {
    // set map dimensions
    width = 5;
    height = 1;
    // create a simple map with no walls between
    char test_map[] = {GHOST, EMPTY, EMPTY, EMPTY, PLAYER};
    // assign the test map to the global pointer
    map = test_map;
    // define player coordinates
    int player_y = 0;
    int player_x = 4;
    // define ghost coordinates
    int ghost_y = 0;
    int ghost_x = 0;
    // check if the function returns the correct direction (right)
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == RIGHT);
    // no need to free map here, it's a stack array
    map = NULL;
}


TEST_CASE("sees player when they are down with no walls") {
    // set map dimensions
    width = 1;
    height = 5;
    // create a simple vertical map
    char test_map[] = {GHOST, EMPTY, EMPTY, EMPTY, PLAYER};
    // assign to global pointer
    map = test_map;
    // define player coordinates
    int player_y = 4;
    int player_x = 0;
    // define ghost coordinates
    int ghost_y = 0;
    int ghost_x = 0;
    // check if the function returns the correct direction (down)
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == DOWN);
    // no need to free map
    map = NULL;
}


TEST_CASE("sees player directly left") {
    // set map dimensions
    width = 5;
    height = 1;
    // create a simple map
    char test_map[] = {PLAYER, EMPTY, EMPTY, EMPTY, GHOST};
    // assign to global pointer
    map = test_map;
    // define player coordinates
    int player_y = 0;
    int player_x = 0;
    // define ghost coordinates
    int ghost_y = 0;
    int ghost_x = 4;
    // check if the function returns the correct direction (left)
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == LEFT);
    // no need to free map
    map = NULL;
}

TEST_CASE("sees player directly up") {
    // set map dimensions
    width = 1;
    height = 5;
    // create a simple vertical map
    char test_map[] = {PLAYER, EMPTY, EMPTY, EMPTY, GHOST};
    // assign to global pointer
    map = test_map;
    // define player coordinates
    int player_y = 0;
    int player_x = 0;
    // define ghost coordinates
    int ghost_y = 4;
    int ghost_x = 0;
    // check if the function returns the correct direction (up)
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == UP);
    // no need to free map
    map = NULL;
}

TEST_CASE("sees player with walls block right") {
    // set map dimensions
    width = 5;
    height = 1;
    // create map with a wall between ghost and player
    char test_map[] = {GHOST, EMPTY, WALL, EMPTY, PLAYER};
    // assign to global pointer
    map = test_map;
    // define player coordinates
    int player_y = 0;
    int player_x = 4;
    // define ghost coordinates
    int ghost_y = 0;
    int ghost_x = 0;
    // check if the function returns sees nothing due to the wall
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == SEES_NOTHING);
    // no need to free map
    map = NULL;
}

// test case: wall blocks vision downwards
TEST_CASE("sees player with walls block down") {
    // set map dimensions
    width = 1;
    height = 5;
    // create vertical map with a wall
    char test_map[] = {GHOST, EMPTY, WALL, EMPTY, PLAYER};
    // assign to global pointer
    map = test_map;
    // define player coordinates
    int player_y = 4;
    int player_x = 0;
    // define ghost coordinates
    int ghost_y = 0;
    int ghost_x = 0;
    // check if the function returns sees nothing
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == SEES_NOTHING);
    // no need to free map
    map = NULL;
}

// test case: player and ghost are not in the same row or column
TEST_CASE("sees player but different row and col") {
    // set map dimensions
    width = 3;
    height = 3;
    // create a map
    char test_map[] = {PLAYER, EMPTY, EMPTY,
                       EMPTY, GHOST, EMPTY,
                       EMPTY, EMPTY, EMPTY};
    // assign to global pointer
    map = test_map;
    // define player coordinates
    int player_y = 0;
    int player_x = 0;
    // define ghost coordinates (diagonal)
    int ghost_y = 1;
    int ghost_x = 1;
    // check if the function returns sees nothing
    CHECK(sees_player(player_y, player_x, ghost_y, ghost_x) == SEES_NOTHING);
    // no need to free map
    map = NULL;
}


// setup map for player movement tests
void setup_player_test_map() {
    // set dimensions
    width = 5;
    height = 5;
    // allocate map memory
    map = (char*)malloc(width * height * sizeof(char));
    // allocate dot map memory
    dot_map = (char*)malloc(width * height * sizeof(char));
    // fill maps with empty initially
    for (int i = 0; i < width * height; i++) {
        map[i] = EMPTY;
        dot_map[i] = EMPTY;
    }
    // place some walls for testing collision
    map[1 * width + 3] = WALL; // wall at (1, 3)
    map[3 * width + 1] = WALL; // wall at (3, 1)
    map[2 * width + 0] = WALL; // wall at (2, 0)
    map[4 * width + 2] = WALL; // wall at (4, 2)
    // place some dots for testing eating
    map[2 * width + 2] = DOT;  // dot at (2, 2)
    dot_map[2 * width + 2] = DOT;
    map[1 * width + 1] = DOT;  // dot at (1, 1)
    dot_map[1 * width + 1] = DOT;
}

// cleanup map after player movement tests
void cleanup_player_test_map() {
    // free map memory if allocated
    if (map) free(map);
    // free dot map memory if allocated
    if (dot_map) free(dot_map);
    // reset pointers
    map = NULL;
    dot_map = NULL;
}

// test case: move player right into empty space
TEST_CASE("move player right") {
    // setup the standard test map
    setup_player_test_map();
    // starting player coordinates
    int player_y = 2;
    int player_x = 1;
    // place player on map
    map[player_y * width + player_x] = PLAYER;
    // attempt to move right
    int result = move_player(&player_y, &player_x, RIGHT);
    // check move was successful
    CHECK(result == MOVED_OKAY);
    // check player's new x coordinate
    CHECK(player_x == 2);
    // check player's y coordinate didn't change
    CHECK(player_y == 2);
    // check player is at the new location on map
    CHECK(map[2 * width + 2] == PLAYER);
    // check the old location is now empty
    CHECK(map[2 * width + 1] == EMPTY);
    // cleanup map memory
    cleanup_player_test_map();
}

// test case: move player left into empty space
TEST_CASE("move player left") {
    // setup map
    setup_player_test_map();
    // starting coordinates
    int player_y = 2;
    int player_x = 2;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move left
    int result = move_player(&player_y, &player_x, LEFT);
    // check success
    CHECK(result == MOVED_OKAY);
    // check new x coordinate
    CHECK(player_x == 1);
    // check y coordinate
    CHECK(player_y == 2);
    // check map update
    CHECK(map[2 * width + 1] == PLAYER);
    // check old position (which was a dot) is now empty in main map
    CHECK(map[2 * width + 2] == EMPTY);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player down into empty space
TEST_CASE("move player down") {
    // setup map
    setup_player_test_map();
    // starting coordinates
    int player_y = 1;
    int player_x = 2;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move down
    int result = move_player(&player_y, &player_x, DOWN);
    // check success
    CHECK(result == MOVED_OKAY);
    // check new y coordinate
    CHECK(player_y == 2);
    // check x coordinate
    CHECK(player_x == 2);
    // check map update
    CHECK(map[2 * width + 2] == PLAYER);
    // check old position
    CHECK(map[1 * width + 2] == EMPTY);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player up into empty space
TEST_CASE("move player up") {
    // setup map
    setup_player_test_map();
    // starting coordinates
    int player_y = 3;
    int player_x = 2;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move up
    int result = move_player(&player_y, &player_x, UP);
    // check success
    CHECK(result == MOVED_OKAY);
    // check new y coordinate
    CHECK(player_y == 2);
    // check x coordinate
    CHECK(player_x == 2);
    // check map update
    CHECK(map[2 * width + 2] == PLAYER);
    // check old position
    CHECK(map[3 * width + 2] == EMPTY);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player right into a wall
TEST_CASE("move player but they hit wall right") {
    // setup map
    setup_player_test_map();
    // starting coordinates (left of wall at (1,3))
    int player_y = 1;
    int player_x = 2;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move right into wall
    int result = move_player(&player_y, &player_x, RIGHT);
    // check move failed due to wall
    CHECK(result == MOVED_WALL);
    // check player's x coordinate didn't change
    CHECK(player_x == 2);
    // check player's y coordinate didn't change
    CHECK(player_y == 1);
    // check player is still at the original location
    CHECK(map[1 * width + 2] == PLAYER);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player left into a wall
TEST_CASE("move player but they hit wall left") {
    // setup map
    setup_player_test_map();
    // starting coordinates (right of wall at (3,1))
    int player_y = 3;
    int player_x = 2;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move left into wall
    int result = move_player(&player_y, &player_x, LEFT);
    // check result
    CHECK(result == MOVED_WALL);
    // check coordinates unchanged
    CHECK(player_x == 2);
    CHECK(player_y == 3);
    // check map unchanged
    CHECK(map[3 * width + 2] == PLAYER);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player down into a wall
TEST_CASE("move player but they hit wall down") {
    // setup map
    setup_player_test_map();
    // starting coordinates (above wall at (4,2))
    int player_y = 3;
    int player_x = 2;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move down into wall
    int result = move_player(&player_y, &player_x, DOWN);
    // check result
    CHECK(result == MOVED_WALL);
    // check coordinates unchanged
    CHECK(player_x == 2);
    CHECK(player_y == 3);
    // check map unchanged
    CHECK(map[3 * width + 2] == PLAYER);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player up into a wall
TEST_CASE("move player but they hit wall up") {
    // setup map
    setup_player_test_map();
    // starting coordinates (below wall at (1,3))
    int player_y = 2;
    int player_x = 3;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move up into wall
    int result = move_player(&player_y, &player_x, UP);
    // check result
    CHECK(result == MOVED_WALL);
    // check coordinates unchanged
    CHECK(player_x == 3);
    CHECK(player_y == 2);
    // check map unchanged
    CHECK(map[2 * width + 3] == PLAYER);
    // cleanup
    cleanup_player_test_map();
}

// test case: try moving player with an invalid direction key
TEST_CASE("move player when its a invalid direction") {
    // setup map
    setup_player_test_map();
    // starting coordinates
    int player_y = 1;
    int player_x = 1;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move with invalid key 'x'
    int result = move_player(&player_y, &player_x, 'x');
    // check result indicates invalid direction
    CHECK(result == MOVED_INVALID_DIRECTION);
    // check coordinates are unchanged
    CHECK(player_x == 1);
    CHECK(player_y == 1);
    // try another invalid key '5'
    result = move_player(&player_y, &player_x, '5');
    // check result
    CHECK(result == MOVED_INVALID_DIRECTION);
    // check coordinates unchanged
    CHECK(player_x == 1);
    CHECK(player_y == 1);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player onto a dot
TEST_CASE("move player over a dot") {
    // setup map
    setup_player_test_map();
    // starting coordinates (next to dot at (2,2))
    int player_y = 2;
    int player_x = 1;
    // place player
    map[player_y * width + player_x] = PLAYER;
    // move right onto dot
    int result = move_player(&player_y, &player_x, RIGHT);
    // check move was okay
    CHECK(result == MOVED_OKAY);
    // check player moved
    CHECK(player_x == 2);
    CHECK(player_y == 2);
    // check player is on the new square in main map
    CHECK(map[2 * width + 2] == PLAYER);
    // check the dot is removed from the dot map
    CHECK(dot_map[2 * width + 2] == EMPTY);
    // cleanup
    cleanup_player_test_map();
}

// test case: move player against edge boundaries (treated as walls)
TEST_CASE("move player at the edge boundary") {
    // setup map (mostly empty)
    width = 3;
    height = 3;
    map = (char*)malloc(width*height*sizeof(char));
    dot_map = (char*)malloc(width*height*sizeof(char));
    for(int i=0; i<width*height; ++i) { map[i]=EMPTY; dot_map[i]=EMPTY; }

    // place player at top-left corner (0,0)
    int player_y = 0;
    int player_x = 0;
    map[player_y * width + player_x] = PLAYER;

    // try moving up (out of bounds)
    CHECK(move_player(&player_y, &player_x, UP) == MOVED_WALL);
    // check coords unchanged
    CHECK(player_y == 0); CHECK(player_x == 0);
    // try moving left (out of bounds)
    CHECK(move_player(&player_y, &player_x, LEFT) == MOVED_WALL);
    // check coords unchanged
    CHECK(player_y == 0); CHECK(player_x == 0);

    // place player at bottom-right corner (2,2)
    player_y = 2;
    player_x = 2;
    map[0] = EMPTY; // clear old spot
    map[player_y * width + player_x] = PLAYER;

    // try moving down (out of bounds)
    CHECK(move_player(&player_y, &player_x, DOWN) == MOVED_WALL);
    // check coords unchanged
    CHECK(player_y == 2); CHECK(player_x == 2);
    // try moving right (out of bounds)
    CHECK(move_player(&player_y, &player_x, RIGHT) == MOVED_WALL);
    // check coords unchanged
    CHECK(player_y == 2); CHECK(player_x == 2);

    // cleanup
    cleanup_player_test_map(); // reuse cleanup function
}



void setup_ghost_test_map() {
    // set dimensions
    width = 5;
    height = 5;
    // allocate memory
    map = (char*)malloc(width * height * sizeof(char));
    dot_map = (char*)malloc(width * height * sizeof(char));
    // fill with empty
    for (int i = 0; i < width * height; i++) {
        map[i] = EMPTY;
        dot_map[i] = EMPTY;
    }
    // place some walls
    map[1 * width + 3] = WALL; // wall at (1, 3)
    map[3 * width + 1] = WALL; // wall at (3, 1)
    map[2 * width + 0] = WALL; // wall at (2, 0)
    map[4 * width + 2] = WALL; // wall at (4, 2)
    // place some dots (ghosts should restore these)
    map[2 * width + 2] = DOT;  // dot at (2, 2)
    dot_map[2 * width + 2] = DOT;
    map[1 * width + 1] = DOT;  // dot at (1, 1)
    dot_map[1 * width + 1] = DOT;
    map[2 * width + 1] = EMPTY; // ensure space next to dot is clear
    dot_map[2 * width + 1] = EMPTY;
}


void cleanup_ghost_test_map() {
    // free map memory
    if (map) free(map);
    // free dot map memory
    if (dot_map) free(dot_map);
    // reset pointers
    map = NULL;
    dot_map = NULL;
}


TEST_CASE("move ghost right") {
    // setup the map
    setup_ghost_test_map();
    // starting ghost coordinates
    int ghost_y = 2;
    int ghost_x = 1;
    // place ghost on map
    map[ghost_y * width + ghost_x] = GHOST;
    // attempt move right
    int result = move_ghost(&ghost_y, &ghost_x, RIGHT);
    // check move was okay
    CHECK(result == MOVED_OKAY);
    // check new x coordinate
    CHECK(ghost_x == 2);
    // check y coordinate unchanged
    CHECK(ghost_y == 2);
    // check ghost is at new location
    CHECK(map[2 * width + 2] == GHOST);
    // check old location is now empty (was originally empty)
    CHECK(map[2 * width + 1] == EMPTY);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost left") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates (on a dot)
    int ghost_y = 2;
    int ghost_x = 2;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move left
    int result = move_ghost(&ghost_y, &ghost_x, LEFT);
    // check okay
    CHECK(result == MOVED_OKAY);
    // check coordinates
    CHECK(ghost_x == 1);
    CHECK(ghost_y == 2);
    // check new location has ghost
    CHECK(map[2 * width + 1] == GHOST);
    // check old location restored the dot
    CHECK(map[2 * width + 2] == DOT);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost down") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates
    int ghost_y = 1;
    int ghost_x = 2;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move down
    int result = move_ghost(&ghost_y, &ghost_x, DOWN);
    // check okay
    CHECK(result == MOVED_OKAY);
    // check coords
    CHECK(ghost_y == 2);
    CHECK(ghost_x == 2);
    // check new location
    CHECK(map[2 * width + 2] == GHOST);
    // check old location
    CHECK(map[1 * width + 2] == EMPTY);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost up") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates
    int ghost_y = 3;
    int ghost_x = 2;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move up
    int result = move_ghost(&ghost_y, &ghost_x, UP);
    // check okay
    CHECK(result == MOVED_OKAY);
    // check coords
    CHECK(ghost_y == 2);
    CHECK(ghost_x == 2);
    // check new location
    CHECK(map[2 * width + 2] == GHOST);
    // check old location
    CHECK(map[3 * width + 2] == EMPTY);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost but hit wall right") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates (left of wall at (1,3))
    int ghost_y = 1;
    int ghost_x = 2;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move right
    int result = move_ghost(&ghost_y, &ghost_x, RIGHT);
    // check wall collision
    CHECK(result == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_x == 2);
    CHECK(ghost_y == 1);
    // check map unchanged
    CHECK(map[1 * width + 2] == GHOST);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost but hit wall left") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates (right of wall at (3,1))
    int ghost_y = 3;
    int ghost_x = 2;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move left
    int result = move_ghost(&ghost_y, &ghost_x, LEFT);
    // check wall collision
    CHECK(result == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_x == 2);
    CHECK(ghost_y == 3);
    // check map unchanged
    CHECK(map[3 * width + 2] == GHOST);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost but hit wall down") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates (above wall at (4,2))
    int ghost_y = 3;
    int ghost_x = 2;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move down
    int result = move_ghost(&ghost_y, &ghost_x, DOWN);
    // check wall collision
    CHECK(result == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_x == 2);
    CHECK(ghost_y == 3);
    // check map unchanged
    CHECK(map[3 * width + 2] == GHOST);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost but hit wall up") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates (below wall at (1,3))
    int ghost_y = 2;
    int ghost_x = 3;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // move up
    int result = move_ghost(&ghost_y, &ghost_x, UP);
    // check wall collision
    CHECK(result == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_x == 3);
    CHECK(ghost_y == 2);
    // check map unchanged
    CHECK(map[2 * width + 3] == GHOST);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost but they are invalid direction") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates
    int ghost_y = 1;
    int ghost_x = 1;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // try invalid key '?'
    int result = move_ghost(&ghost_y, &ghost_x, '?');
    // check result
    CHECK(result == MOVED_INVALID_DIRECTION);
    // check coords unchanged
    CHECK(ghost_x == 1);
    CHECK(ghost_y == 1);
    // try another invalid key ' '
    result = move_ghost(&ghost_y, &ghost_x, ' ');
    // check result
    CHECK(result == MOVED_INVALID_DIRECTION);
    // cleanup
    cleanup_ghost_test_map();
}


TEST_CASE("move ghost over dot") {
    // setup map
    setup_ghost_test_map();
    // starting coordinates (on dot at (1,1))
    int ghost_y = 1;
    int ghost_x = 1;
    // place ghost
    map[ghost_y * width + ghost_x] = GHOST;
    // ensure destination is empty
    map[1 * width + 2] = EMPTY;
    dot_map[1*width+2] = EMPTY;
    // move right
    int result = move_ghost(&ghost_y, &ghost_x, RIGHT);
    // check okay
    CHECK(result == MOVED_OKAY);
    // check coords
    CHECK(ghost_x == 2);
    CHECK(ghost_y == 1);
    // check new location has ghost
    CHECK(map[1 * width + 2] == GHOST);
    // check old location restored the dot from dot_map
    CHECK(map[1 * width + 1] == DOT);
    // cleanup
    cleanup_ghost_test_map();
}

// test case: move ghost against edge boundaries (treated as walls)
TEST_CASE("move ghost at the edge bounds") {
    // setup empty 3x3 map
    width = 3;
    height = 3;
    map = (char*)malloc(width*height*sizeof(char));
    dot_map = (char*)malloc(width*height*sizeof(char));
    for(int i=0; i<width*height; ++i) { map[i]=EMPTY; dot_map[i]=EMPTY; }

    // place ghost at top-left corner (0,0)
    int ghost_y = 0;
    int ghost_x = 0;
    map[ghost_y * width + ghost_x] = GHOST;

    // try moving up
    CHECK(move_ghost(&ghost_y, &ghost_x, UP) == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_y == 0); CHECK(ghost_x == 0);
    // try moving left
    CHECK(move_ghost(&ghost_y, &ghost_x, LEFT) == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_y == 0); CHECK(ghost_x == 0);

    // place ghost at bottom-right corner (2,2)
    ghost_y = 2;
    ghost_x = 2;
    map[0] = EMPTY; // clear old spot
    map[ghost_y * width + ghost_x] = GHOST;

    // try moving down
    CHECK(move_ghost(&ghost_y, &ghost_x, DOWN) == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_y == 2); CHECK(ghost_x == 2);
    // try moving right
    CHECK(move_ghost(&ghost_y, &ghost_x, RIGHT) == MOVED_WALL);
    // check coords unchanged
    CHECK(ghost_y == 2); CHECK(ghost_x == 2);

    // cleanup
    cleanup_ghost_test_map(); // reuse cleanup
}


// end test suite for actor functions
TEST_SUITE_END();


/* tests for game.c */
// start test suite for game logic functions
TEST_SUITE_BEGIN("game tests");

TEST_CASE("check loss player matches ghost") {
    // define player coordinates
    int p_y = 2;
    int p_x = 3;
    // define ghost coordinates array (assuming num_ghosts is 2)
    int g_y[NUM_GHOSTS] = {2, 5}; // first ghost matches player
    int g_x[NUM_GHOSTS] = {3, 1};
    // check if loss condition is met
    CHECK(check_loss(p_y, p_x, g_y, g_x) == YOU_LOSE);
}

TEST_CASE("check loss player matches second ghost") {
    // define player coordinates
    int p_y = 5;
    int p_x = 1;
    // define ghost coordinates array
    int g_y[NUM_GHOSTS] = {2, 5}; // second ghost matches player
    int g_x[NUM_GHOSTS] = {3, 1};
    // check if loss condition is met
    CHECK(check_loss(p_y, p_x, g_y, g_x) == YOU_LOSE);
}

TEST_CASE("check loss player hit with both ghosts") {
    // define player coordinates
    int p_y = 4;
    int p_x = 4;
    // define ghost coordinates array (both match player)
    int g_y[NUM_GHOSTS] = {4, 4};
    int g_x[NUM_GHOSTS] = {4, 4};
    // check if loss condition is met
    CHECK(check_loss(p_y, p_x, g_y, g_x) == YOU_LOSE);
}

TEST_CASE("check loss when player not on ghost") {
    // define player coordinates
    int p_y = 1;
    int p_x = 1;
    // define ghost coordinates array (different from player)
    int g_y[NUM_GHOSTS] = {2, 5};
    int g_x[NUM_GHOSTS] = {3, 1};
    // check if the game should continue (no loss)
    CHECK(check_loss(p_y, p_x, g_y, g_x) == KEEP_GOING);
}


// test case: check win when no dots are left on the dot_map
TEST_CASE("check win if no dots left") {
    // set map dimensions
    width = 3;
    height = 2;
    // allocate dot map memory
    dot_map = (char*)malloc(width * height * sizeof(char));
    // fill dot map with only empty spaces and walls (no dots)
    dot_map[0] = EMPTY; dot_map[1] = EMPTY; dot_map[2] = WALL;
    dot_map[3] = EMPTY; dot_map[4] = EMPTY; dot_map[5] = EMPTY;
    // check if the win condition is met
    CHECK(check_win() == YOU_WIN);
    // free allocated memory
    free(dot_map);
    dot_map = NULL;
}

// test case: check win when there are still dots remaining
TEST_CASE("check win if dots remaining") {
    // set map dimensions
    width = 3;
    height = 2;
    // allocate dot map memory
    dot_map = (char*)malloc(width * height * sizeof(char));
    // fill dot map with some dots
    dot_map[0] = EMPTY; dot_map[1] = DOT;   dot_map[2] = WALL; // one dot here
    dot_map[3] = EMPTY; dot_map[4] = EMPTY; dot_map[5] = DOT;  // another dot here
    // check that the game should continue (no win yet)
    CHECK(check_win() == KEEP_GOING);
    // free allocated memory
    free(dot_map);
    dot_map = NULL;
}

// end test suite for game logic functions
TEST_SUITE_END();