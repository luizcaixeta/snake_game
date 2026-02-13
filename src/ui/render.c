#include "render.h"

#include "../core/grid.h"
#include "../platform/terminal.h"
#include "hud.h"

#include <stdio.h>

static void board_fill(char board[GRID_HEIGHT][GRID_WIDTH], char value) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            board[y][x] = value;
        }
    }
}

static void board_draw_food(char board[GRID_HEIGHT][GRID_WIDTH], const Game* game) {
    if (game == NULL) return;

    if (game->food.x >= 0 && game->food.x < GRID_WIDTH &&
        game->food.y >= 0 && game->food.y < GRID_HEIGHT) {
        board[game->food.y][game->food.x] = 'X';
    }
}

static void board_draw_snake(char board[GRID_HEIGHT][GRID_WIDTH], const Snake* snake) {
    const Segment* current = (snake != NULL) ? snake->head : NULL;
    int index = 0;

    while (current != NULL) {
        if (current->x >= 0 && current->x < GRID_WIDTH &&
            current->y >= 0 && current->y < GRID_HEIGHT) {
            board[current->y][current->x] = (index == 0) ? '@' : 'O';
        }
        current = current->next;
        ++index;
    }
}

static void board_print(const char board[GRID_HEIGHT][GRID_WIDTH]) {
    for (int y = -1; y <= GRID_HEIGHT; ++y) {
        for (int x = -1; x <= GRID_WIDTH; ++x) {
            if (y == -1 || y == GRID_HEIGHT || x == -1 || x == GRID_WIDTH) {
                putchar('*');
            } else {
                putchar(board[y][x]);
            }
        }
        putchar('\n');
    }
}

void render_game(const Game* game) {
    char board[GRID_HEIGHT][GRID_WIDTH];
    board_fill(board, ' ');
    board_draw_food(board, game);
    board_draw_snake(board, (game != NULL) ? game->snake : NULL);

    (void)terminal_move_cursor_home();
    printf("\x1b[J");
    board_print(board);
    hud_draw(game);
    fflush(stdout);
}
