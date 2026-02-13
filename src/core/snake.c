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

#include <stdio.h>
#include <stdlib.h>

typedef struct Segment {
    int x;
    int y;
    struct Segment* next;
} Segment;

typedef struct Snake {
    Segment* head;
    Segment* tail;
    int length;
} Snake;

Snake* create_snake(int x, int y) {

}

void grow_snake(Snake* snake) {
    // configura o crescimento da cobra (x, y) -> (x + dx, x +)
}

void move_snake(Snake* snake, int dx, int dy) {
    // atualiza a posição de cada segmento para a posição do segmento anterior
    // move a cabeça da cobra para a nova posição
}

int check_collision(Snake* snake) {
    // verifica se a cabeça da cobra colidiu com o corpo da cobra: return 1;
    // sem colisão: return 0;
}

