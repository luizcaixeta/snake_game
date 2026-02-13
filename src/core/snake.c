/*
A cobra começa o jogo com três segmentos: o primeiro sendo a cabeça, o segundo o corpo e o terceiro a cauda, se movendo em um grid bidmensional (x, y);
A cobra cresce uma unidade a cada comida que ela come. O jogo termina quando a cobra colidir com si mesma (derrota) ou ocupar toda a tela (vitória);
A cobra deve se mover automaticamente e o jogador deve controlar a dreção da cobra usando as setas do teclado;
A cobra é uma lista encadeada de segmentos, onde cada segmento tem uma posição (x, y) e uma direção (up, down, left, right);
A cabeça da cobra é o primeiro segmento da lista e a cauda é o último segmento 
A cada movimento da cobra, a poisção de cada segmento é atualizada para a posição do segmento anterior, e a cabeça da cobra é movida para a nova posição, assim:

                        posição atual da cobra: (x, y) -> nova posição da cobra (x + dx, y + dy)

Quando a cobra colidir com a parede ela aparece do outro lado da tela, ou seja, se a cobra colidir com a parede direita ela aparece na parede esquerda;
*/

#include <stdlib.h>
#include "snake.h"
#include "grid.h"

// cria a cobra com 1 segmento inicial na posição (x, y)
Snake* create_snake(int x, int y) {
    Snake* snake = malloc(sizeof(Snake));
    if (!snake) return NULL;

    Segment* segment = malloc(sizeof(Segment));
    if (!segment) {
        free(snake);
        return NULL;
    }

    segment->x = x;
    segment->y = y;
    segment->next = NULL;

    snake->head = segment;
    snake->tail = segment;
    snake->length = 1;

    return snake;
}

// adiciona 1 segmento no fim da cobra (posição atual da cauda)
void grow_snake(Snake* snake) {
    if (snake == NULL || snake->tail == NULL) return;

    Segment* new_segment = malloc(sizeof(Segment));
    if (new_segment == NULL) return;

    // um novo segmento nasce na posição atual da cauda
    new_segment->x = snake->tail->x;
    new_segment->y = snake->tail->y;
    new_segment->next = NULL;

    snake->tail->next = new_segment;
    snake->tail = new_segment;
    snake->length++;
}

// move a cabeça por (dx, dy), o corpo segue a cabeça
void move_snake(Snake* snake, int dx, int dy) {
    if (snake == NULL || snake->head == NULL) return;

    int prev_x = snake->head->x;
    int prev_y = snake->head->y;

    int new_x = prev_x + dx;
    int new_y = prev_y + dy;

    // wrap horizontal
    if (new_x < 0) new_x = GRID_WIDTH - 1;
    else if (new_x >= GRID_WIDTH) new_x = 0;

    // wrap vertical
    if (new_y < 0) new_y = GRID_HEIGHT - 1;
    else if (new_y >= GRID_HEIGHT) new_y = 0;
    
    snake->head->x = new_x;
    snake->head->y = new_y;

    Segment* current = snake->head->next;
    while (current != NULL) {
        int temp_x = current->x;
        int temp_y = current->y;

        current->x = prev_x;
        current->y = prev_y;

        prev_x = temp_x;
        prev_y = temp_y;

        current = current->next;
    }
}

// retorna 1 se algum segmento da cobra está em (x, y), caso contrário retorna 0
int snake_contains(const Snake* snake, int x, int y) {
    const Segment* current = snake ? snake->head : NULL;
    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}
