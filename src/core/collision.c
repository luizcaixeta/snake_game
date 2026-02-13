#include "collision.h"
#include <stddef.h>

// verifica se a cabeça da cobra colidiu com o corpo da cobra: return 1; caso contrário return 0;
int check_collision(Snake* snake) {
    if (snake == NULL || snake->head == NULL) return 0;

    int head_x = snake->head->x;
    int head_y = snake->head->y;

    Segment* current = snake->head->next;
    while (current != NULL) {
        if (current->x == head_x && current->y == head_y) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}
