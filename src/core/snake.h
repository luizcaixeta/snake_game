#ifndef SNAKE_H
#define SNAKE_H

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

Snake* create_snake(int x, int y);
void grow_snake(Snake* snake);
void move_snake(Snake* snake, int dx, int dy);

#endif
