#include "food.h"

#include <stdlib.h>
#include <time.h>

static int seeded = 0;

void food_seed_random(void) {
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
}

// sorteia a posição da comida dentro do grid, não populando células que a cobra está ocupando; retorna 1 se sucesso, 0 caso contrário
int food_spawn(Food* food, const Snake* snake, int grid_width, int grid_height) {
    if (food == NULL || snake == NULL || grid_width <= 0 || grid_height <= 0) {
        return 0;
    }

    if (snake->length >= grid_width * grid_height) {
        return 0;
    }

    int x;
    int y;
    do {
        x = rand() % grid_width;
        y = rand() % grid_height;
    } while (snake_contains(snake, x, y));

    food->x = x;
    food->y = y;
    return 1;
}

int food_is_at(const Food* food, int x, int y) {
    if (food == NULL) return 0;
    return food->x == x && food->y == y;
}
