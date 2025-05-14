#include "maze.h"
int main(int argc, char* argv[]) {
    const char* filename = argv[1];
        Maze* maze = load_maze(filename);
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
            if (players_move(Maze* maze, char move_input)) {
                if (check_position(Maze* maze)) {
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