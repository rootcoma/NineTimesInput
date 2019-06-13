#include <Windows.h>
#include <stdio.h>
#include "gui.h"


#define WIN_W 140
#define WIN_H 140

// This needs a more descriptive name
HWND base_window_handle = NULL;

WNDCLASSEX window_class = { 0 };

HINSTANCE instance_handle = NULL;

BOOL gui_initialized = FALSE;


const HWND gui_get_window_handle()
{
    return base_window_handle;
}


LRESULT CALLBACK gui_main_window_cb(HWND win_handle, UINT msg, WPARAM wp, LPARAM lp)
{
    // switch (msg) {
    // case WM_DESTROY:
        // PostQuitMessage(0);
        // return FALSE;
    // case WM_PAINT:
        // PAINTSTRUCT ps;
        // BeginPaint(win_handle, &ps);
        // EndPaint(win_handle, &ps);
        // return FALSE;
    // default:
        // return DefWindowProc(win_handle, msg, wp, lp);
    // }
    // 
    // return FALSE;

    return DefWindowProc(win_handle, msg, wp, lp);
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
    const int screen_right = GetSystemMetrics(SM_CXSCREEN);
    const int screen_bottom = GetSystemMetrics(SM_CYSCREEN);
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
            screen_right/2-WIN_W/2,
            screen_bottom/2-WIN_H/2,
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
    // Set always topmost
    SetWindowPos(base_window_handle, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOSIZE|SWP_NOMOVE);
    SetForegroundWindow(base_window_handle);
    AttachThreadInput(foreground_thread_id, thread_id, FALSE);
    
    SetFocus(base_window_handle);
    SetActiveWindow(base_window_handle);
}


void gui_decimate_window()
{
    if (gui_initialized) {
        DestroyWindow(base_window_handle);
        UnregisterClass(window_class.lpszClassName, instance_handle);
        gui_initialized = FALSE;
    }
}

