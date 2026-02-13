#include "hud.h"

#include <stdio.h>

void hud_draw(const Game* game) {
    if (game == NULL) {
        printf("Score: 0  Length: 0\n");
        printf("Controls: arrows or WASD | Q quit\n");
        return;
    }

    int length = (game->snake != NULL) ? game->snake->length : 0;
    printf("Score: %d  Length: %d\n", game->score, length);

    if (game->game_over) {
        if (game->victory) {
            printf("Victory! Press Q to quit.\n");
            return;
        }
        printf("Game over! Press Q to quit.\n");
        return;
    }

    printf("Controls: arrows or WASD | Q quit\n");
}
