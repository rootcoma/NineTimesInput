#ifdef _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define NTDDI_VERSION NTDDI_VERSION_FROM_WIN32_WINNT
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include "keyboardhook.h"
#include "mousehook.h"
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#endif
#include <stdio.h>
#include "inputhandler.h"
#include "encryption.h"
#include "config.h"
#include "gui.h"
/**
 * Software made by a person, using examples from other people,
 * in the hopes it will be useful. No warranty or support is
 * provided for this code.
 */
/**
 * TODO:
 *  - Fix ctrl+alt+delete skipping flushing keys
 *    + maybe get callback from windows when not in focus?
 *    + maybe use ctrl+alt as escape keys to pause input proxy client?
 */

#ifdef _WIN32
#define SLEEP(T) Sleep(T)
#define SSCANF sscanf_s
#else
#define SLEEP(T) usleep(T*1000)
#define SSCANF sscanf
#endif


#ifdef _WIN32
//Windows only functions

static void hook_inputs()
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


static DWORD WINAPI server_gui_thread_loop(void* arg)
{
    gui_initialize_window();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg); // Loop forever
    }

    return EXIT_SUCCESS;
}


static DWORD WINAPI server_input_thread_loop(void* arg)
{
    hook_inputs();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg); // Loop forever
    }

    return EXIT_SUCCESS;
}


static void spawn_server_threads()
{
    HANDLE input_thread = CreateThread(NULL, 0, server_input_thread_loop, NULL,
            0, NULL);
    if (input_thread == NULL) {
        fprintf(stderr, "[-] Failed to create input hook thread\n");
        exit(-3);
    }

    HANDLE gui_thread = CreateThread(NULL, 0, server_gui_thread_loop, NULL, 0,
            NULL);
    if (gui_thread == NULL) {
        fprintf(stderr, "[-] Failed to create gui thread\n");
        exit(-4);
    }
}


static void disable_console_quick_edit_mode()
{
    // This disables quick-edit mode and clears flags to make the console less
    // likely to be able to be closed or resized.
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, 0x0);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS); // This clears quick-edit
    CloseHandle(hInput);
}


#else
 // GNU/Linux only functions

static void handle_signal()
{
    exit(EXIT_SUCCESS);
}


static void* gui_event_loop_thread(void *arg)
{
    fprintf(stderr, "[+] Starting server gui event loop\n");
    gui_setup();
    gui_create_window();
    gui_event_loop();
    return NULL;
}


static void spawn_server_threads()
{
    static pthread_t gui_thread;
    pthread_create(&gui_thread, NULL, &gui_event_loop_thread, NULL);
}

#endif


static void setup()
{
    inputhandler_setup();
}


// Called at exit, no call this manually
static void cleanup()
{
    unhook_keyboard();
    unhook_mouse();
    gui_cleanup();

    fprintf(stderr, "[+] Bye!\n");
}


static void parse_command(char *cmd)
{
    int i = 0;
    for(i=0;i<BUF_LEN;i++) {
        if (cmd[i] == '\n') {
            break;
        }
    }
    if (i >= BUF_LEN || i < 2) {
        fprintf(stderr, "[-] Could not find newline, discarding\n");
        fprintf(stderr, "[-] cmd: %.32s", cmd); // TODO: Hardcoded magic num
        return;
    }

    size_t num_read = 0;
    int x = 0;
    int y = 0;
    switch (cmd[0]) {
        case '.': // Mouse move
            num_read = SSCANF(&cmd[1], "%d,%d", &x, &y);
            if (num_read != 2) {
                return; // Invalid data
            }
            mouse_move_relative(x, y);
            //fprintf(stderr, "[+] Mouse move - dx: %d, dy: %d\n", x, y);
            break;
        case '*': // Mouse Click
            num_read = SSCANF(&cmd[1], "%d,%d", &x, &y);
            if (num_read != 2) {
                return; // Invalid data
            }
            mouse_click(x, y); // button = x, clickType = y
            //fprintf(stderr, "[+] Mouse click - button: %d, click: %d\n", x, y);
            break;
        case '|': // Mouse Scroll
            if (cmd[1] != '^' && cmd[1] != 'V' && cmd[1] != 'v') {
                return; // Invalid char
            }
            x = cmd[1] == '^' ? 0 : 1;
            mouse_scroll(x); // x = down
            //fprintf(stderr, "[+] Mouse scroll - direction: %d\n", x);
            break;
        case ',': // Key press;
            if (cmd[1] != '^' && cmd[1] != 'V' && cmd[1] != 'v') {
                return; // Invalid char
            }
            num_read = SSCANF(&cmd[2],"%d", &x);
            if (num_read != 1) {
                return; // Invalid data
            }
            y = cmd[1] == '^' ? 0 : 1;
            keyboard_press(x, y); // x = KeyCode, y = down
            //fprintf(stderr, "[+] Key Press - key_code: %d, down: %d\n", x, y);
            break;
    }
}


static void server_loop_until_exit()
{
    // TODO: Get rid of sleep
    char *plaintext;
    unsigned char cipher_text[BUF_LEN] = {0};
    while (1) {
        plaintext = dequeue_output();
        if (plaintext == NULL) {
            SLEEP(2);
            continue;
        }
        while (plaintext != NULL) {
            crypto_xor_plaintext(cipher_text, plaintext, BUF_LEN);
            fwrite(cipher_text, sizeof(unsigned char), BUF_LEN, stdout);
            plaintext = dequeue_output();
        }
        fflush(stdout);
    }
}


// TODO: Move this to a separate thread, or at least command parsing so that
//       stdin reading is not competing with Win32 SendInput calls on the
//       same thread.
static void client_loop_until_exit()
{
    unsigned char buffer[BUF_LEN] = { 0 };
    char decrypt_buffer[BUF_LEN] = { 0 };
    size_t num_read = 0;
    while (!feof(stdin)) {
        num_read = fread(buffer, sizeof(unsigned char), BUF_LEN, stdin);
        if (num_read == 0 || num_read != BUF_LEN) {
            fprintf(stderr, "[-] read failed size: %u, Max: %u\n",
                    (unsigned int)num_read, BUF_LEN);
            continue;
        }
        crypto_xor_cipher(decrypt_buffer, buffer, BUF_LEN);
        parse_command(decrypt_buffer);

        SLEEP(2); // Allow messages to be processed
    }
}


int main(int argc, char **argv)
{
    atexit(cleanup);

    char *config_filename = "config.txt";

    if (argc > 1) {
        config_filename = argv[1];
    } else {
        fprintf(stderr, "[*] Loading default configuration file '%s'\n",
                config_filename);
    }

    if(!config_parse_file(config_filename)) {
        return -6;
    }
    if(!crypto_setup()) {
        return -5;
    }

    int is_server = 1;
    const char *mode = config_get_value("mode");

    if (strcmp(mode, "client") == 0) {
        is_server = 0;
    } else if (strcmp(mode, "server") != 0) {
        fprintf(stderr, "Key 'mode' for configuration is defaulting to" \
                "'server'\n");
    }

#ifdef _WIN32
    if (is_server) {
        disable_console_quick_edit_mode(); // for server mode
    } else {
        _setmode(_fileno(stdin), _O_BINARY); // for client mode
    }
#else
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    // TODO: Catch USR1 signal and toggle focus based on it
#endif

    if (is_server) {
        spawn_server_threads();
        setup();
        server_loop_until_exit();
    } else {
        gui_setup(); // connnect xcb to X11
        client_loop_until_exit();
    }

    return EXIT_SUCCESS;
}
