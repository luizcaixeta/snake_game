#include "input.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

enum ParseState {
    PARSE_NORMAL = 0,
    PARSE_ESC,
    PARSE_CSI
};

static int initialized = 0;
static int original_stdin_flags = 0;
static enum ParseState parse_state = PARSE_NORMAL;

static InputKey parse_key(unsigned char byte) {
    if (parse_state == PARSE_NORMAL) {
        switch (byte) {
            case 'w':
            case 'W':
                return INPUT_KEY_UP;
            case 's':
            case 'S':
                return INPUT_KEY_DOWN;
            case 'a':
            case 'A':
                return INPUT_KEY_LEFT;
            case 'd':
            case 'D':
                return INPUT_KEY_RIGHT;
            case 'q':
            case 'Q':
                return INPUT_KEY_QUIT;
            case 27:
                parse_state = PARSE_ESC;
                return INPUT_KEY_NONE;
            default:
                return INPUT_KEY_NONE;
        }
    }

    if (parse_state == PARSE_ESC) {
        parse_state = (byte == '[') ? PARSE_CSI : PARSE_NORMAL;
        return INPUT_KEY_NONE;
    }

    parse_state = PARSE_NORMAL;
    switch (byte) {
        case 'A':
            return INPUT_KEY_UP;
        case 'B':
            return INPUT_KEY_DOWN;
        case 'C':
            return INPUT_KEY_RIGHT;
        case 'D':
            return INPUT_KEY_LEFT;
        default:
            return INPUT_KEY_NONE;
    }
}

int input_init(void) {
    if (initialized) return 1;

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1) return 0;

    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) return 0;

    original_stdin_flags = flags;
    parse_state = PARSE_NORMAL;
    initialized = 1;
    return 1;
}

void input_shutdown(void) {
    if (!initialized) return;

    (void)fcntl(STDIN_FILENO, F_SETFL, original_stdin_flags);
    initialized = 0;
}

InputKey input_poll_key(void) {
    if (!initialized) return INPUT_KEY_NONE;

    InputKey latest_key = INPUT_KEY_NONE;
    unsigned char byte = 0;

    while (1) {
        ssize_t bytes_read = read(STDIN_FILENO, &byte, sizeof(byte));
        if (bytes_read == 1) {
            InputKey parsed = parse_key(byte);
            if (parsed == INPUT_KEY_QUIT) {
                return INPUT_KEY_QUIT;
            }
            if (parsed != INPUT_KEY_NONE) {
                latest_key = parsed;
            }
            continue;
        }

        if (bytes_read == 0) break;

        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
        if (errno == EINTR) continue;
        break;
    }

    return latest_key;
}

int input_key_to_direction(InputKey key, int* dx, int* dy) {
    if (dx == NULL || dy == NULL) return 0;

    switch (key) {
        case INPUT_KEY_UP:
            *dx = 0;
            *dy = -1;
            return 1;
        case INPUT_KEY_DOWN:
            *dx = 0;
            *dy = 1;
            return 1;
        case INPUT_KEY_LEFT:
            *dx = -1;
            *dy = 0;
            return 1;
        case INPUT_KEY_RIGHT:
            *dx = 1;
            *dy = 0;
            return 1;
        default:
            return 0;
    }
}
