#include <stdio.h>
#include <assert.h>
#include "mousehook.h"
#include "inputhandler.h"
#include "gui.h"

#define MAX_DOWN_KEYS 42
#define BUF_LEN 32
#define MIN_MOUSE_TICKS 10
#define QUEUE_LEN 10

// State information
BOOL is_offscreen = FALSE; // is input sent to remote server
BOOL is_paused = FALSE; // did the user pause
BOOL is_input_locked = FALSE; // are we holding input captive
ULONGLONG last_mouse_move_millis = 0;
UINT_PTR mouse_move_timer = 0;
LONG mouse_move_buff[2] = { 0 };

// buffers to avoid allocations
DWORD down_keys[MAX_DOWN_KEYS] = { 0 };
char tmp_buffer[BUF_LEN] = { 0 };

// Output buffer, need to let hooks return
char output_queue[QUEUE_LEN][BUF_LEN] = {{0}};
int output_queue_offset = 0;

// global storage for values to avoid Win32 calls
int window_center_y; // storage for centerY of this ConsoleWindow when queried
int window_center_x; // same and for center X
                    // TODO: Is this a handle that needs to be closed?
HWND window = NULL; // storage for HWND from `GetConsoleWindow`


void add_to_output_queue(char *output_item)
{
    if (output_queue_offset >= QUEUE_LEN) {
        output_queue_offset = 0;
    }
    strncpy_s(output_queue[output_queue_offset++], 31, output_item, 31);
}


// Helper to remove keys that are released
void key_set_erase(const DWORD msg)
{
    int i = 0;
    for (i = 0; i < MAX_DOWN_KEYS; i++) {
        if (down_keys[i] == msg) {
            down_keys[i] = 0; // Mark for re-use
            return;
        }
    }
}


// Helper to keep add keys that are pressed
void key_set_add(const DWORD msg)
{
    int i = 0;
    int free_i = -1;
    for (i = 0; i < MAX_DOWN_KEYS; i++) {
        if (down_keys[i] == msg) {
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
    down_keys[free_i] = msg;
}


// Uses Win32 `SetCursorPos` to re-center mouse
void reset_locked_mouse()
{
    if (!is_offscreen || !is_input_locked) {
        fprintf(stderr,
                "[-] Attempted to reset mouse when input is not locked.\n");
        return;
    }
    SetCursorPos(window_center_x, window_center_y); // Win32 call
    //fprintf(stderr, "[*] Reset mouse position\n");
}


// ConsoleWindow (this program) is set focused
// Win32 `ClipCursor` is called in center of this program's window
// the mouse is moved to the center of this program
// the flag for input being locked is set.
void lock_input()
{
    // TODO: WIP gui for capturing input

    // Query window rect
    RECT rect = { 0, 0, 0, 0 };

#ifdef WITH_GUI
    show_window();
    HWND base_window_handle = get_gui_handle();
    SetForegroundWindow(base_window_handle); // Win32 calls
    SetActiveWindow(base_window_handle);
    SetFocus(base_window_handle);
    if(!GetWindowRect(base_window_handle, &rect)) {
        fprintf(stderr, "[-] Failed accessing base_window_handle rect. error: 0x%lx\n", GetLastError());
    }
#else
    SetForegroundWindow(window); // Win32 calls
    SetActiveWindow(window);
    SetFocus(window);
    GetWindowRect(window, &rect);
#endif

    window_center_y = rect.bottom/2 + rect.top/2;
    window_center_x = rect.right/2 + rect.left/2;

    // Reuse rect to Clip mous
    rect.top = window_center_y;
    rect.bottom = window_center_y + 1; // +1 works well
    rect.left = window_center_x;
    rect.right = window_center_x + 1;
    ClipCursor(&rect); // Win32 call

    // Center mouse
    reset_locked_mouse();

    is_input_locked = TRUE;
    //fprintf(stderr, "[*] lockInput complete\n");
}


// If the program loses focus or is about to pause, find any 
void flush_down_keys()
{
    // resolve issues when switching offscreen with keys is_key_down
    // Send all the is_key_down keys back up, don't leave any keys pressed
    int i = 0;
    int count = 0;
    char key_buffer[BUF_LEN] = {0};
    for (i=0;i<MAX_DOWN_KEYS;i++) {
        if (down_keys[i] == 0) {
            continue;
        }
        if (!GetKeyNameTextA(down_keys[i], key_buffer, BUF_LEN - 1)) {
            fprintf(stderr,
                    "[-] Could not get name of key in unlockInput. msg: %lu\n",
                    down_keys[i]);
            continue;
        }
        sprintf(tmp_buffer, ",^%.*s\n", BUF_LEN, key_buffer);
        add_to_output_queue(tmp_buffer);
        down_keys[i] = 0;
        count++;
    }

    if (count > 0) {
        fprintf(stderr,
                "[>] Sent %d keys that were down before input was unlocked\n",
                count);
    }
}


// Undo anything that locked mouse and keyboard input while sending
// input data to stdout
void unlock_input()
{
    // The rest of unlocking
    ClipCursor(NULL); // Win32 call, passing NULL unlocks the mouse bounds

    hide_window();

    is_input_locked = FALSE;
    //fprintf(stderr, "[*] unlockInput complete\n");
    flush_down_keys();
}


// If the console window is the foreground window then the window is
// in focus. If the "Pause" is not enabled, then inputs will be sent
// to stdout and mouse and keyboard input will be "locked". Pressing
// "Pause" will toggle pause;
int is_window_focused()
{
    if (is_paused) {
        is_offscreen = FALSE;
        return FALSE;
    }
    if (window == NULL) {
        window = GetConsoleWindow();
    }
    // base_window_handle = gui window, window = console window
    HWND base_window_handle = get_gui_handle();
    if (base_window_handle == GetForegroundWindow() || window == GetForegroundWindow()) {
        is_offscreen = TRUE;
    } else {
        is_offscreen = FALSE;
        unlock_input();
    }
    //fprintf(stderr, "[*] isWindowFocused() -> %d\n", offscreen);
    return is_offscreen;
}


// Called by timeout to allow for buffering of mouse movement
void CALLBACK mouse_move_timer_cb(HWND arg1, UINT arg2, UINT_PTR arg3,
        DWORD arg4)
{
    //fprintf(stderr, "[*] mouseCallback called\n");
    KillTimer(NULL, mouse_move_timer);
    mouse_move_timer = 0;
    register_mouse_move(NULL);
}


// Sends relative movement data from mouse to stdout, the mouse is
// reset to the center of the window after reading movement to keep
// the window in focus and track relative movement again.
void register_mouse_move(MSLLHOOKSTRUCT *mouse_event_data)
{
    // fprintf(stderr, "is_offscreen: %d, is_input_locked: %d\n",
    //         is_offscreen, is_input_locked);
    // Mouse button and keyboard key presses re-evaluate if this
    // program is in focus and updates offscreen to true if so
    if (!is_offscreen) {
        mouse_move_buff[0] = 0;
        mouse_move_buff[1] = 0;
        return;
    }

    // hide/force-focus mouse before entering here
    if (!is_input_locked) {
        lock_input();
        mouse_move_buff[0] = 0;
        mouse_move_buff[1] = 0;
        return;
    }

    ULONGLONG current_millis = GetTickCount64();
    // limit mouse updates to 1 packet per MIN_MOUSE_TICKS milliseconds
    if (mouse_event_data != NULL &&
            (current_millis - last_mouse_move_millis) < MIN_MOUSE_TICKS) {
        mouse_move_buff[0] += mouse_event_data->pt.x - window_center_x;
        mouse_move_buff[1] += mouse_event_data->pt.y - window_center_y;

        // check if a timer has been created to send accumulated mouse moves
        if (!mouse_move_timer) {
            // fprintf(stderr, "[*] Creating timer for mouse move buf %llud\n",
            //     current_millis);
            mouse_move_timer = SetTimer(NULL, 1,
                MIN_MOUSE_TICKS-(current_millis-last_mouse_move_millis),
                mouse_move_timer_cb);
            if (!mouse_move_timer) {
                fprintf(stderr,
                        "[-] Failed to create timer for mouse update\n");
            }
        }

        // Reset mouse to center and return early
        reset_locked_mouse();
        return;
    }

    // If this is not being called by `mouse_move_timer_cb` then accumulate
    // mouse movement
    if (mouse_event_data != NULL) {
        mouse_move_buff[0] += mouse_event_data->pt.x - window_center_x;
        mouse_move_buff[1] += mouse_event_data->pt.y - window_center_y;
    }

    // output mouse movement accumulation

    sprintf(tmp_buffer, ".%ld,%ld\n", mouse_move_buff[0], mouse_move_buff[1]);
    add_to_output_queue(tmp_buffer);

    // Reset state information for mouse movement buffering
    mouse_move_buff[0] = 0;
    mouse_move_buff[1] = 0;
    last_mouse_move_millis = current_millis;
    if (mouse_move_timer) {
        KillTimer(NULL, mouse_move_timer);
        mouse_move_timer = 0;
    }

    // center the mouse on the screen
    reset_locked_mouse();
}


// Sends mouseclick data, such as which button and direction of click, to 
// stdout. Non-standard enum is used for button ids, 0=left, 1=right,
// 2=scroll wheel, 3=x button1, 4=x button2
int register_mouse_click(MSLLHOOKSTRUCT *mouse_event_data, const int button,
        const int click_type)
{
    // TODO: detect if click is on our window?
    is_window_focused();
    if (!is_offscreen) {
        return KEY_NORMAL;
    }
    sprintf(tmp_buffer, "*%d,%d\n", button, click_type);
    add_to_output_queue(tmp_buffer);
    // return KEY_NORMAL;
    return KEY_IGNORE; // ignore all input when offscreen
}


// Removes spaces from `str`
void remove_spaces_from_str(char *str)
{
    const size_t str_len = strlen(str);
    assert(str_len > 0);
    int i = 0;
    int skip = 0;
    for (i = 0; (i+skip) < str_len; i++) {
        while (str[i + skip] == ' ') {
            skip++;
        }
        str[i] = str[i+skip];
    }
    str[i] = '\0';
}


// Returning 0 will cause the next keyboard hook to be called 1 will skip it
// InputHandler sends all keystrokes over stdout as a string description of the
// character such as "A", ";", "Return", "RightShift" instead of a code
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
    is_window_focused(); // Update if we in focus
    if (!is_offscreen) {
        return KEY_NORMAL;
    }
    
    DWORD msg = keyboard_event_data->scanCode << 16;
    if (keyboard_event_data->vkCode != VK_RSHIFT) {
        msg |= (keyboard_event_data->flags & LLKHF_EXTENDED) << 24;
    }

    if (!GetKeyNameTextA(msg, tmp_buffer, BUF_LEN - 1)) {
        fprintf(stderr, "[-] Could not get name of key. msg: %lu\n", msg);
        return KEY_NORMAL;
    }

    remove_spaces_from_str(tmp_buffer);
    char buf[BUF_LEN] = {0}; // TODO: cleanup
    if (is_key_down) {
        sprintf(buf, ",V%.*s\n", BUF_LEN, tmp_buffer);
        key_set_add(msg);
    } else {
        printf(buf, ",^%.*s\n", BUF_LEN, tmp_buffer);
        key_set_erase(msg);
    }
    add_to_output_queue(buf);

    // return KEY_NORMAL;
    return KEY_IGNORE; // ignore all keys while offscreen
}


// Parse if it is mouse scroll up or is_key_down and send to stdout
int register_mouse_scroll(MSLLHOOKSTRUCT *mouse_event_data)
{
    if (!is_offscreen) {
        return KEY_NORMAL;
    }

    // Spec says high order word is equal to +- Wheel delta
    // WORD type is unsigned so checking for positive value
    // is easiest, negative value will be a large value.
    if ((mouse_event_data->mouseData >> 16) == WHEEL_DELTA) {
        add_to_output_queue("|^\n");
    } else {
        add_to_output_queue("|v\n");
    }

    // return KEY_NORMAL;
    return KEY_IGNORE;
}

