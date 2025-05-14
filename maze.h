#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char** grid;
    int rows;
    int cols;
    int start_row;
    int start_col;
    int current_row;
    int current_col;
} Maze;

#define MAX_rows 100
#define MIN_rows 5
#define MAX_cols 100
#define MIN_cols 5

Maze* load_maze(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("This file can not be opened");
        return 2;
    }
    fclose(file);
    return 0;
}
    int players_move(Maze * maze, char move_input) {
        int new_row = maze->current_row;
        int new_col = maze->current_col;

        switch (tolower(move_input)) {
        case 'w': new_row--; break;
        case 's': new_row++; break;
        case 'a': new_col--; break;
        case 'd': new_col++; break;
        case 'q': return false; // quit
        default: return true; // Invalid input
        }

        // Check the boundary
        if (new_row < 0 || new_row >= maze->rows || new_col < 0 || new_col >= maze->cols) {
            printf("Cannot move out of the maze\n");
            return true;
        }

        // Check whether it is a wall
        char next_char = maze->grid[new_row][new_col];
        if (next_char == '#') {
            printf("Hit a wall\n");
            return true;
        }

        maze->current_row = new_row;
        maze->current_col = new_col;
        return true;
    }
}
void free_maze(Maze* maze) {
    if (maze == NULL) return;
    if (maze->grid != NULL) {
        for (int i = 0; i < maze->rows; i++) {
            if (maze->grid[i] != NULL) {
                free(maze->grid[i]);
            }
        }
        free(maze->grid);
    }

    free(maze);
}
void print_maze(Maze* maze) {
    printf("\n"); // Start line break
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            if (i == maze->current_row && j == maze->current_col) {
                printf("X");
            }
            else {
                printf("%c", maze->grid[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n"); // End line break
}
int check_position(Maze* maze) {
    if (!maze)return false;
    char current_char = maze->grid[maze->current_row][maze->current_col];
    return(current_char == 'E');
}

