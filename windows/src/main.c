#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define NTDDI_VERSION NTDDI_VERSION_FROM_WIN32_WINNT
/**
 * Software made by a person, using examples from other people,
 * in the hopes it will be useful. No warranty or support is
 * provided for this code.
 */
#include <stdio.h>
#include <Windows.h>
#include "keyboardhook.h"
#include "mousehook.h"
#include "gui.h"
/**
 * TODO:
 *  - Finalize spec for stdout format and mouse/key names/codes
 *  - Fix ctrl+alt+delete skipping flushing keys
 *    + maybe get callback from windows when not in focus?
 *    + maybe use ctrl+alt as escape keys to pause input proxy client?
 *  - Recover when remote server goes to sleep, or goes offline
 */


// Called at exit, no call this manually
void cleanup()
{
    unhook_keyboard();
    unhook_mouse();
    decimate_window(); // Try and clean up after our gui
    printf("[+] Bye!\n");
}


void setup()
{
    // Hook mouse and keyboard to get input
    if (!hook_keyboard()) {
        fprintf(stderr, "[-] Could not hook keyboard, exiting\n");
        exit(-1);
    }
    if (!hook_mouse()) {
        fprintf(stderr, "[-] Could not hook mouse, exiting\n");
        exit(-2);
    }
}


#ifdef WITH_GUI
DWORD WINAPI gui_thread_loop(void* arg)
{

    initialize_window();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        // Loop forever
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return EXIT_SUCCESS;
}
#endif


DWORD WINAPI input_thread_loop(void* arg)
{
    setup();

#ifdef WITH_GUI
    HANDLE gui_thread = CreateThread(NULL, 0, gui_thread_loop, NULL, 0,
            NULL);
    if (gui_thread == NULL) {
        fprintf(stderr, "[-] Failed to create gui thread\n");
        exit(-4);
    }
#endif

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        // Loop forever
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return EXIT_SUCCESS;
}


int main()
{
    atexit(cleanup);
    // This disables quick-edit mode
    // I think this might clear a lot of flags that we want to clear
    // Making the console less likely to be able to be closed or resized
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, 0x0);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS);
    CloseHandle(hInput);

    HANDLE input_thread = CreateThread(NULL, 0, input_thread_loop, NULL, 0,
            NULL);
    if (input_thread == NULL) {
        fprintf(stderr, "[-] Failed to create input hook thread\n");
        exit(-3);
    }

    WaitForSingleObject(input_thread, INFINITE);

    return EXIT_SUCCESS;
}

