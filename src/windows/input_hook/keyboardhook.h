#ifndef KEYBOARD_HOOK_H
#define KEYBOARD_HOOK_H

int hook_keyboard();

void unhook_keyboard();

void keyboard_press(const int key_code, int is_down);

#endif
