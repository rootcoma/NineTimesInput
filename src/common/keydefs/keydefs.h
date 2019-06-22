#ifndef KEY_DEFS_H
#define KEY_DEFS_H

// buttons
#define BUTTON_NONE			0x0
#define BUTTON_LEFT         0x1
#define BUTTON_MIDDLE       0x2
#define BUTTON_RIGHT        0x3
#define BUTTON_SCROLL_UP    0x4
#define BUTTON_SCROLL_DOWN  0x5
#define BUTTON_SCROLL_LEFT  0x6
#define BUTTON_SCROLL_RIGHT 0x7
#define BUTTON_BACK			0x8
#define BUTTON_FORWARD		0x9
// x button closest to user
#define BUTTON_X1           BUTTON_BACK
// x button closest to monitor
#define BUTTON_X2           BUTTON_FORWARD

// Click types
#define CLICK_DOWN      0x1
#define CLICK_UP        0x0

// Return enums for hooks
#define KEY_NORMAL      0x0
#define KEY_IGNORE      0x1

const unsigned char windows_vk_to_linux_keycode(const int vk_code);

const unsigned char linux_keycode_to_windows_vk(const int key_code);

const unsigned char key_name_to_keycode(const char* name);

#endif
