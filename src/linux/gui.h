#ifndef KXM_GUI_H
#define KXM_GUI_H

void gui_create_window();

void gui_cleanup();

void gui_setup();

void gui_event_loop();

void gui_show_window();

void gui_hide_window();

void keyboard_press(const unsigned char key_code, const int down);

void mouse_move_relative(const int dx, const int dy);

void mouse_move_to(const int x, const int y);

void mouse_scroll(const int down);

void mouse_click(const int button, const int down);

void unhook_keyboard();

void unhook_mouse();

int gui_is_focused();

int gui_lock_mouse();

void gui_unlock_mouse();

void gui_get_window_center(int *window_x, int *window_y);

#endif
