#include <stdio.h>
#include <windows.h>
#include "keyboardhook.h"
#include "inputhandler.h"

HHOOK keyboard_hook_handle = NULL; // A reference to our keyboard hook.


// wp = event type
// lp = event struct data KBDLLHOOKSTRUCT*
// Each keystroke is sent to `register_key_stroke` in inputhandler.cpp
// a return value from register_key_stroke indicates whether this callback
// should conform to microsoft standards and continue by calling the next
// hook, or just return 1 to 'ignore most keystrokes' (ctrl+alt+delete will
// still not be ignored)
LRESULT CALLBACK keyboard_hook_cb(int code, WPARAM wp, LPARAM lp)
{
    if (code < 0) {
        // "If nCode is less than zero, the hook must pass the message.."
        return CallNextHookEx(keyboard_hook_handle, code, wp, lp);
    }

    KBDLLHOOKSTRUCT *keyboard_event_data = (KBDLLHOOKSTRUCT*)lp;
    int err = KEY_NORMAL; // No ignored keys, call next hook
    switch (wp) {
    case WM_KEYDOWN:
        err = register_key_stroke(keyboard_event_data, TRUE, FALSE);
        break;
    case WM_SYSKEYDOWN:
        err = register_key_stroke(keyboard_event_data, TRUE, TRUE);
        break;
    case WM_KEYUP:
        err = register_key_stroke(keyboard_event_data, FALSE, FALSE);
        break;
    case WM_SYSKEYUP:
        err = register_key_stroke(keyboard_event_data, FALSE, TRUE);
        break;
    default:
        fprintf(stderr, "[?] Unknown keyboard callback type 0x%llx\n", wp);
        break;
    }

#ifdef GOBBLE_INPUT
    if (err == KEY_IGNORE) {
        return err;
    }
#endif

    return CallNextHookEx(keyboard_hook_handle, code, wp, lp);
}


// Win32 `UnhookWindowsHookEx(HHOOK)` call to cleanup any hook created
void unhook_keyboard()
{
    UnhookWindowsHookEx(keyboard_hook_handle);
    keyboard_hook_handle = NULL;
}


// Win32 `SetWindowsHookEx` call with WH_KEYBOARD_LL as the hook type, this
// sends all WH_KEYBOARD_LL (LL=lowelevel) events to 
// `LRESULT CALLBACK handlekeys(int code, WPARAM wp, LPARAM lp)` in this file.
int hook_keyboard()
{
    if (keyboard_hook_handle) {
        fprintf(stderr, "[-] It seems the keyboard is already hooked.\n");
        return FALSE;
    }
    fprintf(stderr, "[+] Hooking Keyboard\n");
    keyboard_hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook_cb,
            NULL, 0);
    if (!keyboard_hook_handle) {
        fprintf(stderr, "[-] Failed to set hook.\n");
        return FALSE;
    }
    return TRUE;
}

