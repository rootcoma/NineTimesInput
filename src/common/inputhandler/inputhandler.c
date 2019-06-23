#ifdef _WIN32
#include <Windows.h>
#include "mousehook.h"
#include "keydefs.h"
#else
#include <stdlib.h>
#include <string.h> // memcpy
#include <stdarg.h> // va_start, vsprintf, va_end, va_list
#endif
#include <stdio.h>
#include "inputhandler.h"
#include "gui.h"
#include "config.h"

#define MOUSE_MOVE_MIN_MS 10
#define MAX_DOWN_KEYS 42
#define QUEUE_LEN 64
#define NUM_MOUSE_BUTTONS 5
#define MOUSE_MOVE_MAX 1024


static int is_offscreen = 0; // is input sent to remote server
static int is_paused = 0; // did the user pause
static int is_input_locked = 0; // holding input captive
static unsigned char down_keys[MAX_DOWN_KEYS] = { 0 };
static int down_mouse_buttons[NUM_MOUSE_BUTTONS] = { 0 }; // 1 = pressed
#ifdef _WIN32
static ULONGLONG last_mouse_move_millis = 0; // Last mouse move event in ms
static UINT_PTR mouse_move_timer = 0; // Win32 Timer callback id
static LONG mouse_move_buff[2] = { 0, 0, }; // accumulates mouse moves
#endif
static char output_queue[QUEUE_LEN][BUF_LEN] = {{ 0 }};
static int output_queue_offset = 0;
static int output_dequeue_offset = 0;

static int window_center_y;
static int window_center_x;
static unsigned char pause_keycode = DEFAULT_PAUSE_KEYCODE;



void inputhandler_setup()
{
    const char *tmp = config_get_value("pausekeycode");
    if (tmp[0] == '\0') {
        return;
    }
    int converted_value = atoi(tmp);
    if (converted_value > 0 && converted_value < 256) {
        pause_keycode = (unsigned char)converted_value;
    }
}


char* dequeue_output()
{
    if (output_dequeue_offset == output_queue_offset) {
        return NULL;
    }
    if (output_dequeue_offset >= QUEUE_LEN) {
        output_dequeue_offset = 0;
    }
    return output_queue[output_dequeue_offset++];
}


static void add_to_queuef(char *output_fmt, ...)
{
    if (output_queue_offset >= QUEUE_LEN) {
        output_queue_offset = 0;
    }
    char *output = output_queue[output_queue_offset++];
    va_list args_list;
    va_start(args_list, output_fmt);
#ifdef _WIN32
    vsprintf_s(output, BUF_LEN, output_fmt, args_list);
#else
    vsprintf(output, output_fmt, args_list);
#endif
    va_end(args_list);
}


static void add_to_queue(char *output_item)
{
    if (output_queue_offset >= QUEUE_LEN) {
        output_queue_offset = 0;
    }
#ifdef _WIN32
    strncpy_s(output_queue[output_queue_offset++], BUF_LEN-1,
            output_item, BUF_LEN-1);
#else
    strncpy(output_queue[output_queue_offset++], output_item, BUF_LEN-1);
#endif
}


static void key_set_erase(const unsigned char keycode)
{
    int i = 0;
    for (i = 0; i < MAX_DOWN_KEYS; i++) {
        if (down_keys[i] == keycode) {
            down_keys[i] = 0; // Mark for re-use
            return;
        }
    }
}


static void key_set_add(const unsigned char keycode)
{
    int i = 0;
    int free_i = -1;
    for (i = 0; i < MAX_DOWN_KEYS; i++) {
        if (down_keys[i] == keycode) {
            return; // Sets don't have duplicates
        }
        if (free_i == -1 && down_keys[i] == 0) {
            free_i = i; // Keep track of a place to add
        }
    }
    if (free_i == -1) {
        fprintf(stderr, "[>] Ran out of downKey buffer space\n");
        return; // Ignore this error, something wrong or user is at fault
    }
    down_keys[free_i] = keycode;
}


static void center_locked_mouse()
{
    if (!is_offscreen || !is_input_locked) {
        fprintf(stderr,
                "[-] Attempted to reset mouse when input is not locked.\n");
        return;
    }
    mouse_move_to(window_center_x, window_center_y);
    //fprintf(stderr, "[*] Reset mouse position\n");
}


static void lock_input()
{
    // RECT rect = { 0, 0, 0, 0 };

    // Show window
    gui_show_window();

    // Focus window
    // ...

    // Calculate window center
    gui_get_window_center(&window_center_x, &window_center_y);

    // Lock mouse in window
    gui_lock_mouse();

    // Center mouse
    center_locked_mouse();

    is_input_locked = 1;
    //fprintf(stderr, "[*] lockInput complete\n");
}


// If the program loses focus or is about to pause, flush down keys/buttons
static void flush_down_keys()
{
    int i = 0;
    int count = 0;
    for (i=0;i<MAX_DOWN_KEYS;i++) {
        if (down_keys[i] == 0) {
            continue;
        }
        add_to_queuef(",^%u\n", down_keys[i]);
        down_keys[i] = 0;
        count++;
    }

    if (count > 0) {
        fprintf(stderr,
                "[>] Sent %d keys that were pressed while locked\n",
                count);
    }

    count = 0;
    for (i=0;i<NUM_MOUSE_BUTTONS;i++) {
        if (down_mouse_buttons[i] == 0) {
            continue;
        }
        add_to_queuef("*i,1\n", BUF_LEN, i);
        count++;
    }

    if (count > 0) {
        fprintf(stderr,
                "[>] Sent %d mouse buttons that were pressed while locked\n",
                count);
    }
}


static void unlock_input()
{
    gui_unlock_mouse();
    //ClipCursor(NULL); // Win32 call, passing NULL unlocks the mouse bounds
    gui_hide_window();
    flush_down_keys();
    is_input_locked = 0;
    //fprintf(stderr, "[*] unlockInput complete\n");
}


int is_input_ignored()
{
    if (is_paused) {
        is_offscreen = 0;
        return 0;
    }

    if (gui_is_focused()) {
        is_offscreen = 1;
    } else {
        is_offscreen = 0;
    }
    return is_offscreen;
    //fprintf(stderr, "[*] isWindowFocused() -> %d\n", offscreen);
}

#ifdef _WIN32
static void CALLBACK mouse_move_timer_cb(HWND arg1, UINT arg2, UINT_PTR arg3,
        DWORD arg4)
{
    //fprintf(stderr, "[*] mouseCallback called\n");
    KillTimer(NULL, mouse_move_timer);
    mouse_move_timer = 0;
    register_mouse_move(0, 0);
}

void register_mouse_move(const int x, const int y)
{
    // Mouse button and keyboard key presses re-evaluate if this
    // program is in focused and updates offscreen to true if so
    if (!is_offscreen) {
        mouse_move_buff[0] = 0;
        mouse_move_buff[1] = 0;
        return;
    }

    // hide/force-focus mouse before entering here
    if (!is_input_locked) {
        mouse_move_buff[0] = 0;
        mouse_move_buff[1] = 0;
        lock_input();
        return;
    }

    ULONGLONG current_millis = GetTickCount64();
    // limit mouse updates to 1 packet per MOUSE_MOVE_MIN_MS milliseconds
    if ((x != 0 || y != 0) &&
            (current_millis - last_mouse_move_millis) < MOUSE_MOVE_MIN_MS) {
        mouse_move_buff[0] += x - window_center_x;
        mouse_move_buff[1] += y - window_center_y;

        // check if a timer has been created to send accumulated mouse moves
        if (!mouse_move_timer) {
            // fprintf(stderr, "[*] Creating timer for mouse move buf %llud\n",
            //     current_millis);
            mouse_move_timer = SetTimer(NULL, 1,
                MOUSE_MOVE_MIN_MS-(current_millis-last_mouse_move_millis),
                mouse_move_timer_cb);
            if (!mouse_move_timer) {
                fprintf(stderr,
                        "[-] Failed to create timer for mouse update\n");
            }
        }

        // Reset mouse to center and return early
        center_locked_mouse();
        return;
    }

    // If this is not being called by `mouse_move_timer_cb` then accumulate
    // mouse movement
    if (x != 0 && y != 0) {
        mouse_move_buff[0] += x - window_center_x;
        mouse_move_buff[1] += y - window_center_y;
    }

    if (mouse_move_buff[0] > MOUSE_MOVE_MAX ||
            mouse_move_buff[1] > MOUSE_MOVE_MAX) {
        fprintf(stderr, "[-] Ignoring large mouse move value(s): %ld, %ld\n",
                mouse_move_buff[0], mouse_move_buff[1]);
    } else {
        // output mouse movement accumulation
        add_to_queuef(".%ld,%ld\n", mouse_move_buff[0], mouse_move_buff[1]);
    }

    // Reset state information for mouse movement buffering
    mouse_move_buff[0] = 0;
    mouse_move_buff[1] = 0;
    last_mouse_move_millis = current_millis;
    if (mouse_move_timer) {
        KillTimer(NULL, mouse_move_timer);
        mouse_move_timer = 0;
    }

    // center the mouse on the screen
    is_input_ignored();
    center_locked_mouse();
}

#else

void register_mouse_move(const int x, const int y)
{
    // center the mouse on the screen
    is_input_ignored();
    // Mouse button and keyboard key presses re-evaluate if this
    // program is in focused and updates offscreen to 1 if so
    if (!is_offscreen) {
        return;
    }

    // hide/force-focus mouse before entering here
    if (!is_input_locked) {
        lock_input();
        return;
    }

    // If this is not being called by `mouse_move_timer_cb` then accumulate
    // mouse movement
    const int dx = x-window_center_x;
    const int dy = y-window_center_y;

    if (dx == 0 && dy == 0) {
        // on linux this happens when warp pointer is called
        // ignoring this achieves queueing user movement exclusively
        return;
    }

    if (dx > MOUSE_MOVE_MAX || dy > MOUSE_MOVE_MAX) {
        fprintf(stderr, "[-] Ignoring large mouse move value(s): %d, %d\n",
                dx, dy);
    } else {
        // output mouse movement accumulation
        add_to_queuef(".%d,%d\n", dx, dy);
    }

    center_locked_mouse();
}

#endif

void register_mouse_click(const int button, const int down)
{
    is_input_ignored();
    if (!is_offscreen) {
        return;
    }
    if (button == 4) {
        // scroll down
        add_to_queue("|v\n");
        return;
    } else if (button == 5) {
        add_to_queue("|^\n");
        return;
    }

    add_to_queuef("*%d,%d\n", button, down);
    down_mouse_buttons[button] = down;

    return;
}


void register_key_stroke(const unsigned char keycode, const int down)
{
    // Handle pausing/unpausing
    if (is_paused && down && keycode == pause_keycode) {
        is_paused = 0;
        lock_input();
        is_input_ignored();
        fprintf(stderr, "[+] Unpaused remote input lock\n");
        return;
    } else if (!is_paused && down && keycode == pause_keycode) {
        is_offscreen = 0;
        is_paused = 1;
        unlock_input();
        fprintf(stderr, "[+] Paused remote input lock\n");
        return;
    } else if (keycode == pause_keycode) {
        return;
    }

    is_input_ignored();
    if (!is_offscreen) {
        return;
    }

    if (down) {
        add_to_queuef(",v%u\n", keycode);
        key_set_add(keycode);
    } else {
        add_to_queuef(",^%u\n", keycode);
        key_set_erase(keycode);
    }
}
