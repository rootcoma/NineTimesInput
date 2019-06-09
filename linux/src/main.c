#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//#include <X11/X.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

/**
 * TODO:
 *  - Clear all down keys when program exits
 *  - Figure out which enums are needed for both x buttons
 */
Display *dpy; // Holds X display which allows for input injection

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
        XTestFakeButtonEvent(dpy, button_enum, True, CurrentTime);
    } else {
        XTestFakeButtonEvent(dpy, button_enum, False, CurrentTime);
    }
    XFlush(dpy);
    //fprintf(stderr, "[*] Button click: button: %d, clickType: %d\n",
    //        button, click_type);
}


KeyCode convertToKeyCode(const char* char_str)
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
            return XKeysymToKeycode(dpy, XStringToKeysym(conversions[i+1]));
        }
    }
    if (strlen(char_str) == 1) {
        return XKeysymToKeycode(dpy, XStringToKeysym(char_str));
    }

    KeyCode ret = XKeysymToKeycode(dpy, XStringToKeysym(char_str));
    if (ret == 0) {
        fprintf(stderr, "[*] Missing Input: %s\n", char_str);
    }
    return ret;
}


void send_key(const char* char_str, const int down)
{
    KeyCode keyCode = convertToKeyCode(char_str);
    if (keyCode == 0) {
        return;
    }
    XTestFakeKeyEvent(dpy, keyCode, down ? True : False, CurrentTime);
    XFlush(dpy);
}


void move_to(const int x, const int y)
{
    XTestFakeRelativeMotionEvent(dpy, x, y, CurrentTime);
    XFlush(dpy);
}


void loop()
{
    char c = ' ';
    char tmp[32] = { 0 };
    int num_read = -1;
    int x = 0;
    int y = 0;
    while(c != EOF) {
        c = getchar();
        switch (c) {
        case '.': // Mouse move
            num_read = scanf("%d,%d", &x, &y);
            if (num_read != 2) {
                continue; // Invalid data
            }
            move_to(x, y); // move the mouse
            break;
        case '*': // Mouse Click
            num_read = scanf("%d,%d", &x, &y);
            if (num_read != 2) {
                continue; // Invalid data
            }
            // button = x, clickType = y
            mouse_click(x, y);
            break;
        case '|': // Mouse Scroll
            c = getchar();
            if (c != '^' && c != 'V' && c != 'v') {
                continue; // Invalid char
            }
            x = c == '^' ? 4 : 3;
            // x = button, 3 x1, 4 x2
            mouse_click(x, 0);
            mouse_click(x, 1);
            break;
        case ',': // Key press
            c = getchar();
            if (c != '^' && c != 'V' && c != 'v') {
                continue; // Invalid char
            }
            num_read = scanf("%31s", tmp);
            if (num_read != 1) {
                continue; // Invalid data
            }
            x = c == '^' ? 0 : 1;
            // tmp = Key Name, x = down
            send_key(tmp, x);
            break;
        }
    }
}


void cleanup()
{
    XCloseDisplay(dpy);
    fprintf(stderr, "[+] Bye!\n");
}


void setup()
{
    dpy = XOpenDisplay(0);
    if (dpy == NULL) {
        fprintf(stderr, "[-] Failed to open XDisplay, exiting");
        exit(-1);
    }
    root_window = XRootWindow(dpy, 0);
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

