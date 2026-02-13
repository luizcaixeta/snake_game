#ifndef TERMINAL_H
#define TERMINAL_H

int terminal_init(void);
void terminal_shutdown(void);

int terminal_clear_screen(void);
int terminal_move_cursor_home(void);
int terminal_hide_cursor(void);
int terminal_show_cursor(void);

#endif
