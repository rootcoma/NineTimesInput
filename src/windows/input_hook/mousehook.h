#ifndef MOUSE_HOOK_H
#define MOUSE_HOOK_H

int hook_mouse();

void unhook_mouse();

void mouse_click(const int button, const int is_up);

void mouse_scroll(int up);

void mouse_move_relative(const int dx, const int dy);

void mouse_move_to(const int x, const int y);

#endif
