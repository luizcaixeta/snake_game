#include "collision.h"
#include <stddef.h>

int check_collision(Snake* snake) {
    // verifica se a cabeça da cobra colidiu com o corpo da cobra: return 1;
    // sem colisão: return 0;
    if (snake == NULL || snake->head == NULL) return 0;

    int head_x = snake->head->x;
    int head_y = snake->head->y;

    Segment* current = snake->head->next;
    while (current != NULL) {
        if (current->x == head_x && current->y == head_y) {
            return 1;
        }
    }

    return 0;
}
