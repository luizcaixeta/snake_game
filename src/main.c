#include "core/game.h"
#include "platform/input.h"
#include "platform/terminal.h"
#include "platform/timing.h"
#include "ui/render.h"

#include <stdio.h>

int main(void) {
    int exit_code = 0;
    int terminal_ready = 0;
    int input_ready = 0;
    Game* game = NULL;
    TickTimer timer;

    if (!terminal_init()) {
        fprintf(stderr, "failed to initialize terminal\n");
        return 1;
    }
    terminal_ready = 1;

    if (!input_init()) {
        fprintf(stderr, "failed to initialize input\n");
        exit_code = 1;
        goto cleanup;
    }
    input_ready = 1;

    game = game_create();
    if (game == NULL) {
        fprintf(stderr, "failed to create game\n");
        exit_code = 1;
        goto cleanup;
    }

    if (!timing_init(&timer, 120)) {
        fprintf(stderr, "failed to initialize timer\n");
        exit_code = 1;
        goto cleanup;
    }

    render_game(game);

    while (1) {
        InputKey key = input_poll_key();
        if (key == INPUT_KEY_QUIT) break;

        int dx = 0;
        int dy = 0;
        if (input_key_to_direction(key, &dx, &dy)) {
            game_set_direction(game, dx, dy);
        }

        if (!game->game_over) {
            game_tick(game);
        }

        render_game(game);
        timing_sleep_until_next(&timer);
    }

cleanup:
    if (game != NULL) {
        game_destroy(game);
    }
    if (input_ready) {
        input_shutdown();
    }
    if (terminal_ready) {
        terminal_shutdown();
    }
    return exit_code;
}
