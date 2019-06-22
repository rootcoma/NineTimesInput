#include <stdio.h>
#include "keydefs.h"

// This is from `xmodmap -pke` ran on my gnu/linux machine
#define X_KEYCODE_NO_MATCH 0
#define X_KEYCODE_NEED_MATCH X_KEYCODE_NO_MATCH
#define X_KEYCODE_ESCAPE 9                  // Escape NoSymbol Escape
#define X_KEYCODE_1 10                      // 1 exclam 1 exclam
#define X_KEYCODE_2 11                      // 2 at 2 at
#define X_KEYCODE_3 12                      // 3 numbersign 3 numbersign
#define X_KEYCODE_4 13                      // 4 dollar 4 dollar
#define X_KEYCODE_5 14                      // 5 percent 5 percent
#define X_KEYCODE_6 15                      // 6 asciicircum 6 asciicircum
#define X_KEYCODE_7 16                      // 7 ampersand 7 ampersand
#define X_KEYCODE_8 17                      // 8 asterisk 8 asterisk
#define X_KEYCODE_9 18                      // 9 parenleft 9 parenleft
#define X_KEYCODE_0 19                      // 0 parenright 0 parenright
#define X_KEYCODE_MINUS 20                  // minus underscore minus underscore
#define X_KEYCODE_EQUALS 21                 // equal plus equal plus
#define X_KEYCODE_BACKSPACE 22              // BackSpace Terminate_Server BackSpace Terminate_Server
#define X_KEYCODE_L_TAB 23                  // Tab ISO_Left_Tab Tab ISO_Left_Tab
#define X_KEYCODE_Q 24                      // q Q q Q
#define X_KEYCODE_W 25                      // w W w W
#define X_KEYCODE_E 26                      // e E e E
#define X_KEYCODE_R 27                      // r R r R
#define X_KEYCODE_T 28                      // t T t T
#define X_KEYCODE_Y 29                      // y Y y Y
#define X_KEYCODE_U 30                      // u U u U
#define X_KEYCODE_I 31                      // i I i I
#define X_KEYCODE_O 32                      // o O o O
#define X_KEYCODE_P 33                      // p P p P
#define X_KEYCODE_BRACKET_L 34              // bracketleft braceleft bracketleft braceleft
#define X_KEYCODE_BRACKET_R 35              // bracketright braceright bracketright braceright
#define X_KEYCODE_ENTER  36                 // Return NoSymbol Return
#define X_KEYCODE_L_CTRL  37                // Control_L NoSymbol Control_L
#define X_KEYCODE_A 38                      // a A a A
#define X_KEYCODE_S 39                      // s S s S
#define X_KEYCODE_D 40                      // d D d D
#define X_KEYCODE_F 41                      // f F f F
#define X_KEYCODE_G 42                      // g G g G
#define X_KEYCODE_H 43                      // h H h H
#define X_KEYCODE_J 44                      // j J j J
#define X_KEYCODE_K 45                      // k K k K
#define X_KEYCODE_L 46                      // l L l L
#define X_KEYCODE_SEMICOLON 47              // semicolon colon semicolon colon
#define X_KEYCODE_QUOTE 48                  // apostrophe quotedbl apostrophe quotedbl
#define X_KEYCODE_GRAVE 49                  // grave asciitilde grave asciitilde
#define X_KEYCODE_L_SHIFT 50                // Shift_L NoSymbol Shift_L
#define X_KEYCODE_BACKSLASH 51              // backslash bar backslash bar
#define X_KEYCODE_Z 52                      // z Z z Z
#define X_KEYCODE_X 53                      // x X x X
#define X_KEYCODE_C 54                      // c C c C
#define X_KEYCODE_V 55                      // v V v V
#define X_KEYCODE_B 56                      // b B b B
#define X_KEYCODE_N 57                      // n N n N
#define X_KEYCODE_M 58                      // m M m M
#define X_KEYCODE_COMMA 59                  // comma less comma less
#define X_KEYCODE_PERIOD 60                 // period greater period greater
#define X_KEYCODE_SLASH 61                  // slash question slash question
#define X_KEYCODE_R_SHIFT 62                // Shift_R NoSymbol Shift_R
#define X_KEYCODE_KP_MULTIPLY 63            // KP_Multiply XF86ClearGrab KP_Multiply XF86ClearGrab
#define X_KEYCODE_L_ALT 64                  // Alt_L Meta_L Alt_L Meta_L
#define X_KEYCODE_SPACE 65                  // space NoSymbol space
#define X_KEYCODE_CAPSLOCK 66               // Caps_Lock NoSymbol Caps_Lock
#define X_KEYCODE_F1 67                     // F1 XF86Switch_VT_1 F1 XF86Switch_VT_1
#define X_KEYCODE_F2 68                     // F2 XF86Switch_VT_2 F2 XF86Switch_VT_2
#define X_KEYCODE_F3 69                     // F3 XF86Switch_VT_3 F3 XF86Switch_VT_3
#define X_KEYCODE_F4 70                     // F4 XF86Switch_VT_4 F4 XF86Switch_VT_4
#define X_KEYCODE_F5 71                     // F5 XF86Switch_VT_5 F5 XF86Switch_VT_5
#define X_KEYCODE_F6 72                     // F6 XF86Switch_VT_6 F6 XF86Switch_VT_6
#define X_KEYCODE_F7 73                     // F7 XF86Switch_VT_7 F7 XF86Switch_VT_7
#define X_KEYCODE_F8 74                     // F8 XF86Switch_VT_8 F8 XF86Switch_VT_8
#define X_KEYCODE_F9 75                     // F9 XF86Switch_VT_9 F9 XF86Switch_VT_9
#define X_KEYCODE_F10 76                    // F10 XF86Switch_VT_10 F10 XF86Switch_VT_10
#define X_KEYCODE_NUMLOCK 77                // Num_Lock Pointer_EnableKeys Num_Lock Pointer_EnableKeys
#define X_KEYCODE_SCROLLLOCK 78             // Scroll_Lock NoSymbol Scroll_Lock
#define X_KEYCODE_KP_7 79                   // KP_Home KP_7 KP_Home KP_7
#define X_KEYCODE_KP_8 80                   // KP_Up KP_8 KP_Up KP_8
#define X_KEYCODE_KP_9 81                   // KP_Prior KP_9 KP_Prior KP_9
#define X_KEYCODE_KP_SUBTRACT 82            // KP_Subtract XF86Prev_VMode KP_Subtract XF86Prev_VMode
#define X_KEYCODE_KP_4 83                   // KP_Left KP_4 KP_Left KP_4
#define X_KEYCODE_KP_5 84                   // KP_Begin KP_5 KP_Begin KP_5
#define X_KEYCODE_KP_6 85                   // KP_Right KP_6 KP_Right KP_6
#define X_KEYCODE_KP_ADD 86                 // KP_Add XF86Next_VMode KP_Add XF86Next_VMode
#define X_KEYCODE_KP_1 87                   // KP_End KP_1 KP_End KP_1
#define X_KEYCODE_KP_2 88                   // KP_Down KP_2 KP_Down KP_2
#define X_KEYCODE_KP_3 89                   // KP_Next KP_3 KP_Next KP_3
#define X_KEYCODE_KP_0 90                   // KP_Insert KP_0 KP_Insert KP_0
#define X_KEYCODE_KP_PERIOD 91              // KP_Delete KP_Decimal KP_Delete KP_Decimal
#define X_KEYCODE_LEVEL3_SHIFT 92           // ISO_Level3_Shift NoSymbol ISO_Level3_Shift
#define X_KEYCODE_LESS_GREATER 94           // less greater less greater bar brokenbar
#define X_KEYCODE_F11 95                    // F11 XF86Switch_VT_11 F11 XF86Switch_VT_11
#define X_KEYCODE_F12 96                    // F12 XF86Switch_VT_12 F12 XF86Switch_VT_12
#define X_KEYCODE_KATAKANA 98               // Katakana NoSymbol Katakana
#define X_KEYCODE_HIRAGANA 99               // Hiragana NoSymbol Hiragana
#define X_KEYCODE_HENKAN_MODE 100           // Henkan_Mode NoSymbol Henkan_Mode
#define X_KEYCODE_HIRAGANA_KATAKANA 101     // Hiragana_Katakana NoSymbol Hiragana_Katakana
#define X_KEYCODE_MUHENKAN 102              // Muhenkan NoSymbol Muhenkan
#define X_KEYCODE_KP_ENTER 104              // KP_Enter NoSymbol KP_Enter
#define X_KEYCODE_R_CTRL 105                // Control_R NoSymbol Control_R
#define X_KEYCODE_KP_DIVIDE 106             // KP_Divide XF86Ungrab KP_Divide XF86Ungrab
#define X_KEYCODE_PRINTSCREEN 107           // Print Sys_Req Print Sys_Req
#define X_KEYCODE_R_ALT 108                 // Alt_R Meta_R Alt_R Meta_R
#define X_KEYCODE_LINEFEED 109              // Linefeed NoSymbol Linefeed
#define X_KEYCODE_HOME 110                  // Home NoSymbol Home
#define X_KEYCODE_UP 111                    // Up NoSymbol Up
#define X_KEYCODE_PAGEUP 112                // Prior NoSymbol Prior
#define X_KEYCODE_LEFT 113                  // Left NoSymbol Left
#define X_KEYCODE_RIGHT 114                 // Right NoSymbol Right
#define X_KEYCODE_END 115                   // End NoSymbol End
#define X_KEYCODE_DOWN 116                  // Down NoSymbol Down
#define X_KEYCODE_PAGEDOWN 117              // Next NoSymbol Next
#define X_KEYCODE_INSERT 118                // Insert NoSymbol Insert
#define X_KEYCODE_DELETE 119                // Delete NoSymbol Delete
#define X_KEYCODE_AUDIOMUTE 121             // XF86AudioMute NoSymbol XF86AudioMute
#define X_KEYCODE_AUDIOLOWER 122            // XF86AudioLowerVolume NoSymbol XF86AudioLowerVolume
#define X_KEYCODE_AUDIORAISE 123            // XF86AudioRaiseVolume NoSymbol XF86AudioRaiseVolume
#define X_KEYCODE_POWEROFF 124              // XF86PowerOff NoSymbol XF86PowerOff
#define X_KEYCODE_KP_EQUAL 125              // KP_Equal NoSymbol KP_Equal
#define X_KEYCODE_PLUSMINUS 126             // plusminus NoSymbol plusminus
#define X_KEYCODE_PAUSE 127                 // Pause Break Pause Break
//#define X_KEYCODE_KP_PERIOD 129             // KP_Decimal NoSymbol KP_Decimal
#define X_KEYCODE_HANGUL 130                // Hangul NoSymbol Hangul
#define X_KEYCODE_HANGUL_HANJA 131          // Hangul_Hanja NoSymbol Hangul_Hanja
#define X_KEYCODE_L_SUPER 133               // Super_L NoSymbol Super_L
#define X_KEYCODE_R_SUPER 134               // Super_R NoSymbol Super_R
#define X_KEYCODE_MENU 135                  // Menu NoSymbol Menu
#define X_KEYCODE_CANCEL 136                // Cancel NoSymbol Cancel
#define X_KEYCODE_REDO 137                  // Redo NoSymbol Redo
#define X_KEYCODE_SUNP_ROPS 138             // SunProps NoSymbol SunProps
#define X_KEYCODE_UNDO 139                  // Undo NoSymbol Undo
#define X_KEYCODE_SUN_FRONT 140             // SunFront NoSymbol SunFront
#define X_KEYCODE_COPY 141                  // XF86Copy NoSymbol XF86Copy
#define X_KEYCODE_SUN_OPEN 142              // SunOpen NoSymbol SunOpen
#define X_KEYCODE_PASTE 143                 // XF86Paste NoSymbol XF86Paste
#define X_KEYCODE_FIND 144                  // Find NoSymbol Find
#define X_KEYCODE_CUT 145                   // XF86Cut NoSymbol XF86Cut
#define X_KEYCODE_HELP 146                  // Help NoSymbol Help
//#define X_KEYCODE_MENUKB 147                // XF86MenuKB NoSymbol XF86MenuKB
#define X_KEYCODE_CALC 148                  // XF86Calculator NoSymbol XF86Calculator
#define X_KEYCODE_SLEEP 150                 // XF86Sleep NoSymbol XF86Sleep
#define X_KEYCODE_WAKE 151                  // XF86WakeUp NoSymbol XF86WakeUp
#define X_KEYCODE_EXPLORER 152              // XF86Explorer NoSymbol XF86Explorer
#define X_KEYCODE_SEND 153                  // XF86Send NoSymbol XF86Send
#define X_KEYCODE_TRANSFER 155              // XF86Xfer NoSymbol XF86Xfer
#define X_KEYCODE_LAUNCH1 156               // XF86Launch1 NoSymbol XF86Launch1
#define X_KEYCODE_LAUNCH2 157               // XF86Launch2 NoSymbol XF86Launch2
#define X_KEYCODE_WWW 158                   // XF86WWW NoSymbol XF86WWW
#define X_KEYCODE_DOS 159                   // XF86DOS NoSymbol XF86DOS
#define X_KEYCODE_SCREENSAVER 160           // XF86ScreenSaver NoSymbol XF86ScreenSaver
#define X_KEYCODE_ROTATE 162                // XF86RotateWindows NoSymbol XF86RotateWindows
#define X_KEYCODE_MAIL 163                  // XF86Mail NoSymbol XF86Mail
#define X_KEYCODE_FAVORITES 164             // XF86Favorites NoSymbol XF86Favorites
#define X_KEYCODE_MYCOMPUTER 165            // XF86MyComputer NoSymbol XF86MyComputer
#define X_KEYCODE_BACK 166                  // XF86Back NoSymbol XF86Back
#define X_KEYCODE_FORWARD 167               // XF86Forward NoSymbol XF86Forward
#define X_KEYCODE_EJECT 169                 // XF86Eject NoSymbol XF86Eject
//#define X_KEYCODE_EJECT 170                 // XF86Eject XF86Eject XF86Eject XF86Eject
#define X_KEYCODE_AUDIONEXT 171             // XF86AudioNext NoSymbol XF86AudioNext
#define X_KEYCODE_AUDIOPLAY 172             // XF86AudioPlay XF86AudioPause XF86AudioPlay XF86AudioPause
#define X_KEYCODE_AUDIOPREV 173             // XF86AudioPrev NoSymbol XF86AudioPrev
#define X_KEYCODE_AUDIOSTOP 174             // XF86AudioStop XF86Eject XF86AudioStop XF86Eject
#define X_KEYCODE_AUDIORECORD 175           // XF86AudioRecord NoSymbol XF86AudioRecord
#define X_KEYCODE_AUDIOREWIND 176           // XF86AudioRewind NoSymbol XF86AudioRewind
#define X_KEYCODE_PHONE 177                 // XF86Phone NoSymbol XF86Phone
#define X_KEYCODE_TOOLS 179                 // XF86Tools NoSymbol XF86Tools
#define X_KEYCODE_HOMEPAGE 180              // XF86HomePage NoSymbol XF86HomePage
#define X_KEYCODE_RELOAD 181                // XF86Reload NoSymbol XF86Reload
#define X_KEYCODE_CLOSE 182                 // XF86Close NoSymbol XF86Close
#define X_KEYCODE_SCROLLUP 185              // XF86ScrollUp NoSymbol XF86ScrollUp
#define X_KEYCODE_SCROLLDOWN 186            // XF86ScrollDown NoSymbol XF86ScrollDown
#define X_KEYCODE_PAREN_L 187               // parenleft NoSymbol parenleft
#define X_KEYCODE_PAREN_R 188               // parenright NoSymbol parenright
#define X_KEYCODE_NEW 189                   // XF86New NoSymbol XF86New
//#define X_KEYCODE_REDO 190                  // Redo NoSymbol Redo
#define X_KEYCODE_TOUCHPAD_TOGGLE 200       // XF86TouchpadToggle NoSymbol XF86TouchpadToggle
#define X_KEYCODE_MODE_SWITCH 203           // Mode_switch NoSymbol Mode_switch
//#define X_KEYCODE_L_ALT 204                 // NoSymbol Alt_L NoSymbol Alt_L
#define X_KEYCODE_L_META 205                // NoSymbol Meta_L NoSymbol Meta_L
//#define X_KEYCODE_L_SUPER 206               // NoSymbol Super_L NoSymbol Super_L
#define X_KEYCODE_L_HYPER 207               // NoSymbol Hyper_L NoSymbol Hyper_L
//#define X_KEYCODE_AUDIOPLAY 208             // XF86AudioPlay NoSymbol XF86AudioPlay
#define X_KEYCODE_AUDIOPAUSE 209            // XF86AudioPause NoSymbol XF86AudioPause
#define X_KEYCODE_LAUNCH3 210               // XF86Launch3 NoSymbol XF86Launch3
#define X_KEYCODE_LAUNCH4 211               // XF86Launch4 NoSymbol XF86Launch4
#define X_KEYCODE_SUSPEND 213               // XF86Suspend NoSymbol XF86Suspend
//#define X_KEYCODE_CLOSE 214                 // XF86Close NoSymbol XF86Close
//#define X_KEYCODE_AUDIOPLAY 215             // XF86AudioPlay NoSymbol XF86AudioPlay
#define X_KEYCODE_AUDIOFORWARD 216          // XF86AudioForward NoSymbol XF86AudioForward
#define X_KEYCODE_PRINT 218                 // Print NoSymbol Print
#define X_KEYCODE_WEBCAM 220                // XF86WebCam NoSymbol XF86WebCam
//#define X_KEYCODE_MAIL 223                // XF86Mail NoSymbol XF86Mail
#define X_KEYCODE_SEARCH 225                // XF86Search NoSymbol XF86Search
#define X_KEYCODE_FINANCE 227               // XF86Finance NoSymbol XF86Finance
#define X_KEYCODE_SHOP 229                  // XF86Shop NoSymbol XF86Shop
//#define X_KEYCODE_CANCEL 231                // Cancel NoSymbol Cancel
#define X_KEYCODE_DISP_BRIGHTNESSDOWN 232   // XF86MonBrightnessDown NoSymbol XF86MonBrightnessDown
#define X_KEYCODE_DISP_BRIGHTNESSUP 233     // XF86MonBrightnessUp NoSymbol XF86MonBrightnessUp
#define X_KEYCODE_AUDIOMEDIA 234            // XF86AudioMedia NoSymbol XF86AudioMedia
#define X_KEYCODE_DISPLAY 235               // XF86Display NoSymbol XF86Display
#define X_KEYCODE_KBD_LIGHTTOGGLE 236       // XF86KbdLightOnOff NoSymbol XF86KbdLightOnOff
#define X_KEYCODE_KBD_BRIGHTNESSDOWN 237    // XF86KbdBrightnessDown NoSymbol XF86KbdBrightnessDown
#define X_KEYCODE_KBD_BRIGHTNESSUP 238      // XF86KbdBrightnessUp NoSymbol XF86KbdBrightnessUp
//#define X_KEYCODE_SEND 239                  // XF86Send NoSymbol XF86Send
#define X_KEYCODE_REPLY 240                 // XF86Reply NoSymbol XF86Reply
#define X_KEYCODE_MAILFORWARD 241           // XF86MailForward NoSymbol XF86MailForward
#define X_KEYCODE_SAVE 242                  // XF86Save NoSymbol XF86Save
#define X_KEYCODE_DOCUMENTS 243             // XF86Documents NoSymbol XF86Documents
#define X_KEYCODE_BATTERY 244               // XF86Battery NoSymbol XF86Battery
#define X_KEYCODE_BLUETOOTH 245             // XF86Bluetooth NoSymbol XF86Bluetooth
#define X_KEYCODE_WLAN 246                  // XF86WLAN NoSymbol XF86WLAN
#define X_KEYCODE_WWAN 254                  // XF86WWAN NoSymbol XF86WWAN
#define X_KEYCODE_RFKILL 255                // XF86RFKill NoSymbol XF86RFKill


// Used http://cherrytree.at/misc/vk.htm as reference
// X_KEYCODE_NO_MATCH = no match, X_KEYCODE_NEED_MATCH means need to find match
static const unsigned char windows_vk_conversions[256] = {
    X_KEYCODE_NO_MATCH,     // 0 I don't know
    X_KEYCODE_NO_MATCH,     // 1 VK_LBUTTON left mouse button
    X_KEYCODE_NO_MATCH,     // 2 VK_RBUTTON right mouse button
    X_KEYCODE_CANCEL,       // 3 VK_Cancel control-break processing
    X_KEYCODE_NO_MATCH,     // 4 VK_MBUTTON middle mouse button
    X_KEYCODE_NO_MATCH,     // 5 VK_XBUTTON1 x1 mouse button
    X_KEYCODE_NO_MATCH,     // 6 VK_XBUTTON2 x2 mouse button
    X_KEYCODE_NO_MATCH,     // 7 Undefined
    X_KEYCODE_BACKSPACE,    // 8 VK_BACK BACKSPACE key
    X_KEYCODE_L_TAB,        // 9 VK_TAB TAB key
    X_KEYCODE_NO_MATCH,     // 10 Reserved
    X_KEYCODE_NO_MATCH,     // 11 Reserved
    X_KEYCODE_NEED_MATCH,   // 12 VK_CLEAR CLEAR key ??? unsure
    X_KEYCODE_ENTER,        // 13 VK_RETURN ENTER key
    X_KEYCODE_NO_MATCH,     // 14 Undefined
    X_KEYCODE_NO_MATCH,     // 15 Undefined
    X_KEYCODE_L_SHIFT,      // 16 VK_SHIFT SHIFT key
    X_KEYCODE_L_CTRL,       // 17 VK_CONTROL CTRL key
    X_KEYCODE_L_ALT,        // 18 VK_MENU ALT key
    X_KEYCODE_PAUSE,        // 19 VK_PAUSE
    X_KEYCODE_NEED_MATCH,   // 20 VK_CAPITAL
    X_KEYCODE_HANGUL,       // 21 VK_KANA/VK_HANGUEL/VK_HANGUL IME Kana mode
    X_KEYCODE_NO_MATCH,     // 22 Undefined
    X_KEYCODE_NEED_MATCH,   // 23 VK_JUNJA IME Junja mode
    X_KEYCODE_NEED_MATCH,   // 24 VK_FINAL IME final mode
    X_KEYCODE_HANGUL_HANJA, // 25 VK_HANJA/VK_KANJI IME Hanja mode, IME Kanji mode
    X_KEYCODE_NO_MATCH,     // 26 Undefined
    X_KEYCODE_ESCAPE,       // 27 VK_ESCAPE ESC key
    X_KEYCODE_NEED_MATCH,   // 28 VK_CONVERT IME convert
    X_KEYCODE_NEED_MATCH,   // 29 VK_NONCONVERT IME nonconvert
    X_KEYCODE_NEED_MATCH,   // 30 VK_ACCEPT IME accept
    X_KEYCODE_NEED_MATCH,   // 31 VK_MODECHANGE IME mode change request
    X_KEYCODE_SPACE,        // 32 VK_SPACE SPACEBAR
    X_KEYCODE_PAGEUP,       // 33 VK_PRIOR PAGE UP key
    X_KEYCODE_PAGEDOWN,     // 34 VK_NEXT PAGE DOWN key
    X_KEYCODE_END,          // 35 VK_END END key
    X_KEYCODE_HOME,         // 36 VK_HOME HOME key
    X_KEYCODE_LEFT,         // 37 VK_LEFT LEFT ARROW key
    X_KEYCODE_UP,           // 38 VK_UP UP ARROW key
    X_KEYCODE_RIGHT,        // 39 VK_RIGHT RIGHT ARROW key
    X_KEYCODE_DOWN,         // 40 VK_DOWN DOWN ARROW key
    X_KEYCODE_NEED_MATCH,   // 41 VK_SELECT SELECT key
    X_KEYCODE_PRINT,        // 42 VK_PRINT PRINT key
    X_KEYCODE_NEED_MATCH,   // 43 VK_EXECUTE EXECUTE key
    X_KEYCODE_PRINTSCREEN,  // 44 VK_SNAPSHOT PRINT SCREEN key
    X_KEYCODE_INSERT,       // 45 VK_INSERT INS key
    X_KEYCODE_DELETE,       // 46 VK_DELETE DEL key
    X_KEYCODE_HELP,         // 47 VK_HELP HELP key
    X_KEYCODE_0,            // 48 VK_KEY_0 0 key
    X_KEYCODE_1,            // 49 VK_KEY_1 1 key
    X_KEYCODE_2,            // 50 VK_KEY_2 2 key
    X_KEYCODE_3,            // 51 VK_KEY_3 3 key
    X_KEYCODE_4,            // 52 VK_KEY_4 4 key
    X_KEYCODE_5,            // 53 VK_KEY_5 5 key
    X_KEYCODE_6,            // 54 VK_KEY_6 6 key
    X_KEYCODE_7,            // 55 VK_KEY_7 7 key
    X_KEYCODE_8,            // 56 VK_KEY_8 8 key
    X_KEYCODE_9,            // 57 VK_KEY_9 9 key
    X_KEYCODE_NO_MATCH,     // 58 Undefined
    X_KEYCODE_NO_MATCH,     // 59 Undefined
    X_KEYCODE_NO_MATCH,     // 60 Undefined
    X_KEYCODE_NO_MATCH,     // 61 Undefined
    X_KEYCODE_NO_MATCH,     // 62 Undefined
    X_KEYCODE_NO_MATCH,     // 63 Undefined
    X_KEYCODE_NO_MATCH,     // 64 Undefined
    X_KEYCODE_A,            // 65 VK_KEY_A A key
    X_KEYCODE_B,            // 66 VK_KEY_B B key
    X_KEYCODE_C,            // 67 VK_KEY_C C key
    X_KEYCODE_D,            // 68 VK_KEY_D D key
    X_KEYCODE_E,            // 69 VK_KEY_E E key
    X_KEYCODE_F,            // 70 VK_KEY_F F key
    X_KEYCODE_G,            // 71 VK_KEY_G G key
    X_KEYCODE_H,            // 72 VK_KEY_H H key
    X_KEYCODE_I,            // 73 VK_KEY_I I key
    X_KEYCODE_J,            // 74 VK_KEY_J J key
    X_KEYCODE_K,            // 75 VK_KEY_K K key
    X_KEYCODE_L,            // 76 VK_KEY_L L key
    X_KEYCODE_M,            // 77 VK_KEY_M M key
    X_KEYCODE_N,            // 78 VK_KEY_N N key
    X_KEYCODE_O,            // 79 VK_KEY_O O key
    X_KEYCODE_P,            // 80 VK_KEY_P P key
    X_KEYCODE_Q,            // 81 VK_KEY_Q Q key
    X_KEYCODE_R,            // 82 VK_KEY_R R key
    X_KEYCODE_S,            // 83 VK_KEY_S S key
    X_KEYCODE_T,            // 84 VK_KEY_T T key
    X_KEYCODE_U,            // 85 VK_KEY_U U key
    X_KEYCODE_V,            // 86 VK_KEY_V V key
    X_KEYCODE_W,            // 87 VK_KEY_W W key
    X_KEYCODE_X,            // 88 VK_KEY_X X key
    X_KEYCODE_Y,            // 89 VK_KEY_Y Y key
    X_KEYCODE_Z,            // 90 VK_KEY_Z Z key
    X_KEYCODE_L_SUPER,      // 91 VK_LWIN Left Windows key
    X_KEYCODE_R_SUPER,      // 92 VK_RWIN Right Windows Key
    X_KEYCODE_MENU,         // 93 VK_APPS Applications key ??? Unsure
    X_KEYCODE_NO_MATCH,     // 94 Reserved
    X_KEYCODE_SLEEP,        // 95 VK_SLEEP Computer Sleep key
    X_KEYCODE_KP_0,         // 96 VK_NUMPAD0 Numeric keypad 0 key
    X_KEYCODE_KP_1,         // 97 VK_NUMPAD1 Numeric keypad 1 key
    X_KEYCODE_KP_2,         // 98 VK_NUMPAD2 Numeric keypad 2 key
    X_KEYCODE_KP_3,         // 99 VK_NUMPAD3 Numeric keypad 3 key
    X_KEYCODE_KP_4,         // 100 VK_NUMPAD4 Numeric keypad 4 key
    X_KEYCODE_KP_5,         // 101 VK_NUMPAD5 Numeric keypad 5 key
    X_KEYCODE_KP_6,         // 102 VK_NUMPAD6 Numeric keypad 6 key
    X_KEYCODE_KP_7,         // 103 VK_NUMPAD7 Numeric keypad 7 key
    X_KEYCODE_KP_8,         // 104 VK_NUMPAD8 Numeric keypad 8 key
    X_KEYCODE_KP_9,         // 105 VK_NUMPAD9 Numeric keypad 9 key
    X_KEYCODE_KP_MULTIPLY,  // 106 VK_MULTIPLY Multiply key
    X_KEYCODE_KP_ADD,       // 107 VK_ADD Add key
    X_KEYCODE_NEED_MATCH,   // 108 VK_SEPARATOR Separator key
    X_KEYCODE_KP_SUBTRACT,  // 109 VK_SUBTRACT Subtract key
    X_KEYCODE_KP_PERIOD,    // 110 VK_DECIMAL Decimal key
    X_KEYCODE_KP_DIVIDE,    // 111 VK_DIVIDE Divide key
    X_KEYCODE_F1,           // 112 VK_F1 F1 key
    X_KEYCODE_F2,           // 113 VK_F2 F2 key
    X_KEYCODE_F3,           // 114 VK_F3 F3 key
    X_KEYCODE_F4,           // 115 VK_F4 F4 key
    X_KEYCODE_F5,           // 116 VK_F5 F5 key
    X_KEYCODE_F6,           // 117 VK_F6 F6 key
    X_KEYCODE_F7,           // 118 VK_F7 F7 key
    X_KEYCODE_F8,           // 119 VK_F8 F8 key
    X_KEYCODE_F9,           // 120 VK_F9 F9 key
    X_KEYCODE_F10,          // 121 VK_F10 F10 key
    X_KEYCODE_F11,          // 122 VK_F11 F11 key
    X_KEYCODE_F12,          // 123 VK_F12 F12 key
    X_KEYCODE_NEED_MATCH,   // 124 VK_F13 F13 key
    X_KEYCODE_NEED_MATCH,   // 125 VK_F14 F14 key
    X_KEYCODE_NEED_MATCH,   // 126 VK_F15 F15 key
    X_KEYCODE_NEED_MATCH,   // 127 VK_F16 F16 key
    X_KEYCODE_NEED_MATCH,   // 128 VK_F17 F17 key
    X_KEYCODE_NEED_MATCH,   // 129 VK_F18 F18 key
    X_KEYCODE_NEED_MATCH,   // 130 VK_F19 F19 key
    X_KEYCODE_NEED_MATCH,   // 131 VK_F20 F20 key
    X_KEYCODE_NEED_MATCH,   // 132 VK_F21 F21 key
    X_KEYCODE_NEED_MATCH,   // 133 VK_F22 F22 key
    X_KEYCODE_NEED_MATCH,   // 134 VK_F23 F23 key
    X_KEYCODE_NEED_MATCH,   // 135 VK_F24 F24 key
    X_KEYCODE_NO_MATCH,     // 136 Unassigned
    X_KEYCODE_NO_MATCH,     // 137 Unassigned
    X_KEYCODE_NO_MATCH,     // 138 Unassigned
    X_KEYCODE_NO_MATCH,     // 139 Unassigned
    X_KEYCODE_NO_MATCH,     // 140 Unassigned
    X_KEYCODE_NO_MATCH,     // 141 Unassigned
    X_KEYCODE_NO_MATCH,     // 142 Unassigned
    X_KEYCODE_NO_MATCH,     // 143 Unassigned
    X_KEYCODE_NUMLOCK,      // 144 VK_NUMLOCK NUM LOCK key
    X_KEYCODE_SCROLLLOCK,   // 145 VK_SCROLL SCROLL LOCK key
    X_KEYCODE_NO_MATCH,     // 146 OEM specific
    X_KEYCODE_NO_MATCH,     // 147 OEM specific
    X_KEYCODE_NO_MATCH,     // 148 OEM specific
    X_KEYCODE_NO_MATCH,     // 149 OEM specific
    X_KEYCODE_NO_MATCH,     // 150 OEM specific
    X_KEYCODE_NO_MATCH,     // 151 Unassigned
    X_KEYCODE_NO_MATCH,     // 152 Unassigned
    X_KEYCODE_NO_MATCH,     // 153 Unassigned
    X_KEYCODE_NO_MATCH,     // 154 Unassigned
    X_KEYCODE_NO_MATCH,     // 155 Unassigned
    X_KEYCODE_NO_MATCH,     // 156 Unassigned
    X_KEYCODE_NO_MATCH,     // 157 Unassigned
    X_KEYCODE_NO_MATCH,     // 158 Unassigned
    X_KEYCODE_NO_MATCH,     // 159 Unassigned
    X_KEYCODE_L_SHIFT,      // 160 VK_LSHIFT Left SHIFT key
    X_KEYCODE_R_SHIFT,      // 161 VK_RSHIFT Right SHIFT key
    X_KEYCODE_L_CTRL,       // 162 VK_LCONTROL Left CONTROL key
    X_KEYCODE_R_CTRL,       // 163 VK_RCONTROL Right CONTROL key
    X_KEYCODE_L_ALT,        // 164 VK_LMENU Left MENU key
    X_KEYCODE_R_ALT,        // 165 VK_RMENU Right MENU key
    X_KEYCODE_BACK,         // 166 VK_BROWSER_BACK Browser Back key ??? unsure
    X_KEYCODE_FORWARD,      // 167 VK_BROWSER_FORWARD Browser Forward key ??? unsure
    X_KEYCODE_RELOAD,       // 168 VK_BROWSER_REFRESH Browser Refresh key ??? unsure
    X_KEYCODE_NEED_MATCH,   // 169 VK_BROWSER_STOP Browser Stop Key
    X_KEYCODE_SEARCH,       // 170 VK_BROWSER_SEARCH Browser Search key
    X_KEYCODE_FAVORITES,    // 171 VK_BROWSER_FAVORITES Browser Favorites key ??? unsure
    X_KEYCODE_HOMEPAGE,     // 172 VK_BROWSER_HOME Browser Start and Home key
    X_KEYCODE_AUDIOMUTE,    // 173 VK_VOLUME_MUTE Volume Mute key
    X_KEYCODE_AUDIOLOWER,   // 174 VK_VOLUME_DOWN Volume Down key
    X_KEYCODE_AUDIORAISE,   // 175 VK_VOLUME_UP Volume Up key
    X_KEYCODE_AUDIONEXT,    // 176 VK_MEDIA_NEXT_TRACK Next Track key ??? unsure
    X_KEYCODE_AUDIOPREV,    // 177 VK_MEDIA_PREV_TRACK Previous Track key ??? unsure
    X_KEYCODE_AUDIOSTOP,    // 178 VK_MEDIA_STOP Stop Media key
    X_KEYCODE_NEED_MATCH,   // 179 VK_MEDIA_PLAY_PAUSE Play/Pause Media key
    X_KEYCODE_MAIL,         // 180 VK_LAUNCH_MAIL Start Mail key ??? unsure
    X_KEYCODE_NEED_MATCH,   // 181 VK_LAUNCH_MEDIA_SELECT Select Media key
    X_KEYCODE_LAUNCH1,      // 182 VK_LAUNCH_APP1 Start Application 1 key ??? unsure
    X_KEYCODE_LAUNCH2,      // 183 VK_LAUNCH_APP2 Start Application 2 key ??? unsure
    X_KEYCODE_NO_MATCH,     // 184 Reserved
    X_KEYCODE_NO_MATCH,     // 185 Reserved
    X_KEYCODE_SEMICOLON,    // 186 VK_OEM_1 For the US standard keyboard, the ';:' key
    X_KEYCODE_EQUALS,       // 187 VK_OEM_PLUS For any country/region, the '+'' key ??? unsure
    X_KEYCODE_COMMA,        // 188 VK_OEM_COMMA For any country/region, the ',' key ??? unsure
    X_KEYCODE_MINUS,        // 189 VK_OEM_MINUS For any country/region, the '-' key ??? unsure
    X_KEYCODE_PERIOD,       // 190 VK_OEM_PERIOD For any country/region, the '.' key ??? unsure
    X_KEYCODE_SLASH,        // 191 VK_OEM_2 For the US standard keyboard, the '/?' key
    X_KEYCODE_GRAVE,        // 192 VK_OEM_3 For the US standard keyboard, the '`~' key
    X_KEYCODE_NO_MATCH,     // 193 Reserved
    X_KEYCODE_NO_MATCH,     // 194 Reserved
    X_KEYCODE_NO_MATCH,     // 195 Reserved
    X_KEYCODE_NO_MATCH,     // 196 Reserved
    X_KEYCODE_NO_MATCH,     // 197 Reserved
    X_KEYCODE_NO_MATCH,     // 198 Reserved
    X_KEYCODE_NO_MATCH,     // 199 Reserved
    X_KEYCODE_NO_MATCH,     // 200 Reserved
    X_KEYCODE_NO_MATCH,     // 201 Reserved
    X_KEYCODE_NO_MATCH,     // 202 Reserved
    X_KEYCODE_NO_MATCH,     // 203 Reserved
    X_KEYCODE_NO_MATCH,     // 204 Reserved
    X_KEYCODE_NO_MATCH,     // 205 Reserved
    X_KEYCODE_NO_MATCH,     // 206 Reserved
    X_KEYCODE_NO_MATCH,     // 207 Reserved
    X_KEYCODE_NO_MATCH,     // 208 Reserved
    X_KEYCODE_NO_MATCH,     // 209 Reserved
    X_KEYCODE_NO_MATCH,     // 210 Reserved
    X_KEYCODE_NO_MATCH,     // 211 Reserved
    X_KEYCODE_NO_MATCH,     // 212 Reserved
    X_KEYCODE_NO_MATCH,     // 213 Reserved
    X_KEYCODE_NO_MATCH,     // 214 Reserved
    X_KEYCODE_NO_MATCH,     // 215 Reserved
    X_KEYCODE_NO_MATCH,     // 216 Unassigned
    X_KEYCODE_NO_MATCH,     // 217 Unassigned
    X_KEYCODE_NO_MATCH,     // 218 Unassigned
    X_KEYCODE_BRACKET_L,    // 219 VK_OEM_4 For the US standard keyboard, the '[{' key
    X_KEYCODE_BACKSLASH,    // 220 VK_OEM_5 For the US standard keyboard, the '\|' key
    X_KEYCODE_BRACKET_R,    // 221 VK_OEM_6 For the US standard keyboard, the ']}' key
    X_KEYCODE_QUOTE,        // 222 VK_OEM_7 For the US standard keyboard, the '\'"" key
    X_KEYCODE_NEED_MATCH,   // 223 VK_OEM_8 (void)
    X_KEYCODE_NO_MATCH,     // 224 Reserved
    X_KEYCODE_NO_MATCH,     // 225 OEM specific
    X_KEYCODE_LESS_GREATER, // 226 VK_OEM_102 Either the angle bracket key or the backslash key on RT 102-key keyboard ??? unsure
    X_KEYCODE_NO_MATCH,     // 227 OEM specific
    X_KEYCODE_NO_MATCH,     // 228 OEM specific
    X_KEYCODE_NEED_MATCH,   // 229 IME PROCESS key
    X_KEYCODE_NO_MATCH,     // 230 OEM specific
    X_KEYCODE_NEED_MATCH,   // 231 VK_PACKET Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, See Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
    X_KEYCODE_NO_MATCH,     // 232 Unassigned
    X_KEYCODE_NO_MATCH,     // 233 OEM specific
    X_KEYCODE_NO_MATCH,     // 234 OEM specific
    X_KEYCODE_NO_MATCH,     // 235 OEM specific
    X_KEYCODE_NO_MATCH,     // 236 OEM specific
    X_KEYCODE_NO_MATCH,     // 237 OEM specific
    X_KEYCODE_NO_MATCH,     // 238 OEM specific
    X_KEYCODE_NO_MATCH,     // 239 OEM specific
    X_KEYCODE_NO_MATCH,     // 240 OEM specific
    X_KEYCODE_NO_MATCH,     // 241 OEM specific
    X_KEYCODE_NO_MATCH,     // 242 OEM specific
    X_KEYCODE_NO_MATCH,     // 243 OEM specific
    X_KEYCODE_NO_MATCH,     // 244 OEM specific
    X_KEYCODE_NO_MATCH,     // 245 OEM specific
    X_KEYCODE_NEED_MATCH,   // 246 VK_ATTN Attn key
    X_KEYCODE_NEED_MATCH,   // 247 VK_CRSEL CrSel key
    X_KEYCODE_NEED_MATCH,   // 248 VK_EXSEL ExSel key
    X_KEYCODE_NEED_MATCH,   // 249 VK_EREOF Erase EOF key
    X_KEYCODE_AUDIOPLAY,    // 250 VK_PLAY Play key
    X_KEYCODE_NEED_MATCH,   // 251 VK_ZOOM Zoom key
    X_KEYCODE_NO_MATCH,     // 252 VK_NONAME Reserved for future use
    X_KEYCODE_NEED_MATCH,   // 253 VK_PA1 PA1 key
    X_KEYCODE_NEED_MATCH,   // 254 VK_OEM_CLEAR Clear key
    X_KEYCODE_NEED_MATCH,   // 255 end of list
};

const char* x_keynames[256] = {
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "escape",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "minus",
    "equal",
    "backspace",
    "tab",
    "q",
    "w",
    "e",
    "r",
    "t",
    "y",
    "u",
    "i",
    "o",
    "p",
    "bracketleft",
    "bracketright",
    "return",
    "control_l",
    "a",
    "s",
    "d",
    "f",
    "g",
    "h",
    "j",
    "k",
    "l",
    "semicolon",
    "apostrophe",
    "grave",
    "shift_l",
    "backslash",
    "z",
    "x",
    "c",
    "v",
    "b",
    "n",
    "m",
    "comma",
    "period",
    "slash",
    "shift_r",
    "kp_multiply",
    "alt_l",
    "space",
    "caps_lock",
    "f1",
    "f2",
    "f3",
    "f4",
    "f5",
    "f6",
    "f7",
    "f8",
    "f9",
    "f10",
    "num_lock",
    "scroll_lock",
    "kp_home",
    "kp_up",
    "kp_prior",
    "kp_subtract",
    "kp_left",
    "kp_begin",
    "kp_right",
    "kp_add",
    "kp_end",
    "kp_down",
    "kp_next",
    "kp_insert",
    "kp_delete",
    "iso_level3_shift",
    "nosymbol",
    "less", //less_greater
    "f11",
    "f12",
    "nosymbol",
    "katakana",
    "hiragana",
    "henkan_mode",
    "hiragana_katakana",
    "muhenkan",
    "noSymbol",
    "kp_enter",
    "control_r",
    "kp_divide",
    "print",
    "alt_r",
    "linefeed",
    "home",
    "up",
    "prior",
    "left",
    "right",
    "end",
    "down",
    "next",
    "insert",
    "delete",
    "nosymbol",
    "xf86audiomute",
    "xf86audiolowervolume",
    "xf86audioraisevolume",
    "xf86poweroff",
    "kp_equal",
    "plusminus",
    "pause",
    "nosymbol",
    "kp_decimal",
    "hangul",
    "hangul_hanja",
    "nosymbol",
    "super_l",
    "super_r",
    "menu",
    "cancel",
    "redo",
    "sunprops",
    "undo",
    "sunfront",
    "xf86copy",
    "sunopen",
    "xf86paste",
    "find",
    "xf86cut",
    "help",
    "xf86menukb",
    "xf86calculator",
    "nosymbol",
    "xf86sleep",
    "xf86wakeup",
    "xf86explorer",
    "xf86send",
    "nosymbol",
    "xf86xfer",
    "xf86launch1",
    "xf86launch2",
    "xf86www",
    "xf86dos",
    "xf86screensaver",
    "nosymbol",
    "xf86rotatewindows",
    "xf86mail",
    "xf86favorites",
    "xf86mycomputer",
    "xf86back",
    "xf86forward",
    "nosymbol",
    "xf86eject",
    "xf86eject",
    "xf86audionext",
    "xf86audioplay",
    "xf86audioprev",
    "xf86audiostop",
    "xf86audiorecord",
    "xf86audiorewind",
    "xf86phone",
    "nosymbol",
    "xf86tools",
    "xf86homepage",
    "xf86reload",
    "xf86close",
    "nosymbol",
    "nosymbol",
    "xf86scrollup",
    "xf86scrolldown",
    "parenleft",
    "parenright",
    "xf86new",
    "redo",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "xf86touchpadtoggle",
    "nosymbol",
    "nosymbol",
    "mode_switch",
    "alt_l",
    "meta_l",
    "super_l",
    "hyper_l",
    "xf86audioplay",
    "xf86audiopause",
    "xf86launch3",
    "xf86launch4",
    "nosymbol",
    "xf86suspend",
    "xf86close",
    "xf86audioplay",
    "xf86audioforward",
    "nosymbol",
    "print",
    "nosymbol",
    "xf86webcam",
    "nosymbol",
    "nosymbol",
    "xf86mail",
    "nosymbol",
    "xf86search",
    "nosymbol",
    "xf86finance",
    "nosymbol",
    "xf86shop",
    "nosymbol",
    "cancel",
    "xf86monbrightnessdown",
    "xf86monbrightnessup",
    "xf86audiomedia",
    "xf86display",
    "xf86kbdlightonoff",
    "xf86kbdbrightnessdown",
    "xf86kbdbrightnessup",
    "xf86send",
    "xf86reply",
    "xf86mailforward",
    "xf86save",
    "xf86documents",
    "xf86battery",
    "xf86bluetooth",
    "xf86wlan",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "nosymbol",
    "xf86wwan",
    "xf86rfkill",
};



const unsigned char windows_vk_to_linux_keycode(const int vk_code)
{
    if (vk_code > 0xFF) {
        return X_KEYCODE_NO_MATCH;
    }
    return windows_vk_conversions[(const unsigned char)vk_code];
}


const unsigned char linux_keycode_to_windows_vk(const int key_code)
{
    if (key_code > 0xFF) {
        return X_KEYCODE_NO_MATCH;
    }
    int i = 0;
    for (i=0; i<0xFF; i++) {
        if (windows_vk_conversions[i] == (const unsigned char)key_code) {
            return i;
        }
    }

    fprintf(stderr, "[-] TODO: find keycode: %d\n", key_code);
    return X_KEYCODE_NO_MATCH;
}


static int keycomp(const char* keyname, const char* str)
{
    int i = 0;
    do {
        if (str[i] <= 'Z' && str[i] >= 'A') {
            if (keyname[i] != str[i]+0x20) {
                return 0;
            }
        } else if (keyname[i] != str[i]) {
            return 0; 
        }
    } while (keyname[i++] != '\0');

    return 1;
}


const unsigned char key_name_to_keycode(const char *keyname)
{
    int i = 0;
    for (i=0; i<256; i++) {
        if (keycomp(x_keynames[i], keyname)) {
            return i;
        }
    }

    fprintf(stderr, "[-] Could not find keycode for '%s'", keyname);
    return -1;
}
