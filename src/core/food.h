#ifndef FOOD_H
#define FOOD_H

#include "snake.h"

typedef struct Food {
    int x;
    int y;
} Food;

void food_seed_random(void);
int food_spawn(Food* food, const Snake* snake, int grid_width, int grid_height);
int food_is_at(const Food* food, int x, int y);

#endif
