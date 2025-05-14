/**
 * @file maze.c
 * @brief Complete implementation of maze game for COMP1921 Assignment 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 100
#define MIN_DIM 5

#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord {
    int x;
    int y;
} coord;

typedef struct __Maze {
    char** map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

// Helper function for safe memory allocation
void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_MAZE_ERROR);
    }
    return ptr;
}

int create_maze(maze* this, int height, int width) {
    if (height < MIN_DIM || height > MAX_DIM || width < MIN_DIM || width > MAX_DIM) {
        return 1;
    }

    this->height = height;
    this->width = width;
    this->map = (char**)safe_malloc(height * sizeof(char*));

    for (int i = 0; i < height; i++) {
        this->map[i] = (char*)safe_malloc(width * sizeof(char));
    }

    return 0;
}

void free_maze(maze* this) {
    if (!this) return;

    if (this->map) {
        for (int i = 0; i < this->height; i++) {
            free(this->map[i]);
        }
        free(this->map);
    }
}

int get_width(FILE* file) {
    char line[MAX_DIM + 2];
    if (!fgets(line, sizeof(line), file)) return 0;

    int width = strlen(line);
    if (line[width - 1] == '\n') width--;

    rewind(file);
    return (width >= MIN_DIM && width <= MAX_DIM) ? width : 0;
}

int get_height(FILE* file) {
    char line[MAX_DIM + 2];
    int height = 0;

    while (fgets(line, sizeof(line), file)) height++;

    rewind(file);
    return (height >= MIN_DIM && height <= MAX_DIM) ? height : 0;
}

int read_maze(maze* this, FILE* file) {
    int width = get_width(file);
    int height = get_height(file);
    if (!width || !height) return 1;

    if (create_maze(this, height, width)) return 1;

    char line[MAX_DIM + 2];
    int start_count = 0, end_count = 0;

    for (int i = 0; i < height; i++) {
        if (!fgets(line, sizeof(line), file)) {
            free_maze(this);
            return 1;
        }

        line[strcspn(line, "\n")] = '\0';
        if ((int)strlen(line) != width) {
            free_maze(this);
            return 1;
        }

        for (int j = 0; j < width; j++) {
            char c = line[j];
            this->map[i][j] = c;

            if (c == 'S') {
                start_count++;
                this->start.x = j;
                this->start.y = i;
            }
            else if (c == 'E') {
                end_count++;
                this->end.x = j;
                this->end.y = i;
            }
            else if (c != '#' && c != ' ') {
                free_maze(this);
                return 1;
            }
        }
    }

    if (start_count != 1 || end_count != 1) {
        free_maze(this);
        return 1;
    }

    return 0;
}

/**
 * @brief Prints the maze with player position
 */
void print_maze(maze* this, coord* player) {
    printf("\n");
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (player->x == j && player->y == i) {
                printf("X");
            }
            else {
                printf("%c", this->map[i][j]);
            }
        }
        printf("\n");
    }
}

void move(maze* this, coord* player, char direction) {
    coord new_pos = *player;

    switch (direction) {
    case 'W': case 'w': new_pos.y--; break;
    case 'A': case 'a': new_pos.x--; break;
    case 'S': case 's': new_pos.y++; break;
    case 'D': case 'd': new_pos.x++; break;
    default:
        printf("Invalid direction. Use W/A/S/D.\n");
        return;
    }

    // Check boundaries and walls
    if (new_pos.x < 0 || new_pos.x >= this->width ||
        new_pos.y < 0 || new_pos.y >= this->height ||
        this->map[new_pos.y][new_pos.x] == '#') {
        printf("Can't move there!\n");
        return;
    }

    *player = new_pos;
}

int has_won(maze* this, coord* player) {
    return (player->x == this->end.x && player->y == this->end.y);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mazefile>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return EXIT_FILE_ERROR;
    }

    maze* this_maze = (maze*)safe_malloc(sizeof(maze));
    if (read_maze(this_maze, f)) {
        fprintf(stderr, "Error: Invalid maze file %s\n", argv[1]);
        fclose(f);
        free_maze(this_maze);
        free(this_maze);
        return EXIT_MAZE_ERROR;
    }
    fclose(f);

    coord* player = (coord*)safe_malloc(sizeof(coord));
    *player = this_maze->start;

    char input;
    while (1) {
        printf("Enter move (W/A/S/D/M/Q): ");
        if (scanf(" %c", &input) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (input == 'Q' || input == 'q') break;
        if (input == 'M' || input == 'm') {
            print_maze(this_maze, player);
            continue;
        }

        move(this_maze, player, input);

        if (has_won(this_maze, player)) {
            print_maze(this_maze, player);
            printf("Congratulations! You won!\n");
            break;
        }
    }

    free(player);
    free_maze(this_maze);
    free(this_maze);

    return EXIT_SUCCESS;
}