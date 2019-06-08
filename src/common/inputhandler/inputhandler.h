#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#define BUF_LEN 24
#define DEFAULT_PAUSE_KEYCODE 127

void inputhandler_setup();

int is_input_ignored();

void register_mouse_click(const int button, const int down);

void register_mouse_move(const int x, const int y);

void register_key_stroke(const unsigned char keycode, const int down);

char *dequeue_output();

#endif
