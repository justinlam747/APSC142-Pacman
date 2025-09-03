// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "colours.h"
#include "map.h"

extern char *map, *dot_map;
extern int width, height;

/**
 * You should use this function to print out individual characters
 * in the colours used in the demo.
 * @param c
 */
static void printc(char c) {
    switch (c) {
        case WALL:
            change_text_colour(BLUE);
            break;
        case GHOST:
            change_text_colour(PINK);
            break;
        case PLAYER:
            change_text_colour(YELLOW);
            break;
        case DOT:
            change_text_colour(WHITE);
            break;
        default:
            change_text_colour(WHITE);
    }
    printf("%c", c);
}

void print_map(void) {
    // print the top row of wall
    for (int i = 0; i < width + 2; i++) {
        printc(WALL);
        if (i < width + 1) {
            printf(" ");
        }
    }

    // move to the next line
    printf("\n");

    // go through each row of the map
    for (int rowIndex = 0; rowIndex < height; rowIndex++) {
        // print left wall
        printc(WALL);
        printf(" ");

        // go through each tile in the row
        for (int columnIndex = 0; columnIndex < width; columnIndex++) {
            // find the right index in the 1D array
            int tileIndex = rowIndex * width + columnIndex;

            // print that character in color
            printc(map[tileIndex]);
            printf(" ");
        }

        // print right wall
        printc(WALL);
        printf("\n");
    }

    // print the bottom row of wall
    for (int i = 0; i < width + 2; i++) {
        printc(WALL);
        if (i < width + 1) {
            printf(" ");
        }
    }

    // add final newline
    printf("\n");
}


// check if a specific coordinate has a wall
int is_wall(int y, int x) {
    // check if y is above or below the map
    if (y < 0 || y >= height) {
        return YES_WALL;
    }

    // check if x is left or right outside the map
    if (x < 0 || x >= width) {
        return YES_WALL;
    }

    // figure out the 1D array index from 2D coordinates
    int index = y * width + x;

    // check if the tile is a wall
    if (map[index] == WALL) {
        return YES_WALL;
    }

    // not a wall, so return no wall
    return NOT_WALL;
}




// loads a map from a file and returns a pointer to a dynamically allocated 1D map array
char * load_map(char * fileNameToOpen, int *mapHeightOutput, int *mapWidthOutput) {
    // open the file for reading
    FILE *filePointer = fopen(fileNameToOpen, "r");

    // if file couldn’t be opened, return nothing
    if (filePointer == NULL) {
        return NULL;
    }

    // set width and height to zero
    *mapHeightOutput = 0;
    *mapWidthOutput = 0;

    // line buffer to read one line at a time
    char lineBuffer[256];

    // go through file once to figure out dimensions
    while (fgets(lineBuffer, sizeof(lineBuffer), filePointer)) {
        // increase number of rows for each line
        *mapHeightOutput = *mapHeightOutput + 1;

        // make a temporary width counter
        int currentLineWidth = 0;

        // go through each character in the line
        for (int i = 0; lineBuffer[i] != '\0' && lineBuffer[i] != '\n'; i++) {
            // count characters every third position (separated by two spaces)
            if (i % 3 == 0) {
                currentLineWidth = currentLineWidth + 1;
            }
        }

        // if this line is the longest so far, save its width
        if (currentLineWidth > *mapWidthOutput) {
            *mapWidthOutput = currentLineWidth;
        }
    }

    // calculate total size needed for the map
    int numberOfMapTiles = (*mapHeightOutput) * (*mapWidthOutput);

    // allocate space for the map
    char *newMapMemory = (char*)malloc(sizeof(char) * numberOfMapTiles);

    // if malloc fails, clean up and return
    if (newMapMemory == NULL) {
        fclose(filePointer);
        return NULL;
    }

    // set every tile in the map to empty space
    for (int i = 0; i < numberOfMapTiles; i++) {
        newMapMemory[i] = EMPTY;
    }

    // go back to the beginning of the file
    rewind(filePointer);

    // variable to track what row we’re on
    int currentRowIndex = 0;

    // read the map a second time, now to store it
    while (fgets(lineBuffer, sizeof(lineBuffer), filePointer)) {
        // variable to track what column we’re writing into
        int currentColumnIndex = 0;

        // go through each character in the line again
        for (int i = 0; lineBuffer[i] != '\0' && lineBuffer[i] != '\n'; i++) {
            // every third character is a map tile
            if (i % 3 == 0) {
                // make sure we don’t write past the end of the row
                if (currentColumnIndex < *mapWidthOutput && currentRowIndex < *mapHeightOutput) {
                    int indexInMap = currentRowIndex * (*mapWidthOutput) + currentColumnIndex;
                    newMapMemory[indexInMap] = lineBuffer[i];
                }

                // move to next column
                currentColumnIndex = currentColumnIndex + 1;
            }
        }

        // move to next row
        currentRowIndex = currentRowIndex + 1;
    }

    // close the file after reading
    fclose(filePointer);

    // return the new map we just created
    return newMapMemory;
}
