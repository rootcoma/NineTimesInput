#include <stdio.h>
#include <Windows.h>
#include "mousehook.h"
#include "inputhandler.h"

HHOOK mouse_hook_handle = NULL; // our mouse hook


// wp = event type
// lp = event struct data MSLLHOOKSTRUCT*
// When 1 aka KEY_IGNORE is returned from any case, then the normal
// convention of calling the next windows hook will not be done and
// 1 will be returned intead attempting to ignore the mouse event
LRESULT CALLBACK mouse_event_cb(int code, WPARAM wp, LPARAM lp)
{
    if (code < 0) {
        return CallNextHookEx(mouse_hook_handle, code, wp, lp);
    }

#define WHICH_X_BUTTON (mouse_event_data->mouseData >> 16)

    MSLLHOOKSTRUCT *mouse_event_data = (MSLLHOOKSTRUCT*)lp;
    int err = KEY_NORMAL; // No ignored mouse events, call next hook
    switch (wp) {
    case WM_MOUSEMOVE:
        register_mouse_move(mouse_event_data);
        break;
    case WM_LBUTTONDOWN:
        err = register_mouse_click(mouse_event_data, BUTTON_LEFT, CLICK_DOWN);
        break;
    case WM_LBUTTONUP:
        err = register_mouse_click(mouse_event_data, BUTTON_LEFT, CLICK_UP);
        break;
    case WM_LBUTTONDBLCLK:
        err = register_mouse_click(mouse_event_data, BUTTON_LEFT, CLICK_DOUBLE);
        break;
    case WM_RBUTTONDOWN:
        err = register_mouse_click(mouse_event_data, BUTTON_RIGHT, CLICK_DOWN);
        break;
    case WM_RBUTTONUP:
        err = register_mouse_click(mouse_event_data, BUTTON_RIGHT, CLICK_UP);
        break;
    case WM_RBUTTONDBLCLK:
        err = register_mouse_click(mouse_event_data, BUTTON_RIGHT,
                CLICK_DOUBLE);
        break;
    case WM_MBUTTONDOWN:
        err = register_mouse_click(mouse_event_data, BUTTON_MIDDLE, CLICK_DOWN);
        break;
    case WM_MBUTTONUP:
        err = register_mouse_click(mouse_event_data, BUTTON_MIDDLE, CLICK_UP);
        break;
    case WM_MBUTTONDBLCLK:
        err = register_mouse_click(mouse_event_data, BUTTON_MIDDLE,
                CLICK_DOUBLE);
        break;
    case WM_XBUTTONDOWN:
        err = register_mouse_click(mouse_event_data,
                WHICH_X_BUTTON == XBUTTON1 ? BUTTON_X1 : BUTTON_X2,
                CLICK_DOWN);
        break;
    case WM_XBUTTONUP:
        err = register_mouse_click(mouse_event_data,
                WHICH_X_BUTTON == XBUTTON1 ? BUTTON_X1 : BUTTON_X2,
                CLICK_UP);
        break;
    case WM_XBUTTONDBLCLK:
        err = register_mouse_click(mouse_event_data,
                WHICH_X_BUTTON == XBUTTON1 ? BUTTON_X1 : BUTTON_X2,
                CLICK_DOUBLE);
        break;
    case WM_MOUSEWHEEL:
        err = register_mouse_scroll(mouse_event_data);
        break;
    default:
        fprintf(stderr, "[?] Unknown mouse callback type 0x%llx\n", wp);
        break;
    }

    if (err == KEY_IGNORE) {
        return err;
    }

    return CallNextHookEx(mouse_hook_handle, code, wp, lp);
}


// Win32 `SetWindowsHookEx` call with WH_MOUSE_LL as the hook type, this
// sends all WH_MOUSE_LL (LL=lowelevel) events to 
// `LRESULT CALLBACK handleMouse(int code, WPARAM wp, LPARAM lp)` in this file.
int hook_mouse()
{
    if (mouse_hook_handle) {
        fprintf(stderr, "[-] It seems the mouse is already hooked.");
        return FALSE;
    }
    fprintf(stderr, "[+] Hooking Mouse\n");
    mouse_hook_handle = SetWindowsHookEx(WH_MOUSE_LL, mouse_event_cb, NULL, 0);
    if (!mouse_hook_handle) {
        fprintf(stderr, "[-] Failed to set hook.\n");
        return FALSE;
    }
    return TRUE;
}


// Win32 `UnhookWindowsHookEx(HHOOK)` call to cleanup any hook created
void unhook_mouse()
{
    UnhookWindowsHookEx(mouse_hook_handle);
    mouse_hook_handle = NULL;
}

