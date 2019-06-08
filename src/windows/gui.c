#include <Windows.h>
#include <stdio.h>
#include "gui.h"

#define WIN_W 140
#define WIN_H 140

HWND base_window_handle = NULL;
WNDCLASSEX window_class = { 0 };
HINSTANCE instance_handle = NULL;
BOOL gui_initialized = FALSE;


void gui_setup()
{
    // TODO: Find something to do here
}


const HWND gui_get_window_handle()
{
    return base_window_handle;
}


void gui_get_window_center(int *window_x, int *window_y)
{
    RECT rect = { 0, 0, 0, 0 };
    HWND base_window_handle = gui_get_window_handle();

    if(!GetWindowRect(base_window_handle, &rect)) {
        fprintf(stderr, "[-] GetWindowRect base_window_handle. error: 0x%lx\n",
                GetLastError());
    }

    *window_y = rect.bottom/2 + rect.top/2;
    *window_x = rect.right/2 + rect.left/2;
}


static LRESULT CALLBACK gui_main_window_cb(HWND win_handle, UINT msg, WPARAM wp,
        LPARAM lp)
{
    return DefWindowProc(win_handle, msg, wp, lp);
}


int gui_lock_mouse()
{
    RECT rect = { 0, 0, 0, 0 };
    HWND base_window_handle = gui_get_window_handle();
    if(!GetWindowRect(base_window_handle, &rect)) {
        fprintf(stderr, "[-] GetWindowRect base_window_handle. error: 0x%lx\n",
                GetLastError());
        return 0;
    }

    const int window_center_y = rect.bottom/2 + rect.top/2;
    const int window_center_x = rect.right/2 + rect.left/2;

    rect.top = window_center_y;
    rect.bottom = window_center_y + 1; // +1 works well
    rect.left = window_center_x;
    rect.right = window_center_x + 1;
    ClipCursor(&rect);
    return 1;
}


void gui_unlock_mouse()
{
    ClipCursor(NULL); // Win32 call, passing NULL unlocks the mouse bounds
}


void gui_initialize_window()
{
    if (gui_initialized) {
        fprintf(stderr, "[*] GUI Window is already initialized\n");
        return;
    }
    gui_initialized = TRUE;
    fprintf(stderr, "[+] Creating window\n");

    HINSTANCE instance_handle = (HINSTANCE)GetModuleHandle(NULL);
    LPCTSTR class_name = "KxmWindow";
    LPCTSTR window_title = "kxm";

    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.lpfnWndProc = gui_main_window_cb;
    window_class.hInstance = instance_handle;
    window_class.lpszClassName = class_name;
    window_class.hIcon = NULL;
    window_class.hIconSm = NULL;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)COLOR_WINDOW+1;
    window_class.lpszMenuName = NULL;

    // Register the window class
    if(!RegisterClassEx(&window_class)) {
        fprintf(stderr, "Failed to register window class Error: 0x%lx\n",
                GetLastError());
        gui_initialized = FALSE;
        return;
    }

    base_window_handle = CreateWindowEx(0, class_name, window_title,
            WS_POPUPWINDOW,
            10, 10,
            WIN_W, WIN_H,
            NULL, NULL, instance_handle, NULL);
    
    if (base_window_handle == NULL) {
        fprintf(stderr, "[-] Failed to create window. Error 0x%lx\n",
                GetLastError());
        UnregisterClass(window_class.lpszClassName, instance_handle);
        gui_initialized = FALSE;
        return;
    }

    EnableWindow(base_window_handle, FALSE); // Disable input
}


int gui_is_focused()
{
    return gui_get_window_handle() == GetForegroundWindow();
}


void gui_hide_window()
{
    if (!gui_initialized) {
        return;
    }
    ShowWindow(base_window_handle, SW_HIDE);
    UpdateWindow(base_window_handle);
}


void gui_show_window()
{
    if (!gui_initialized) {
        gui_initialize_window();
    }
    
    ShowWindow(base_window_handle, SW_SHOW);
    UpdateWindow(base_window_handle);

    // Get window to top Z
    // https://stackoverflow.com/questions/916259/
    HWND current_foreground_handle = GetForegroundWindow();
    DWORD foreground_thread_id =\
            GetWindowThreadProcessId(current_foreground_handle, NULL);
    DWORD thread_id = GetCurrentThreadId();
    AttachThreadInput(foreground_thread_id, thread_id, TRUE);
    SetWindowPos(base_window_handle, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOSIZE|SWP_NOMOVE); // Set always topmost
    SetForegroundWindow(base_window_handle);
    AttachThreadInput(foreground_thread_id, thread_id, FALSE);
    SetFocus(base_window_handle);
    SetActiveWindow(base_window_handle);
}


void gui_cleanup()
{
    if (gui_initialized) {
        DestroyWindow(base_window_handle);
        UnregisterClass(window_class.lpszClassName, instance_handle);
        gui_initialized = FALSE;
    }
}
