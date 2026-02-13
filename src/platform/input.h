#ifndef INPUT_H
#define INPUT_H

typedef enum InputKey {
    INPUT_KEY_NONE = 0,
    INPUT_KEY_UP,
    INPUT_KEY_DOWN,
    INPUT_KEY_LEFT,
    INPUT_KEY_RIGHT,
    INPUT_KEY_QUIT
} InputKey;

int input_init(void);
void input_shutdown(void);

InputKey input_poll_key(void);
int input_key_to_direction(InputKey key, int* dx, int* dy);

#endif
