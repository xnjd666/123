#include "maze.h"

    Maze* load_maze(const char* filename) {
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            perror("This file can not be opened");
            return 2;
    }

    void free_maze(Maze * maze) {
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
    int players_move(Maze* maze,char move_input) {
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

    int main(int argc,char* argv[]) {
        const char* filename = argv[1]
            Maze * maze = load_maze(filename);
        if (maze == NULL) {
            return 2;
        }
        int game_over = 0;
        while (!game_over) {
            char input;
            scanf("%c", &input);
            printf("%c\n", input);
            switch (tolower(input)) {
            case 'w':
            case 'a':
            case 's':
            case 'd':
                if (players_move(Maze * maze, char move_input)) {
                    if (check_position(Maze * maze)) { 
                        game_over = 1;
                        printf("win\n");
                        break;//players reach the end point and win
                    }
                }
            case 'm':
                open_map(maze);
                break;
            case 'q':
                break;//players quit game
            }
            free_maze(maze);
            return 0;
        }
    }