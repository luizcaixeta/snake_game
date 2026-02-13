#include "terminal.h"

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>

static struct termios original_termios;
static int termios_saved = 0;
static int raw_mode_enabled = 0;

static int write_all(const char* data, size_t len) {
    size_t total_written = 0;
    while (total_written < len) {
        ssize_t written = write(STDOUT_FILENO, data + total_written, len - total_written);
        if (written < 0) {
            if (errno == EINTR) continue;
            return 0;
        }
        total_written += (size_t)written;
    }

    return 1;
}

static int terminal_enable_raw_mode(void) {
    if (raw_mode_enabled) return 1;

    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        return 0;
    }
    termios_saved = 1;

    struct termios raw = original_termios;
    raw.c_iflag &= (tcflag_t)~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag |= (tcflag_t)(CS8);
    raw.c_lflag &= (tcflag_t)~(ECHO | ICANON | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        return 0;
    }

    raw_mode_enabled = 1;
    return 1;
}

static void terminal_disable_raw_mode(void) {
    if (!raw_mode_enabled || !termios_saved) return;

    (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    raw_mode_enabled = 0;
}

int terminal_clear_screen(void) {
    static const char clear_seq[] = "\x1b[2J";
    return write_all(clear_seq, sizeof(clear_seq) - 1U);
}

int terminal_move_cursor_home(void) {
    static const char home_seq[] = "\x1b[H";
    return write_all(home_seq, sizeof(home_seq) - 1U);
}

int terminal_hide_cursor(void) {
    static const char hide_seq[] = "\x1b[?25l";
    return write_all(hide_seq, sizeof(hide_seq) - 1U);
}

int terminal_show_cursor(void) {
    static const char show_seq[] = "\x1b[?25h";
    return write_all(show_seq, sizeof(show_seq) - 1U);
}

int terminal_init(void) {
    if (!terminal_enable_raw_mode()) {
        return 0;
    }

    if (!terminal_hide_cursor()) {
        terminal_disable_raw_mode();
        return 0;
    }

    if (!terminal_clear_screen() || !terminal_move_cursor_home()) {
        (void)terminal_show_cursor();
        terminal_disable_raw_mode();
        return 0;
    }

    return 1;
}

void terminal_shutdown(void) {
    (void)terminal_show_cursor();
    terminal_disable_raw_mode();
}
