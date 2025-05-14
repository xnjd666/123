/**
 * @file maze.c
 * @author (YOUR NAME)
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze* this, int height, int width)
{
    if (height < MIN_DIM || height > MAX_DIM || width < MIN_DIM || width > MAX_DIM) {
        return 1;
    }

    this->height = height;
    this->width = width;

    this->map = (char**)malloc(height * sizeof(char*));
    if (!this->map) {
        return 1;
    }

    for (int i = 0; i < height; i++) {
        this->map[i] = (char*)malloc(width * sizeof(char));
        if (!this->map[i]) {
            for (int j = 0; j < i; j++) {
                free(this->map[j]);
            }
            free(this->map);
            return 1;
        }
    }
    this->start = (coord){ .x = -1, .y = -1 };
    this->end = (coord){ .x = -1, .y = -1 };


    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze* this)
{
    if (!this) return;

    if (this->map) {
        for (int i = 0; i < this->height; i++) {
            free(this->map[i]);
        }
        free(this->map);
    }
}
/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE* file)
{
    if (!file) return 0;

    char line[MAX_DIM + 2];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }

    int width = strlen(line);
    while (width > 0 && (line[width - 1] == '\n' || line[width - 1] == '\r')) {
        width--;
    }

    rewind(file);

    if (width < MIN_DIM || width > MAX_DIM) {
        return 0;
    }

    return width;
}


/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE* file)
{
    if (!file) return 0;

    char line[MAX_DIM + 2];
    int height = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        height++;
    }

    // Reset file pointer to beginning
    rewind(file);

    if (height < MIN_DIM || height > MAX_DIM) {
        return 0;
    }

    return height;
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze* this, FILE* file)
{
    if (!this || !file) return 1;
    this->height = 0;
    this->width = 0;
    this->map = NULL;
    this->start.x = -1;
    this->start.y = -1;
    this->end.x = -1;
    this->end.y = -1;
    if (this->map != NULL) {
        return 1;
    }

    int width = get_width(file);
    int height = get_height(file);

    if (width == 0 || height == 0) {
        return 1;
    }

    if (create_maze(this, height, width)) {
        return 1;
    }

    int start_count = 0;
    int end_count = 0;
    char line[MAX_DIM + 2];

    for (int i = 0; i < height; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            free_maze(this);
            return 1;
        }

        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            len--;
        }

        if (len != width) {
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

    // Validate start and end points
    if (start_count != 1 || end_count != 1) {
        if (this->start.x < 0 || this->start.x >= this->width ||
            this->start.y < 0 || this->start.y >= this->height ||
            this->end.x < 0 || this->end.x >= this->width ||
            this->end.y < 0 || this->end.y >= this->height) {
            free_maze(this);
            return 1;
        }
    }
    // Check all borders are walls
    for (int i = 0; i < height; i++) {
        if (this->map[i][0] != '#' || this->map[i][width - 1] != '#') {
            free_maze(this);
            return 1;
        }
    }

    for (int j = 0; j < width; j++) {
        if (this->map[0][j] != '#' || this->map[height - 1][j] != '#') {
            free_maze(this);
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze* this, coord* player, char direction)
{
    if (!this || !player) return;

    coord new_pos = *player;

    switch (direction) {
    case 'W':
    case 'w':
        new_pos.y--;
        break;
    case 'A':
    case 'a':
        new_pos.x--;
        break;
    case 'S':
    case 's':
        new_pos.y++;
        break;
    case 'D':
    case 'd':
        new_pos.x++;
        break;
    default:
        printf("Invalid direction. Valid directions are W(up), A(left), S(down), D(right).\n");
        return;
    }

    // Check if move is valid
    if (new_pos.x < 0 || new_pos.x >= this->width ||
        new_pos.y < 0 || new_pos.y >= this->height) {
        printf("Cannot move out of bounds.\n");
        return;
    }

    if (this->map[new_pos.y][new_pos.x] == '#') {
        printf("Cannot move through walls.\n");
        return;
    }

    // Update player position
    *player = new_pos;
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze* this, coord* player)
{
    if (!this || !player) return 0;

    return (player->x == this->end.x && player->y == this->end.y);
}

int main(int argc, char* argv[])
{// Check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mazefile>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }
    coord* player;
    maze* this_maze = malloc(sizeof(maze));

    // Open maze file
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return EXIT_FILE_ERROR;
    }

    // Initialize maze
    if (!this_maze) {
        fclose(f);
        return EXIT_MAZE_ERROR;
    }

    // Read maze from file
    if (read_maze(this_maze, f)) {
        fprintf(stderr, "Error: Invalid maze file %s\n", argv[1]);
        free_maze(this_maze);
        free(this_maze);
        fclose(f);
        return EXIT_MAZE_ERROR;
    }

    fclose(f);

    // Initialize player position
    player = malloc(sizeof(coord));
    if (!player) {
        free_maze(this_maze);
        free(this_maze);
        return EXIT_MAZE_ERROR;
    }

    *player = this_maze->start;

    // Print initial maze
    print_maze(this_maze, player);

    // Game loop
    char input;
    while (1) {
        printf("Enter move (W/A/S/D/Q): ");

        // Read input character
        if (scanf(" %c", &input) != 1) {
            printf("Invalid input. Please enter a single character.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue;
        }

        // Clear input buffer
        while (getchar() != '\n');

        // Process input
        if (input == 'Q' || input == 'q') {
            printf("Quitting game.\n");
            break;
        }

        move(this_maze, player, input);
        print_maze(this_maze, player);

        // Check win condition
        if (has_won(this_maze, player)) {
            printf("Congratulations! You reached the exit!\n");
            break;
        }
    }

    // Cleanup
    free(player);
    free_maze(this_maze);
    free(this_maze);

    return EXIT_SUCCESS;
    // check args

    // set up some useful variables (you can rename or remove these if you want)

    // open and validate mazefile

    // read in mazefile to struct

    // maze game loop

    // win

    // return, free, exit
}