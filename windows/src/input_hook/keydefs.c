#include "keydefs.h"
//#include <Windows.h>
// From https://raw.githubusercontent.com/torvalds/linux/
// include/uapi/linux/input-event-codes.h

/*
 * Keys and buttons
 *
 * Most of the keys/buttons are modeled after USB HUT 1.12
 * (see http://www.usb.org/developers/hidpage).
 * Abbreviations in the comments:
 * AC - Application Control
 * AL - Application Launch Button
 * SC - System Control
 */

#define KEY_RESERVED    0
#define KEY_ESC         1
#define KEY_1           2
#define KEY_2           3
#define KEY_3           4
#define KEY_4           5
#define KEY_5           6
#define KEY_6           7
#define KEY_7           8
#define KEY_8           9
#define KEY_9           10
#define KEY_0           11
#define KEY_MINUS       12
#define KEY_EQUAL       13
#define KEY_BACKSPACE   14
#define KEY_TAB         15
#define KEY_Q           16
#define KEY_W           17
#define KEY_E           18
#define KEY_R           19
#define KEY_T           20
#define KEY_Y           21
#define KEY_U           22
#define KEY_I           23
#define KEY_O           24
#define KEY_P           25
#define KEY_LEFTBRACE   26
#define KEY_RIGHTBRACE  27
#define KEY_ENTER       28
#define KEY_LEFTCTRL    29
#define KEY_A           30
#define KEY_S           31
#define KEY_D           32
#define KEY_F           33
#define KEY_G           34
#define KEY_H           35
#define KEY_J           36
#define KEY_K           37
#define KEY_L           38
#define KEY_SEMICOLON   39
#define KEY_APOSTROPHE  40
#define KEY_GRAVE       41
#define KEY_LEFTSHIFT   42
#define KEY_BACKSLASH   43
#define KEY_Z           44
#define KEY_X           45
#define KEY_C           46
#define KEY_V           47
#define KEY_B           48
#define KEY_N           49
#define KEY_M           50
#define KEY_COMMA       51
#define KEY_DOT         52
#define KEY_SLASH       53
#define KEY_RIGHTSHIFT  54
#define KEY_KPASTERISK  55
#define KEY_LEFTALT     56
#define KEY_SPACE       57
#define KEY_CAPSLOCK    58
#define KEY_F1          59
#define KEY_F2          60
#define KEY_F3          61
#define KEY_F4          62
#define KEY_F5          63
#define KEY_F6          64
#define KEY_F7          65
#define KEY_F8          66
#define KEY_F9          67
#define KEY_F10         68
#define KEY_NUMLOCK     69
#define KEY_SCROLLLOCK  70
#define KEY_KP7         71
#define KEY_KP8         72
#define KEY_KP9         73
#define KEY_KPMINUS     74
#define KEY_KP4         75
#define KEY_KP5         76
#define KEY_KP6         77
#define KEY_KPPLUS      78
#define KEY_KP1         79
#define KEY_KP2         80
#define KEY_KP3         81
#define KEY_KP0         82
#define KEY_KPDOT       83

#define KEY_ZENKAKUHANKAKU   85
#define KEY_102ND            86
#define KEY_F11              87
#define KEY_F12              88
#define KEY_RO               89
#define KEY_KATAKANA         90
#define KEY_HIRAGANA         91
#define KEY_HENKAN           92
#define KEY_KATAKANAHIRAGANA 93
#define KEY_MUHENKAN         94
#define KEY_KPJPCOMMA        95
#define KEY_KPENTER          96
#define KEY_RIGHTCTRL        97
#define KEY_KPSLASH          98
#define KEY_SYSRQ            99
#define KEY_RIGHTALT         100
#define KEY_LINEFEED         101
#define KEY_HOME             102
#define KEY_UP               103
#define KEY_PAGEUP           104
#define KEY_LEFT             105
#define KEY_RIGHT            106
#define KEY_END              107
#define KEY_DOWN             108
#define KEY_PAGEDOWN         109
#define KEY_INSERT           110
#define KEY_DELETE           111
#define KEY_MACRO            112
#define KEY_MUTE             113
#define KEY_VOLUMEDOWN       114
#define KEY_VOLUMEUP         115
#define KEY_POWER            116 /* SC System Power Down */
#define KEY_KPEQUAL          117
#define KEY_KPPLUSMINUS      118
#define KEY_PAUSE            119
#define KEY_SCALE            120 /* AL Compiz Scale (Expose) */

#define KEY_KPCOMMA          121
#define KEY_HANGEUL          122
#define KEY_HANGUEL          KEY_HANGEUL
#define KEY_HANJA            123
#define KEY_YEN              124
#define KEY_LEFTMETA         125
#define KEY_RIGHTMETA        126
#define KEY_COMPOSE          127

#define KEY_STOP            128 /* AC Stop */
#define KEY_AGAIN           129
#define KEY_PROPS           130 /* AC Properties */
#define KEY_UNDO            131 /* AC Undo */
#define KEY_FRONT           132
#define KEY_COPY            133 /* AC Copy */
#define KEY_OPEN            134 /* AC Open */
#define KEY_PASTE           135 /* AC Paste */
#define KEY_FIND            136 /* AC Search */
#define KEY_CUT             137 /* AC Cut */
#define KEY_HELP            138 /* AL Integrated Help Center */
#define KEY_MENU            139 /* Menu (show menu) */
#define KEY_CALC            140 /* AL Calculator */
#define KEY_SETUP           141
#define KEY_SLEEP           142 /* SC System Sleep */
#define KEY_WAKEUP          143 /* System Wake Up */
#define KEY_FILE            144 /* AL Local Machine Browser */
#define KEY_SENDFILE        145
#define KEY_DELETEFILE      146
#define KEY_XFER            147
#define KEY_PROG1           148
#define KEY_PROG2           149
#define KEY_WWW             150 /* AL Internet Browser */
#define KEY_MSDOS           151
#define KEY_COFFEE          152 /* AL Terminal Lock/Screensaver */
#define KEY_SCREENLOCK      KEY_COFFEE
#define KEY_ROTATE_DISPLAY  153 /* Display orientation for e.g. tablets */
#define KEY_DIRECTION       KEY_ROTATE_DISPLAY
#define KEY_CYCLEWINDOWS    154
#define KEY_MAIL            155
#define KEY_BOOKMARKS       156 /* AC Bookmarks */
#define KEY_COMPUTER        157
#define KEY_BACK            158 /* AC Back */
#define KEY_FORWARD         159 /* AC Forward */
#define KEY_CLOSECD         160
#define KEY_EJECTCD         161
#define KEY_EJECTCLOSECD    162
#define KEY_NEXTSONG        163
#define KEY_PLAYPAUSE       164
#define KEY_PREVIOUSSONG    165
#define KEY_STOPCD          166
#define KEY_RECORD          167
#define KEY_REWIND          168
#define KEY_PHONE           169 /* Media Select Telephone */
#define KEY_ISO             170
#define KEY_CONFIG          171 /* AL Consumer Control Configuration */
#define KEY_HOMEPAGE        172 /* AC Home */
#define KEY_REFRESH         173 /* AC Refresh */
#define KEY_EXIT            174 /* AC Exit */
#define KEY_MOVE            175
#define KEY_EDIT            176
#define KEY_SCROLLUP        177
#define KEY_SCROLLDOWN      178
#define KEY_KPLEFTPAREN     179
#define KEY_KPRIGHTPAREN    180
#define KEY_NEW             181 /* AC New */
#define KEY_REDO            182 /* AC Redo/Repeat */

#define KEY_F13         183
#define KEY_F14         184
#define KEY_F15         185
#define KEY_F16         186
#define KEY_F17         187
#define KEY_F18         188
#define KEY_F19         189
#define KEY_F20         190
#define KEY_F21         191
#define KEY_F22         192
#define KEY_F23         193
#define KEY_F24         194

#define KEY_PLAYCD          200
#define KEY_PAUSECD         201
#define KEY_PROG3           202
#define KEY_PROG4           203
#define KEY_DASHBOARD       204 /* AL Dashboard */
#define KEY_SUSPEND         205
#define KEY_CLOSE           206 /* AC Close */
#define KEY_PLAY            207
#define KEY_FASTFORWARD     208
#define KEY_BASSBOOST       209
#define KEY_PRINT           210 /* AC Print */
#define KEY_HP              211
#define KEY_CAMERA          212
#define KEY_SOUND           213
#define KEY_QUESTION        214
#define KEY_EMAIL           215
#define KEY_CHAT            216
#define KEY_SEARCH          217
#define KEY_CONNECT         218
#define KEY_FINANCE         219 /* AL Checkbook/Finance */
#define KEY_SPORT           220
#define KEY_SHOP            221
#define KEY_ALTERASE        222
#define KEY_CANCEL          223 /* AC Cancel */
#define KEY_BRIGHTNESSDOWN  224
#define KEY_BRIGHTNESSUP    225
#define KEY_MEDIA           226

#define KEY_SWITCHVIDEOMODE 227 /* Cycle between available video
                                   outputs (Monitor/LCD/TV-out/etc) */
#define KEY_KBDILLUMTOGGLE  228
#define KEY_KBDILLUMDOWN    229
#define KEY_KBDILLUMUP      230

#define KEY_SEND        231 /* AC Send */
#define KEY_REPLY       232 /* AC Reply */
#define KEY_FORWARDMAIL 233 /* AC Forward Msg */
#define KEY_SAVE        234 /* AC Save */
#define KEY_DOCUMENTS   235

#define KEY_BATTERY     236

#define KEY_BLUETOOTH   237
#define KEY_WLAN        238
#define KEY_UWB         239

#define KEY_UNKNOWN     240

#define KEY_VIDEO_NEXT          241 /* drive next video source */
#define KEY_VIDEO_PREV          242 /* drive previous video source */
#define KEY_BRIGHTNESS_CYCLE    243 /* brightness up, after max is min */
#define KEY_BRIGHTNESS_AUTO     244 /* Set Auto Brightness: manual
                                       brightness control is off,
                                       rely on ambient */
#define KEY_BRIGHTNESS_ZERO     KEY_BRIGHTNESS_AUTO
#define KEY_DISPLAY_OFF         245 /* display device to off state */

#define KEY_WWAN        246 /* Wireless WAN (LTE, UMTS, GSM, etc.) */
#define KEY_WIMAX       KEY_WWAN
#define KEY_RFKILL      247 /* Key that controls all radios */

#define KEY_MICMUTE     248 /* Mute / unmute the microphone */


// Used http://cherrytree.at/misc/vk.htm as reference
// 0 = no match, 0xff means I need to find a match
const DWORD windows_vk_conversions[256] = {
    KEY_RESERVED,   // 0 I don't know
    0,              // 1 VK_LBUTTON left mouse button
    0,              // 2 VK_RBUTTON right mouse button
    KEY_CANCEL,     // 3 VK_Cancel control-break processing
    0,              // 4 VK_MBUTTON middle mouse button
    0,              // 5 VK_XBUTTON1 x1 mouse button
    0,              // 6 VK_XBUTTON2 x2 mouse button
    0,              // 7 Undefined
    KEY_BACKSPACE,  // 8 VK_BACK BACKSPACE key
    KEY_TAB,        // 9 VK_TAB TAB key
    0,              // 10 Reserved
    0,              // 11 Reserved
    KEY_ALTERASE,   // 12 VK_CLEAR CLEAR key ??? unsure
    KEY_ENTER,      // 13 VK_RETURN ENTER key
    0,              // 14 Undefined
    0,              // 15 Undefined
    KEY_LEFTSHIFT,  // 16 VK_SHIFT SHIFT key
    KEY_LEFTCTRL,   // 17 VK_CONTROL CTRL key
    KEY_LEFTALT,    // 18 VK_MENU ALT key
    KEY_PAUSE,      // 19 VK_PAUSE
    KEY_CAPSLOCK,   // 20 VK_CAPITAL
    KEY_HANGEUL,    // 21 VK_KANA/VK_HANGUEL/VK_HANGUL IME Kana mode ????
    0,              // 22 Undefined
    0xFF,           // 23 VK_JUNJA IME Junja mode
    0xFF,           // 24 VK_FINAL IME final mode
    KEY_HANJA,      // 25 VK_HANJA/VK_KANJI IME Hanja mode, IME Kanji mode
    0,              // 26 Undefined
    KEY_ESC,        // 27 VK_ESCAPE ESC key
    0xFF,           // 28 VK_CONVERT IME convert
    0xFF,           // 29 VK_NONCONVERT IME nonconvert
    0xFF,           // 30 VK_ACCEPT IME accept
    0xFF,           // 31 VK_MODECHANGE IME mode change request
    KEY_SPACE,      // 32 VK_SPACE SPACEBAR
    KEY_PAGEUP,     // 33 VK_PRIOR PAGE UP key
    KEY_PAGEDOWN,   // 34 VK_NEXT PAGE DOWN key
    KEY_END,        // 35 VK_END END key
    KEY_HOME,       // 36 VK_HOME HOME key
    KEY_LEFT,       // 37 VK_LEFT LEFT ARROW key
    KEY_UP,         // 38 VK_UP UP ARROW key
    KEY_RIGHT,      // 39 VK_RIGHT RIGHT ARROW key
    KEY_DOWN,       // 40 VK_DOWN DOWN ARROW key
    0xFF,           // 41 VK_SELECT SELECT key
    KEY_PRINT,      // 42 VK_PRINT PRINT key
    0xFF,           // 43 VK_EXECUTE EXECUTE key
    KEY_SYSRQ,      // 44 VK_SNAPSHOT PRINT SCREEN key
    KEY_INSERT,     // 45 VK_INSERT INS key
    KEY_DELETE,     // 46 VK_DELETE DEL key
    KEY_HELP,       // 47 VK_HELP HELP key
    KEY_0,          // 48 VK_KEY_0 0 key
    KEY_1,          // 49 VK_KEY_1 1 key
    KEY_2,          // 50 VK_KEY_2 2 key
    KEY_3,          // 51 VK_KEY_3 3 key
    KEY_4,          // 52 VK_KEY_4 4 key
    KEY_5,          // 53 VK_KEY_5 5 key
    KEY_6,          // 54 VK_KEY_6 6 key
    KEY_7,          // 55 VK_KEY_7 7 key
    KEY_8,          // 56 VK_KEY_8 8 key
    KEY_9,          // 57 VK_KEY_9 9 key
    0,              // 58 Undefined
    0,              // 59 Undefined
    0,              // 60 Undefined
    0,              // 61 Undefined
    0,              // 62 Undefined
    0,              // 63 Undefined
    0,              // 64 Undefined
    KEY_A,          // 65 VK_KEY_A A key
    KEY_B,          // 66 VK_KEY_B B key
    KEY_C,          // 67 VK_KEY_C C key
    KEY_D,          // 68 VK_KEY_D D key
    KEY_E,          // 69 VK_KEY_E E key
    KEY_F,          // 70 VK_KEY_F F key
    KEY_G,          // 71 VK_KEY_G G key
    KEY_H,          // 72 VK_KEY_H H key
    KEY_I,          // 73 VK_KEY_I I key
    KEY_J,          // 74 VK_KEY_J J key
    KEY_K,          // 75 VK_KEY_K K key
    KEY_L,          // 76 VK_KEY_L L key
    KEY_M,          // 77 VK_KEY_M M key
    KEY_N,          // 78 VK_KEY_N N key
    KEY_O,          // 79 VK_KEY_O O key
    KEY_P,          // 80 VK_KEY_P P key
    KEY_Q,          // 81 VK_KEY_Q Q key
    KEY_R,          // 82 VK_KEY_R R key
    KEY_S,          // 83 VK_KEY_S S key
    KEY_T,          // 84 VK_KEY_T T key
    KEY_U,          // 85 VK_KEY_U U key
    KEY_V,          // 86 VK_KEY_V V key
    KEY_W,          // 87 VK_KEY_W W key
    KEY_X,          // 88 VK_KEY_X X key
    KEY_Y,          // 89 VK_KEY_Y Y key
    KEY_Z,          // 90 VK_KEY_Z Z key
    KEY_LEFTMETA,   // 91 VK_LWIN Left Windows key
    KEY_RIGHTMETA,  // 92 VK_RWIN Right Windows Key
    KEY_MENU,       // 93 VK_APPS Applications key ??? Unsure
    0,              // 94 Reserved
    KEY_SLEEP,      // 95 VK_SLEEP Computer Sleep key
    KEY_KP0,        // 96 VK_NUMPAD0 Numeric keypad 0 key
    KEY_KP1,        // 97 VK_NUMPAD1 Numeric keypad 1 key
    KEY_KP2,        // 98 VK_NUMPAD2 Numeric keypad 2 key
    KEY_KP3,        // 99 VK_NUMPAD3 Numeric keypad 3 key
    KEY_KP4,        // 100 VK_NUMPAD4 Numeric keypad 4 key
    KEY_KP5,        // 101 VK_NUMPAD5 Numeric keypad 5 key
    KEY_KP6,        // 102 VK_NUMPAD6 Numeric keypad 6 key
    KEY_KP7,        // 103 VK_NUMPAD7 Numeric keypad 7 key
    KEY_KP8,        // 104 VK_NUMPAD8 Numeric keypad 8 key
    KEY_KP9,        // 105 VK_NUMPAD9 Numeric keypad 9 key
    KEY_KPASTERISK, // 106 VK_MULTIPLY Multiply key
    KEY_KPPLUS,     // 107 VK_ADD Add key
    KEY_KPCOMMA,    // 108 VK_SEPARATOR Separator key ??? Unsure
    KEY_KPMINUS,    // 109 VK_SUBTRACT Subtract key
    KEY_KPDOT,      // 110 VK_DECIMAL Decimal key
    KEY_KPSLASH,    // 111 VK_DIVIDE Divide key
    KEY_F1,         // 112 VK_F1 F1 key
    KEY_F2,         // 113 VK_F2 F2 key
    KEY_F3,         // 114 VK_F3 F3 key
    KEY_F4,         // 115 VK_F4 F4 key
    KEY_F5,         // 116 VK_F5 F5 key
    KEY_F6,         // 117 VK_F6 F6 key
    KEY_F7,         // 118 VK_F7 F7 key
    KEY_F8,         // 119 VK_F8 F8 key
    KEY_F9,         // 120 VK_F9 F9 key
    KEY_F10,        // 121 VK_F10 F10 key
    KEY_F11,        // 122 VK_F11F1 k1ey
    KEY_F12,        // 123 VK_F12 key
    KEY_F13,        // 124 VK_F13 key
    KEY_F14,        // 125 VK_F14 key
    KEY_F15,        // 126 VK_F15 key
    KEY_F16,        // 127 VK_F16 key
    KEY_F17,        // 128 VK_F17 key
    KEY_F18,        // 129 VK_F18 key
    KEY_F19,        // 130 VK_F19 key
    KEY_F20,        // 131 VK_F20 key
    KEY_F21,        // 132 VK_F21 key
    KEY_F22,        // 133 VK_F22 key
    KEY_F23,        // 134 VK_F23 key
    KEY_F24,        // 135 VK_F24 key
    0,              // 136 Unassigned
    0,              // 137 Unassigned
    0,              // 138 Unassigned
    0,              // 139 Unassigned
    0,              // 140 Unassigned
    0,              // 141 Unassigned
    0,              // 142 Unassigned
    0,              // 143 Unassigned
    KEY_NUMLOCK,    // 144 VK_NUMLOCK NUM LOCK key
    KEY_SCROLLLOCK, // 145 VK_SCROLL SCROLL LOCK key
    0,              // 146 OEM specific
    0,              // 147 OEM specific
    0,              // 148 OEM specific
    0,              // 149 OEM specific
    0,              // 150 OEM specific
    0,              // 151 Unassigned
    0,              // 152 Unassigned
    0,              // 153 Unassigned
    0,              // 154 Unassigned
    0,              // 155 Unassigned
    0,              // 156 Unassigned
    0,              // 157 Unassigned
    0,              // 158 Unassigned
    0,              // 159 Unassigned
    KEY_LEFTSHIFT,  // 160 VK_LSHIFT Left SHIFT key
    KEY_RIGHTSHIFT, // 161 VK_RSHIFT Right SHIFT key
    KEY_LEFTCTRL,   // 162 VK_LCONTROL Left CONTROL key
    KEY_RIGHTCTRL,  // 163 VK_RCONTROL Right CONTROL key
    KEY_LEFTALT,    // 164 VK_LMENU Left MENU key
    KEY_RIGHTALT,   // 165 VK_RMENU Right MENU key
    KEY_BACK,       // 166 VK_BROWSER_BACK Browser Back key ??? unsure
    KEY_FORWARD,    // 167 VK_BROWSER_FORWARD Browser Forward key ??? unsure
    KEY_REFRESH,    // 168 VK_BROWSER_REFRESH Browser Refresh key
    KEY_STOP,       // 169 VK_BROWSER_STOP Browser Stop Key
    KEY_SEARCH,     // 170 VK_BROWSER_SEARCH Browser Search key
    KEY_BOOKMARKS,  // 171 VK_BROWSER_FAVORITES Browser Favorites key ??? unsure
    KEY_HOMEPAGE,   // 172 VK_BROWSER_HOME Browser Start and Home key
    KEY_MUTE,       // 173 VK_VOLUME_MUTE Volume Mute key
    KEY_VOLUMEDOWN, // 174 VK_VOLUME_DOWN Volume Down key
    KEY_VOLUMEUP,   // 175 VK_VOLUME_UP Volume Up key
    KEY_NEXTSONG,   // 176 VK_MEDIA_NEXT_TRACK Next Track key ??? unsure
    KEY_PREVIOUSSONG,   // 177 VK_MEDIA_PREV_TRACK Previous Track key ??? unsure
    KEY_STOP,       // 178 VK_MEDIA_STOP Stop Media key
    KEY_PLAYPAUSE,  // 179 VK_MEDIA_PLAY_PAUSE Play/Pause Media key
    KEY_EMAIL,      // 180 VK_LAUNCH_MAIL Start Mail key ??? unsure
    KEY_MEDIA,      // 181 VK_LAUNCH_MEDIA_SELECT Select Media key ??? unsure
    KEY_PROG1,      // 182 VK_LAUNCH_APP1 Start Application 1 key ??? unsure
    KEY_PROG2,      // 183 VK_LAUNCH_APP2 Start Application 2 key ??? unsure
    0,              // 184 Reserved
    0,              // 185 Reserved
    KEY_SEMICOLON,  // 186 VK_OEM_1 For the US standard keyboard, the ';:' key
    KEY_EQUAL,      // 187 VK_OEM_PLUS For any country/region, the '+'' key ??? unsure
    KEY_COMMA,      // 188 VK_OEM_COMMA For any country/region, the ',' key ??? unsure
    KEY_MINUS,      // 189 VK_OEM_MINUS For any country/region, the '-' key ??? unsure
    KEY_DOT,        // 190 VK_OEM_PERIOD For any country/region, the '.' key ??? unsure
    KEY_SLASH,      // 191 VK_OEM_2 For the US standard keyboard, the '/?' key
    KEY_GRAVE,      // 192 VK_OEM_3 For the US standard keyboard, the '`~' key
    0,              // 193 Reserved
    0,              // 194 Reserved
    0,              // 195 Reserved
    0,              // 196 Reserved
    0,              // 197 Reserved
    0,              // 198 Reserved
    0,              // 199 Reserved
    0,              // 200 Reserved
    0,              // 201 Reserved
    0,              // 202 Reserved
    0,              // 203 Reserved
    0,              // 204 Reserved
    0,              // 205 Reserved
    0,              // 206 Reserved
    0,              // 207 Reserved
    0,              // 208 Reserved
    0,              // 209 Reserved
    0,              // 210 Reserved
    0,              // 211 Reserved
    0,              // 212 Reserved
    0,              // 213 Reserved
    0,              // 214 Reserved
    0,              // 215 Reserved
    0,              // 216 Unassigned
    0,              // 217 Unassigned
    0,              // 218 Unassigned
    KEY_LEFTBRACE,  // 219 VK_OEM_4 For the US standard keyboard, the '[{' key
    KEY_BACKSLASH,  // 220 VK_OEM_5 For the US standard keyboard, the '\|' key
    KEY_RIGHTBRACE, // 221 VK_OEM_6 For the US standard keyboard, the ']}' key
    KEY_APOSTROPHE, // 222 VK_OEM_7 For the US standard keyboard, the '\'"" key
    0xFF,           // 223 VK_OEM_8 (void)
    0,              // 224 Reserved
    0,              // 225 OEM specific
    KEY_102ND,      // 226 VK_OEM_102 Either the angle bracket key or the backslash key on RT 102-key keyboard ??? unsure
    0,              // 227 OEM specific
    0,              // 228 OEM specific
    0xFF,           // 229 IME PROCESS key
    0,              // 230 OEM specific
    0xFF,           // 231 VK_PACKET Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, See Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
    0,              // 232 Unassigned
    0,              // 233 OEM specific
    0,              // 234 OEM specific
    0,              // 235 OEM specific
    0,              // 236 OEM specific
    0,              // 237 OEM specific
    0,              // 238 OEM specific
    0,              // 239 OEM specific
    0,              // 240 OEM specific
    0,              // 241 OEM specific
    0,              // 242 OEM specific
    0,              // 243 OEM specific
    0,              // 244 OEM specific
    0,              // 245 OEM specific
    0xFF,           // 246 VK_ATTN Attn key
    0xFF,           // 247 VK_CRSEL CrSel key
    0xFF,           // 248 VK_EXSEL ExSel key
    0xFF,           // 249 VK_EREOF Erase EOF key
    KEY_PLAY,       // 250 VK_PLAY Play key
    0xFF,           // 251 VK_ZOOM Zoom key
    0,              // 252 VK_NONAME Reserved for future use
    0xFF,           // 253 VK_PA1 PA1 key
    0xFF,           // 254 VK_OEM_CLEAR Clear key
    0xFF,           // 255 end of list
};


unsigned char windows_vk_to_linux_keycode(DWORD vk_code)
{
    if (vk_code > 0xFF) {
        return 0xFF;
    }
    return (unsigned char)windows_vk_conversions[(unsigned char)vk_code];
}

