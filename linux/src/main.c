#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <X11/extensions/XTest.h>
#include "encryption.h"

// Make sure BUFFER_LEN < INT_MAX
// This needs to match everywhere for xor process to work right now
#define BUF_LEN 24

/**
 * TODO:
 *  - Clear all down keys when program exits
 *  - Figure out which enums are needed for both x buttons
 */
Display *display; // Holds X display which allows for input injection

Window root_window; // Not sure if needed globally


void mouse_click(const int button, const int click_type)
{
    if (button > 4) {
        fprintf(stderr, "[>] TODO: Implement ms btn > 4. (b:%d,%d)\n",
                button, click_type);
        return;
    }
    if (click_type > 1) {
        fprintf(stderr, "[!] Skipping click. (b:%d,%d)\n",
                button, click_type);
        return; // skip anything crazy for now
    }
    int button_enum = 0;
    // What is enum for buttons?
    if (button == 0) {
        button_enum = 1; // left click
    } else if (button == 1) {
        button_enum = 3; // right
    } else if (button == 2) {
        button_enum = 2; // middle click ? I want scroll wheel click for this
    } else if (button == 3) {
        button_enum = 5; // X button closest to user
                         // TODO: should be back, is currently scroll down
    } else if (button == 4) {
        button_enum = 4; // X button closest to monitor, TODO: should be forward
    }
    
    if (click_type == 0) {
        XTestFakeButtonEvent(display, button_enum, True, CurrentTime);
    } else {
        XTestFakeButtonEvent(display, button_enum, False, CurrentTime);
    }
    XFlush(display);
    //fprintf(stderr, "[*] Button click: button: %d, clickType: %d\n",
    //        button, click_type);
}


KeyCode convert_to_key_code(const char* char_str)
{
    static const char *conversions[] = {
            "Alt",            "Alt_L",
            "Application",    "Menu",
            "Backspace",      "BackSpace",
            "CapsLock",       "Caps_Lock",
            "Ctrl",           "Control_L",
            "Enter",          "Return",
            "Esc",            "Escape",
            "LeftWindows",    "Super_L",
            "Num0",           "KP_0",
            "Num1",           "KP_1",
            "Num2",           "KP_2",
            "Num3",           "KP_3",
            "Num4",           "KP_4",
            "Num5",           "KP_5",
            "Num6",           "KP_6",
            "Num7",           "KP_7",
            "Num8",           "KP_8",
            "Num9",           "KP_9",
            "Num/",           "KP_Divide",
            "Num*",           "KP_Multiply",
            "Num-",           "KP_Subtract",
            "Num+",           "KP_Add",
            "NumLock",        "Num_Lock",
            "NumEnter",       "KP_Enter",
            "NumDel",         "KP_Delete", // Num period
            "PageUp",         "Page_Up",
            "PageDown",       "Page_Down",
            "PrntScrn",       "Print", // Not sure
            "RightWindows",   "Super_R",
            "RightAlt",       "Alt_R",
            "RightCtrl",      "Control_R",
            "RightShift",     "Shift_R",
            "Shift",          "Shift_L",
            "Space",          "space",
            ";",              "semicolon",
            "[",              "bracketleft",
            "]",              "bracketright",
            "\\",             "backslash",
            "/",              "slash",
            "-",              "minus",
            "`",              "grave",
            "'",              "quotedbl",
            ",",              "comma",
            ".",              "period",
            "=",              "equal",
    };
    for(int i=0; i<sizeof(conversions)/sizeof(const char*); i+=2) {
        if (!strcmp(conversions[i], char_str)) {
            return XKeysymToKeycode(display, XStringToKeysym(conversions[i+1]));
        }
    }
    if (strlen(char_str) == 1) {
        return XKeysymToKeycode(display, XStringToKeysym(char_str));
    }

    KeyCode ret = XKeysymToKeycode(display, XStringToKeysym(char_str));
    if (ret == 0) {
        fprintf(stderr, "[*] Missing Input: %s\n", char_str);
    }
    return ret;
}


void keyboard_press(const char* char_str, const int down)
{
    KeyCode key_code = convert_to_key_code(char_str);
    if (key_code == 0) {
        return;
    }
    XTestFakeKeyEvent(display, key_code, down ? True : False, CurrentTime);
    XFlush(display);
}


void mouse_move_to(const int x, const int y)
{
    XTestFakeRelativeMotionEvent(display, x, y, CurrentTime);
    XFlush(display);
}


void parse_command(char *cmd)
{
#ifndef SKIP_SAFE_CHECK	
	int i = 0;
	for(i=0;i<BUF_LEN;i++) {
		if (cmd[i] == '\n') {
			break;
		}
	}
	if (i >= BUF_LEN || i < 2) {
		fprintf(stderr, "[-] Could not find newline, discarding\n");
		return;
	}
#endif
	size_t num_read = 0;
	int x = 0;
	int y = 0;
	char tmp[BUF_LEN] = { 0 };
    switch (cmd[0]) {
        case '.': // Mouse move
            num_read = sscanf(&cmd[1], "%d,%d", &x, &y);
            if (num_read != 2) {
                return; // Invalid data
            }
            mouse_move_to(x, y);
            break;
        case '*': // Mouse Click
            num_read = sscanf(&cmd[1], "%d,%d", &x, &y);
            if (num_read != 2) {
                return; // Invalid data
            }
            mouse_click(x, y); // button = x, clickType = y
            break;
        case '|': // Mouse Scroll
            if (cmd[1] != '^' && cmd[1] != 'V' && cmd[1] != 'v') {
                return; // Invalid char
            }
            x = cmd[1] == '^' ? 4 : 3;
            mouse_click(x, 0); // x = button, 3 x1, 4 x2
            mouse_click(x, 1);
            break;
        case ',': // Key press;
            if (cmd[1] != '^' && cmd[1] != 'V' && cmd[1] != 'v') {
                return; // Invalid char
            }
            num_read = sscanf(&cmd[2],"%22s", tmp);
            if (num_read != 1) {
                return; // Invalid data
            }
            x = cmd[1] == '^' ? 0 : 1;
            keyboard_press(tmp, x); // tmp = Key Name, x = down
            break;
    }
}


void loop()
{
	unsigned char buffer[BUF_LEN] = { 0 };
	char decrypt_buffer[BUF_LEN] = { 0 };
	int num_read = 0;

	while (!feof(stdin)) {
		num_read = (int)fread(buffer, sizeof(unsigned char), BUF_LEN, stdin);
		if (num_read == 0 || num_read != BUF_LEN) {
			fprintf(stderr, "[-] read failed size: %ul, Max: %ul\n", num_read, BUF_LEN);
			continue;
		}
		output_decrypt(decrypt_buffer, buffer, BUF_LEN);
		parse_command(decrypt_buffer);
	}
}


void cleanup()
{
    XCloseDisplay(display);
    fprintf(stderr, "[+] Bye!\n");
}


void setup()
{
    display = XOpenDisplay(0);
    if (display == NULL) {
        fprintf(stderr, "[-] Failed to open XDisplay, exiting");
        exit(-1);
    }
    root_window = XRootWindow(display, 0);
}


void handle_signal()
{
    exit(EXIT_SUCCESS);
}


int main()
{
    atexit(cleanup);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    setup();
    loop();

    return EXIT_SUCCESS;
}

