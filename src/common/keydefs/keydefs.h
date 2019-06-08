#ifndef KEY_DEFS_H
#define KEY_DEFS_H

// buttons
#define BUTTON_LEFT         0x1
#define BUTTON_MIDDLE       0x2
#define BUTTON_RIGHT        0x3
#define BUTTON_SCROLL_DOWN  0x4
#define BUTTON_SCROLL_UP    0x5
#define BUTTON_SCROLL_CLICK 0x6
#define BUTTON_SCROLL_SIDE  0x7
// x button closest to user
#define BUTTON_X1           0x8
// x button closest to monitor
#define BUTTON_X2           0x9

// Click types
#define CLICK_DOWN      0x1
#define CLICK_UP        0x0

// Return enums for hooks
#define KEY_NORMAL      0x0
#define KEY_IGNORE      0x1

const unsigned char windows_vk_to_linux_keycode(const int vk_code);

const unsigned char linux_keycode_to_windows_vk(const int key_code);

#endif
