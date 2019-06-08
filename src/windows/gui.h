#ifndef GUI_H
#define GUI_H

const HWND gui_get_window_handle();

void gui_show_window();

void gui_hide_window();

void gui_cleanup();

void gui_initialize_window();

int gui_is_focused();

void gui_get_window_center(int *window_x, int *window_y);

int gui_lock_mouse();

void gui_unlock_mouse();

void gui_setup();

#endif
