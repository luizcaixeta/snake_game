#include "game.h"
#include "collision.h"

#include <stdlib.h>

// aloca e inicializa o estado do jogo 
Game* game_create(void) {
    food_seed_random();

    Game* game = malloc(sizeof(Game));
    if (game == NULL) return NULL;

    game->snake = create_snake(GRID_WIDTH / 2, GRID_HEIGHT / 2);
    if (game->snake == NULL) {
        free(game);
        return NULL;
    }

    // começa com três segmentos
    grow_snake(game->snake);
    grow_snake(game->snake);

    game->dir_x = 1;
    game->dir_y = 0;
    game->score = 0;
    game->game_over = 0;
    game->victory = 0;

    if (!food_spawn(&game->food, game->snake, GRID_WIDTH, GRID_HEIGHT)) {
        game->victory = 1;
        game->game_over = 1;
    }
    return game;
}

// libera memória da cobra e do game
void game_destroy(Game* game) {
    if (game == NULL) return;

    if (game->snake != NULL) {
        Segment* current = game->snake->head;
        while (current != NULL) {
            Segment* next = current->next;
            free(current);
            current = next;
        }
        free(game->snake);
    }
    free(game);
}

// executa um passo do jogo (mover, checar colisão, comer, crescer, pontuar, verificar vitória ou derrota)
void game_tick(Game* game) {
    if (game == NULL || game->game_over) return;

    move_snake(game->snake, game->dir_x, game->dir_y);

    if (check_collision(game->snake)) {
        game->game_over = 1;
        return;
    }

    if (food_is_at(&game->food, game->snake->head->x, game->snake->head->y)) {
        grow_snake(game->snake);
        game->score++;

        if (!food_spawn(&game->food, game->snake, GRID_WIDTH, GRID_HEIGHT)) {
            game->victory = 1;
            game->game_over = 1;
        }
    }
}

//atualiza direção e bloqueia reversão imediata
void game_set_direction(Game* game, int dx, int dy) {
    if (game == NULL || game->game_over) return;

    if (!((dx == 1 && dy == 0) || (dx == -1 && dy == 0) ||
        (dx == 0 && dy == 1) || (dx == 0 && dy == -1))) {
        return;
    }

    if (game->snake != NULL && game->snake->length > 1 &&
        dx == -game->dir_x && dy == -game->dir_y) {
        return;
    }

    game->dir_x = dx;
    game->dir_y = dy;
}

int game_snake_contains(const Game* game, int x, int y) {
    if (game == NULL || game->snake == NULL) return 0;
    return snake_contains(game->snake, x, y);
}
