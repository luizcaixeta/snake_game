#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "snake.h"
#include "food.h"

typedef struct Game {
    Snake* snake;
    Food food;
    int dir_x;
    int dir_y;
    int score;
    int game_over;
    int victory;
} Game; 

Game* game_create(void);
void game_destroy(Game* game);

void game_set_direction(Game* game, int dx, int dy);
void game_tick(Game* game);

int game_snake_contains(const Game* game, int x, int y);

#endif
