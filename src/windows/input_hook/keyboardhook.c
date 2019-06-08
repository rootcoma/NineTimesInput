#include <stdio.h>
#include <windows.h>
#include "keydefs.h"
#include "keyboardhook.h"
#include "inputhandler.h"

HHOOK keyboard_hook_handle = NULL; // A reference to our keyboard hook.


static LRESULT CALLBACK keyboard_hook_cb(int code, WPARAM wp, LPARAM lp)
{
    if (code < 0) {
        // "If nCode is less than zero, the hook must pass the message.."
        return CallNextHookEx(keyboard_hook_handle, code, wp, lp);
    }

    int is_focused = is_input_ignored();
    KBDLLHOOKSTRUCT *keyboard_event_data = (KBDLLHOOKSTRUCT*)lp;
    unsigned char keycode =\
            windows_vk_to_linux_keycode(keyboard_event_data->vkCode);
    switch (wp) {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        register_key_stroke(keycode, 1);
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        register_key_stroke(keycode, 0);
        break;
    default:
        fprintf(stderr, "[?] Unknown keyboard callback type 0x%llx\n", wp);
        break;
    }

    if (is_focused) {
        return KEY_IGNORE;
    }

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


void keyboard_press(const int key_code, int is_down)
{
    WORD vk_code = linux_keycode_to_windows_vk(key_code);

    UINT num_input_structs = 1;
    INPUT input_struct = { 0 };
    input_struct.type = INPUT_KEYBOARD;
    input_struct.ki.dwFlags = is_down ? 0 : KEYEVENTF_KEYUP;
    input_struct.ki.wVk = vk_code;
    HKL keyboard_layout = GetKeyboardLayout(0); // 0 for current thread
    input_struct.ki.wScan = MapVirtualKeyEx(vk_code, MAPVK_VK_TO_VSC,
            keyboard_layout);
    int cb_size = sizeof(input_struct);
    UINT num_inserted = SendInput(num_input_structs, &input_struct, cb_size);
    if (num_inserted < 1) {
        fprintf(stderr, "[-] Failed to inject keystroke. vk_code: %u\n",
                vk_code);
    }
}
