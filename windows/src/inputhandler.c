#include <stdio.h>
#include <assert.h>
#include "mousehook.h"
#include "inputhandler.h"
#include "gui.h"
#include "keydefs.h"

#define MOUSE_MOVE_MIN_MS 10
#define MAX_DOWN_KEYS 42
#define QUEUE_LEN 64
#define NUM_MOUSE_BUTTONS 5


BOOL is_offscreen = FALSE; // is input sent to remote server
BOOL is_paused = FALSE; // did the user pause
BOOL is_input_locked = FALSE; // holding input captive
ULONGLONG last_mouse_move_millis = 0; // Last mouse move event in ms
UINT_PTR mouse_move_timer = 0; // Win32 Timer callback id
LONG mouse_move_buff[2] = { 0, 0, }; // accumulates mouse moves
unsigned char down_keys[MAX_DOWN_KEYS] = { 0 };
BOOL down_mouse_buttons[NUM_MOUSE_BUTTONS] = { 0 }; // 1 = pressed

char output_queue[QUEUE_LEN][BUF_LEN] = {{ 0 }};
int output_queue_offset = 0;
int output_dequeue_offset = 0;

int window_center_y; // storage for centerY of this ConsoleWindow when queried
int window_center_x; // same and for center X
                     // TODO: Is this a handle that needs to be closed?


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


void add_to_queuef(char *output_fmt, ...)
{
    if (output_queue_offset >= QUEUE_LEN) {
        output_queue_offset = 0;
    }
    char *output = output_queue[output_queue_offset++];
    va_list args_list;
    va_start(args_list, output_fmt);
    vsprintf_s(output, BUF_LEN, output_fmt, args_list);
    va_end(args_list);
}


void add_to_queue(char *output_item)
{
    if (output_queue_offset >= QUEUE_LEN) {
        output_queue_offset = 0;
    }
    strncpy_s(output_queue[output_queue_offset++], BUF_LEN-1,
            output_item, BUF_LEN-1);
}


void key_set_erase(const unsigned char vk_code)
{
    int i = 0;
    for (i = 0; i < MAX_DOWN_KEYS; i++) {
        if (down_keys[i] == vk_code) {
            down_keys[i] = 0; // Mark for re-use
            return;
        }
    }
}


void key_set_add(const unsigned char vk_code)
{
    int i = 0;
    int free_i = -1;
    for (i = 0; i < MAX_DOWN_KEYS; i++) {
        if (down_keys[i] == vk_code) {
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
    down_keys[free_i] = vk_code;
}


void center_locked_mouse()
{
    if (!is_offscreen || !is_input_locked) {
        fprintf(stderr,
                "[-] Attempted to reset mouse when input is not locked.\n");
        return;
    }
    SetCursorPos(window_center_x, window_center_y);
    //fprintf(stderr, "[*] Reset mouse position\n");
}


void lock_input()
{
    RECT rect = { 0, 0, 0, 0 };

    gui_show_window();
    HWND base_window_handle = gui_get_window_handle();
    SetForegroundWindow(base_window_handle);
    SetActiveWindow(base_window_handle);
    SetFocus(base_window_handle);
    if(!GetWindowRect(base_window_handle, &rect)) {
        fprintf(stderr, "[-] GetWindowRect base_window_handle. error: 0x%lx\n",
                GetLastError());
    }

    window_center_y = rect.bottom/2 + rect.top/2;
    window_center_x = rect.right/2 + rect.left/2;

    rect.top = window_center_y;
    rect.bottom = window_center_y + 1; // +1 works well
    rect.left = window_center_x;
    rect.right = window_center_x + 1;
    ClipCursor(&rect);

    // Center mouse
    center_locked_mouse();

    is_input_locked = TRUE;
    //fprintf(stderr, "[*] lockInput complete\n");
}


// If the program loses focus or is about to pause, find any
// TODO: rename it includes mouse
void flush_down_keys()
{
    // resolve issues when switching offscreen with keys is_key_down
    // Send all the is_key_down keys back up, don't leave any keys pressed
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


void unlock_input()
{
    ClipCursor(NULL); // Win32 call, passing NULL unlocks the mouse bounds
    gui_hide_window();
    flush_down_keys();
    is_input_locked = FALSE;
    //fprintf(stderr, "[*] unlockInput complete\n");
}


int is_window_focused()
{
    if (is_paused) {
        is_offscreen = FALSE;
        return FALSE;
    }

    if (gui_get_window_handle() == GetForegroundWindow()) {
        is_offscreen = TRUE;
    } else {
        is_offscreen = FALSE;
    }
    return is_offscreen;
    //fprintf(stderr, "[*] isWindowFocused() -> %d\n", offscreen);
}


void CALLBACK mouse_move_timer_cb(HWND arg1, UINT arg2, UINT_PTR arg3,
        DWORD arg4)
{
    //fprintf(stderr, "[*] mouseCallback called\n");
    KillTimer(NULL, mouse_move_timer);
    mouse_move_timer = 0;
    register_mouse_move(NULL);
}


void register_mouse_move(MSLLHOOKSTRUCT *mouse_event_data)
{
    // Mouse button and keyboard key presses re-evaluate if this
    // program is in focused and updates offscreen to true if so
    if (!is_offscreen) {
        // mouse_move_buff[0] = 0;
        // mouse_move_buff[1] = 0;
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
    if (mouse_event_data != NULL &&
            (current_millis - last_mouse_move_millis) < MOUSE_MOVE_MIN_MS) {
        mouse_move_buff[0] += mouse_event_data->pt.x - window_center_x;
        mouse_move_buff[1] += mouse_event_data->pt.y - window_center_y;

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
    if (mouse_event_data != NULL) {
        mouse_move_buff[0] += mouse_event_data->pt.x - window_center_x;
        mouse_move_buff[1] += mouse_event_data->pt.y - window_center_y;
    }

    // output mouse movement accumulation
    add_to_queuef(".%ld,%ld\n", mouse_move_buff[0], mouse_move_buff[1]);

    // Reset state information for mouse movement buffering
    mouse_move_buff[0] = 0;
    mouse_move_buff[1] = 0;
    last_mouse_move_millis = current_millis;
    if (mouse_move_timer) {
        KillTimer(NULL, mouse_move_timer);
        mouse_move_timer = 0;
    }

    // center the mouse on the screen
    is_window_focused();
    center_locked_mouse();
}


int register_mouse_click(MSLLHOOKSTRUCT *mouse_event_data, const int button,
        const int is_up)
{
    // TODO: detect if click is on our window?
    is_window_focused();
    if (!is_offscreen) {
        return KEY_NORMAL;
    }
    add_to_queuef("*%d,%d\n", button, is_up);
    down_mouse_buttons[button] = is_up ? 0 : 1; // stored opposite

    return KEY_IGNORE; // ignore all input when sending to remote client
}


// Returning 0 will cause the next keyboard hook to be called 1 will skip it
int register_key_stroke(KBDLLHOOKSTRUCT *keyboard_event_data, int is_key_down,
        int is_alt_down)
{
    // Handle pausing/unpausing
    if (is_paused && is_key_down && keyboard_event_data->vkCode == VK_PAUSE) {
        is_paused = FALSE;
        lock_input();
        is_window_focused();
        fprintf(stderr, "[+] Unpaused remote input lock\n");
        return KEY_IGNORE;
    } else if (!is_paused && is_key_down &&
            keyboard_event_data->vkCode == VK_PAUSE) {
        is_offscreen = FALSE;
        is_paused = TRUE;
        unlock_input();
        fprintf(stderr, "[+] Paused remote input lock\n");
        return KEY_IGNORE;
    } else if (keyboard_event_data->vkCode == VK_PAUSE) {
        return KEY_IGNORE; // Skip sending keyup on resume..
    }

    // Re-check if remote input is enabled
    is_window_focused();
    if (!is_offscreen) {
        return KEY_NORMAL;
    }

    unsigned char keycode =\
            windows_vk_to_linux_keycode(keyboard_event_data->vkCode);

    //fprintf(stderr, "Windows: %lu, Linux: %u\n",
    //        keyboard_event_data->vkCode, keycode);

    if (is_key_down) {
        add_to_queuef(",v%u\n", keycode);
        key_set_add(keycode);
    } else {
        add_to_queuef(",^%u\n", keycode);
        key_set_erase(keycode);
    }

    return KEY_IGNORE;
}


int register_mouse_scroll(MSLLHOOKSTRUCT *mouse_event_data)
{
    if (!is_offscreen) {
        return KEY_NORMAL;
    }

    // Spec says high order word is equal to +- Wheel delta
    // WORD type is unsigned so checking for positive value
    // is easiest, negative value will be a large value.
    if ((mouse_event_data->mouseData >> 16) == WHEEL_DELTA) {
        add_to_queue("|^\n");
    } else {
        add_to_queue("|v\n");
    }

    return KEY_IGNORE;
}

