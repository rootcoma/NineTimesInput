#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <Windows.h>

#define BUF_LEN 24

// buttons
#define BUTTON_LEFT     0x0
#define BUTTON_RIGHT    0x1
#define BUTTON_MIDDLE   0x2
// x button closest to user
#define BUTTON_X1       0x3
// x button closest to monitor
#define BUTTON_X2       0x4

// Click types
#define CLICK_DOWN      0x0
#define CLICK_UP        0x1
#define CLICK_DOUBLE    0x2

// Return enums for hooks
#define KEY_NORMAL      0x0
#define KEY_IGNORE      0x1


// return 1 to ignore click or 0 to pass the event on
int register_mouse_click(MSLLHOOKSTRUCT *mouse_event_data, int button,
        int click_type);


// return 1 to ignore scroll or 0 to pass the event on
int register_mouse_scroll(MSLLHOOKSTRUCT *mouse_event_data);


void register_mouse_move(MSLLHOOKSTRUCT *mouse_event_data);


// return 1 to ignore key or 0 to pass the event on
int register_key_stroke(KBDLLHOOKSTRUCT *keyboard_event_data, int is_key_down,
        int is_alt_down);


// Get 1 line of formatted output for stdout that represents input
char *dequeue_output();


#endif

