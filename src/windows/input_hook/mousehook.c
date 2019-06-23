#include <stdio.h>
#include <Windows.h>
#include "mousehook.h"
#include "inputhandler.h"
#include "keydefs.h"

HHOOK mouse_hook_handle = NULL; // our mouse hook


static LRESULT CALLBACK mouse_event_cb(int code, WPARAM wp, LPARAM lp)
{
    if (code < 0) {
        return CallNextHookEx(mouse_hook_handle, code, wp, lp);
    }

    int is_focused = is_input_ignored();

#define M_DATA (mouse_event_data->mouseData >> 16)

    MSLLHOOKSTRUCT *mouse_event_data = (MSLLHOOKSTRUCT*)lp;
    switch (wp) {
    case WM_MOUSEMOVE:
        register_mouse_move(mouse_event_data->pt.x, mouse_event_data->pt.y);
        break;
    case WM_LBUTTONDOWN:
        register_mouse_click(BUTTON_LEFT, CLICK_DOWN);
        break;
    case WM_LBUTTONUP:
        register_mouse_click(BUTTON_LEFT, CLICK_UP);
        break;
    case WM_LBUTTONDBLCLK:
        fprintf(stderr, "[-] Unexpected Left Button double-click event\n");
        break;
    case WM_RBUTTONDOWN:
        register_mouse_click(BUTTON_RIGHT, CLICK_DOWN);
        break;
    case WM_RBUTTONUP:
        register_mouse_click(BUTTON_RIGHT, CLICK_UP);
        break;
    case WM_RBUTTONDBLCLK:
        fprintf(stderr, "[-] Unexpected Right Button double-click event\n");
        break;
    case WM_MBUTTONDOWN:
        register_mouse_click(BUTTON_MIDDLE, CLICK_DOWN);
        break;
    case WM_MBUTTONUP:
        register_mouse_click(BUTTON_MIDDLE, CLICK_UP);
        break;
    case WM_MBUTTONDBLCLK:
        fprintf(stderr, "[-] Unexpected Middle Button double-click event\n");
        break;
    case WM_XBUTTONDOWN:
        register_mouse_click(
                M_DATA == XBUTTON1 ? BUTTON_X1 : BUTTON_X2,
                CLICK_DOWN);
        break;
    case WM_XBUTTONUP:
        register_mouse_click(
                M_DATA == XBUTTON1 ? BUTTON_X1 : BUTTON_X2,
                CLICK_UP);
        break;
    case WM_XBUTTONDBLCLK:
        fprintf(stderr, "[-] Unexpected X Button double-click event\n");
        break;
    case WM_MOUSEWHEEL:
        {
            if (M_DATA == WHEEL_DELTA) {
                register_mouse_click(
                    BUTTON_SCROLL_DOWN, CLICK_DOWN);
                register_mouse_click(
                    BUTTON_SCROLL_DOWN, CLICK_UP);
            } else {
                register_mouse_click(
                    BUTTON_SCROLL_UP, CLICK_DOWN);
                register_mouse_click(
                    BUTTON_SCROLL_UP, CLICK_UP);
            }
        }
        break;
    default:
        fprintf(stderr, "[?] Unknown mouse callback type 0x%llx\n", wp);
        break;
    }

    if (is_focused) {
        return KEY_IGNORE;
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


void mouse_move_to(const int x, const int y)
{
    SetCursorPos(x, y);
}


void mouse_move_relative(const int dx, const int dy)
{
    UINT num_input_structs = 1;
    INPUT input_struct = { 0 };
    input_struct.type = INPUT_MOUSE;
    input_struct.mi.time = 0;
    input_struct.mi.dx = dx;
    input_struct.mi.dy = dy;
    input_struct.mi.dwFlags = MOUSEEVENTF_MOVE;
    int cb_size = sizeof(input_struct);
    UINT num_inserted = SendInput(num_input_structs, &input_struct, cb_size);
    if (num_inserted < 1) {
        fprintf(stderr, "[-] Failed to inject mouse move\n");
    }
}


void mouse_scroll(const int down)
{
    UINT num_input_structs = 1;
    INPUT input_struct = { 0 };
    input_struct.type = INPUT_MOUSE;
    input_struct.mi.time = 0;
    input_struct.mi.dx = 0;
    input_struct.mi.dy = 0;
    input_struct.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input_struct.mi.mouseData = down ? WHEEL_DELTA : -WHEEL_DELTA;
    int cb_size = sizeof(input_struct);
    UINT num_inserted = SendInput(num_input_structs, &input_struct, cb_size);
    if (num_inserted < 1) {
        fprintf(stderr, "[-] Failed to inject mouse scroll\n");
    }
}


void mouse_click(const int button, const int down)
{
    UINT num_input_structs = 1;
    INPUT input_struct = { 0 };
    input_struct.type = INPUT_MOUSE;
    input_struct.mi.time = 0;
    input_struct.mi.dx = 0;
    input_struct.mi.dy = 0;
    #define DOWN_ELSE(X, Y) (down ? X : Y)
    switch (button) {
    case 0: // None
        break;
    case 1: // left
        input_struct.mi.dwFlags = DOWN_ELSE(MOUSEEVENTF_LEFTDOWN,
                MOUSEEVENTF_LEFTUP);
        break;
    case 2: // middle
        input_struct.mi.dwFlags = DOWN_ELSE(MOUSEEVENTF_MIDDLEDOWN,
                MOUSEEVENTF_MIDDLEUP);
        break;
    case 3: // right
        input_struct.mi.dwFlags = DOWN_ELSE(MOUSEEVENTF_RIGHTDOWN,
                MOUSEEVENTF_RIGHTUP);
        break;
    case 4:
        input_struct.mi.mouseData = -WHEEL_DELTA;
        input_struct.mi.dwFlags = MOUSEEVENTF_WHEEL;
        break;
    case 5:
        input_struct.mi.mouseData = WHEEL_DELTA;
        input_struct.mi.dwFlags = MOUSEEVENTF_WHEEL;
        break;
    case 6:
        input_struct.mi.mouseData = -WHEEL_DELTA;
        input_struct.mi.dwFlags = MOUSEEVENTF_HWHEEL;
        break;
    case 7:
        input_struct.mi.mouseData = WHEEL_DELTA;
        input_struct.mi.dwFlags = MOUSEEVENTF_HWHEEL;
        break;
    case 8: // x down
        input_struct.mi.mouseData = XBUTTON1;
        input_struct.mi.dwFlags = DOWN_ELSE(MOUSEEVENTF_XDOWN,
                MOUSEEVENTF_XUP);

        break;
    case 9: // x up
        input_struct.mi.mouseData = XBUTTON2;
        input_struct.mi.dwFlags = DOWN_ELSE(MOUSEEVENTF_XDOWN,
                MOUSEEVENTF_XUP);

        break;
    }
    int cb_size = sizeof(input_struct);
    UINT num_inserted = SendInput(num_input_structs, &input_struct, cb_size);
    if (num_inserted < 1) {
        fprintf(stderr, "[-] Failed to inject mouse click\n");
    }
}
