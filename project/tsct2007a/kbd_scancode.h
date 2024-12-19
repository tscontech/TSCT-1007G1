#ifndef __KBD_SCANCODE__
#define __KBD_SCANCODE__


#ifdef NULL
#undef NULL
#endif

#define NULL (char*)0

enum KEYCODE {
    KEYCODE_UNKNOWN_0, KEYCODE_UNKNOWN_1, KEYCODE_UNKNOWN_2, KEYCODE_UNKNOWN_3, KEYCODE_A,
    KEYCODE_B, KEYCODE_C, KEYCODE_D, KEYCODE_E, KEYCODE_F,
    KEYCODE_G, KEYCODE_H, KEYCODE_I, KEYCODE_J, KEYCODE_K,
    KEYCODE_L, KEYCODE_M, KEYCODE_N, KEYCODE_O, KEYCODE_P,
    KEYCODE_Q, KEYCODE_R, KEYCODE_S, KEYCODE_T, KEYCODE_U,

    KEYCODE_V, KEYCODE_W, KEYCODE_X, KEYCODE_Y, KEYCODE_Z,
    KEYCODE_1, KEYCODE_2, KEYCODE_3, KEYCODE_4, KEYCODE_5,
    KEYCODE_6, KEYCODE_7, KEYCODE_8, KEYCODE_9, KEYCODE_0,
    KEYCODE_RETURN, KEYCODE_ESCAPE, KEYCODE_BACKSPACE, KEYCODE_TAB, KEYCODE_SPACE,
    KEYCODE_HIPHEN_MINUS, KEYCODE_EQUAL, KEYCODE_LEFT_SQUARE_BRACKET, KEYCODE_RIGHT_SQUARE_BRACKET, KEYCODE_BACKSLASH,

    KEYCODE_SHAP, KEYCODE_SEMICOLON, KEYCODE_APOSTROPHE, KEYCODE_ACCENT, KEYCODE_COMMA,
    KEYCODE_DOT, KEYCODE_SLASH, KEYCODE_CAPSLOCK, KEYCODE_F1, KEYCODE_F2,
    KEYCODE_F3, KEYCODE_F4, KEYCODE_F5, KEYCODE_F6, KEYCODE_F7,
    KEYCODE_F8, KEYCODE_F9, KEYCODE_F10, KEYCODE_F11, KEYCODE_F12,
    KEYCODE_PRINTSCREEN, KEYCODE_SCROLLLOCK, KEYCODE_PAUSE, KEYCODE_INSERT, KEYCODE_HOME,

    KEYCODE_PAGEUP, KEYCODE_DELETE, KEYCODE_END, KEYCODE_PAGEDOWN, KEYCODE_RIGHT,
    KEYCODE_LEFT, KEYCODE_DOWN, KEYCODE_UP, KEYCODE_NUMLOCK, KEYCODE_KEYPAD_SLASH,
    KEYCODE_KEYPAD_ASTERISK, KEYCODE_KEYPAD_HIPHEN_MINUS, KEYCODE_KEYPAD_PLUS, KEYCODE_KEYPAD_ENTER, KEYCODE_KEYPAD_1,
    KEYCODE_KEYPAD_2, KEYCODE_KEYPAD_3, KEYCODE_KEYPAD_4, KEYCODE_KEYPAD_5,KEYCODE_KEYPAD_6,
    KEYCODE_KEYPAD_7, KEYCODE_KEYPAD_8, KEYCODE_KEYPAD_9, KEYCODE_KEYPAD_0, KEYCODE_KEYPAD_DOT,

    KEYCODE_UNKNOWN_100, KEYCODE_APPLICATION, KEYCODE_POWER, KEYCODE_KEYPAD_EQUAL, KEYCODE_F13,
    KEYCODE_F14, KEYCODE_F15, KEYCODE_F16, KEYCODE_F17, KEYCODE_F18,
    KEYCODE_F19, KEYCODE_F20, KEYCODE_F21, KEYCODE_F22, KEYCODE_F23,
    KEYCODE_F24, KEYCODE_EXECUTE, KEYCODE_HELP, KEYCODE_MENU, KEYCODE_SELECT,
    KEYCODE_STOP, KEYCODE_AGAIN, KEYCODE_UNDO, KEYCODE_CUT, KEYCODE_COPY,

    KEYCODE_PASTE, KEYCODE_FIND, KEYCODE_MUTE, KEYCODE_VOLUME_UP, KEYCODE_VOLUME_DOWN,
    KEYCODE_UNKNOWN_130, KEYCODE_UNKNOWN_131, KEYCODE_UNKNOWN_132, KEYCODE_KEYPAD_COMMA, KEYCODE_KEYPAD_EQUAL_AS400,
    KEYCODE_UNKNOWN_135, KEYCODE_UNKNOWN_136, KEYCODE_UNKNOWN_137, KEYCODE_UNKNOWN_138, KEYCODE_UNKNOWN_139,
    KEYCODE_UNKNOWN_140, KEYCODE_UNKNOWN_141, KEYCODE_UNKNOWN_142, KEYCODE_UNKNOWN_143, KEYCODE_UNKNOWN_144,
    KEYCODE_UNKNOWN_145, KEYCODE_UNKNOWN_146, KEYCODE_UNKNOWN_147, KEYCODE_UNKNOWN_148, KEYCODE_UNKNOWN_149,

    KEYCODE_UNKNOWN_150, KEYCODE_UNKNOWN_151, KEYCODE_UNKNOWN_152, KEYCODE_ALTERASE, KEYCODE_SYSREQ,
    KEYCODE_CANCEL, KEYCODE_CLEAR, KEYCODE_PRIOR, KEYCODE_RETURN2, KEYCODE_SEPARATOR,
    KEYCODE_OUT, KEYCODE_OPER, KEYCODE_CLEAR_AGAIN, KEYCODE_CRSEL, KEYCODE_EXSEL,
    KEYCODE_UNKNOWN_165, KEYCODE_UNKNOWN_166, KEYCODE_UNKNOWN_167, KEYCODE_UNKNOWN_168, KEYCODE_UNKNOWN_169,
    KEYCODE_UNKNOWN_170, KEYCODE_UNKNOWN_171, KEYCODE_UNKNOWN_172, KEYCODE_UNKNOWN_173, KEYCODE_UNKNOWN_174,
    
    KEYCODE_UNKNOWN_175, KEYCODE_KEYPAD_00, KEYCODE_KEYPAD_000, KEYCODE_THOUSANDS_SEPARATOR, KEYCODE_DECIMAL_SEPARATOR,
    KEYCODE_CURRENCY_UNIT, KEYCODE_CURRENCY_SUBUNIT, KEYCODE_KEYPAD_LEFT_BRACKET, KEYCODE_KEYPAD_RIGHT_BRACKET, KEYCODE_KEYPAD_LEFT_BRACES,
    KEYCODE_KEYPAD_RIGHT_BRACES, KEYCODE_KEYPAD_TAB, KEYCODE_KEYPAD_BACKSPACE, KEYCODE_KEYPAD_A, KEYCODE_KEYPAD_B,
    KEYCODE_KEYPAD_C, KEYCODE_KEYPAD_D, KEYCODE_KEYPAD_E, KEYCODE_KEYPAD_F, KEYCODE_KEYPAD_XOR,
    KEYCODE_KEYPAD_CARET, KEYCODE_KEYPAD_PERCENT, KEYCODE_KEYPAD_LEFT_ANGLE_BRACKET, KEYCODE_KEYPAD_RIGHT_ANGLE_BRACKET, KEYCODE_KEYPAD_AMPERSANT,

    KEYCODE_KEYPAD_LOGICAL_AND, KEYCODE_KEYPAD_VERTICAL_LINE, KEYCODE_KEYPAD_LOGICAL_OR, KEYCODE_KEYPAD_COLON, KEYCODE_KEYPAD_SHAP,
    KEYCODE_KEYPAD_SPACE, KEYCODE_KEYPAD_AT, KEYCODE_KEYPAD_EXCLAMATION, KEYCODE_KEYPAD_MEMSTORE, KEYCODE_KEYPAD_MEMRECALL,
    KEYCODE_KEYPAD_MEMCLEAR, KEYCODE_KEYPAD_MEMADD, KEYCODE_KEYPAD_MEMSUBTRACT, KEYCODE_KEYPAD_MEMMULTIPLY, KEYCODE_KEYPAD_MEMDIVIDE,
    KEYCODE_KEYPAD_PLUS_MINUS, KEYCODE_KEYPAD_CLEAR, KEYCODE_KEYPAD_CLEARENTRY, KEYCODE_KEYPAD_BINARY, KEYCODE_KEYPAD_OCTAL,
    KEYCODE_KEYPAD_DECIMAL, KEYCODE_KEYPAD_HEXADECIMAL, KEYCODE_UNKNOWN_222, KEYCODE_UNKNOWN_223, KEYCODE_LEFT_CONTROL,

    KEYCODE_LEFT_SHIFT, KEYCODE_LEFT_ALT, KEYCODE_LEFT_GUI, KEYCODE_RIGHT_CONTROL, KEYCODE_RIGHT_SHIFT,
    KEYCODE_RIGHT_ALT, KEYCODE_RIGHT_GUI, KEYCODE_UNKNOWN_232, KEYCODE_UNKNOWN_233, KEYCODE_UNKNOWN_234,
    KEYCODE_UNKNOWN_235, KEYCODE_UNKNOWN_236, KEYCODE_UNKNOWN_237, KEYCODE_UNKNOWN_238, KEYCODE_UNKNOWN_239,
    KEYCODE_UNKNOWN_240, KEYCODE_UNKNOWN_241, KEYCODE_UNKNOWN_242, KEYCODE_UNKNOWN_243, KEYCODE_UNKNOWN_244,
    KEYCODE_UNKNOWN_245, KEYCODE_UNKNOWN_246, KEYCODE_UNKNOWN_247, KEYCODE_UNKNOWN_248, KEYCODE_UNKNOWN_249,

    KEYCODE_UNKNOWN_250, KEYCODE_UNKNOWN_251, KEYCODE_UNKNOWN_252, KEYCODE_UNKNOWN_253, KEYCODE_UNKNOWN_254,
    KEYCODE_UNKNOWN_255, KEYCODE_UNKNOWN_256, KEYCODE_MODE_SWITCH, KEYCODE_AUDIO_NEXT, KEYCODE_AUDIO_PREV,
    KEYCODE_AUDIO_STOP, KEYCODE_AUDIO_PLAY, KEYCODE_AUDIO_MUTE, KEYCODE_AUDIO_SELECT, KEYCODE_WWW,
    KEYCODE_MAIL, KEYCODE_CALCULATOR, KEYCODE_COMPUTER, KEYCODE_AC_SEARCH, KEYCODE_AC_HOME,
    KEYCODE_AC_BACK, KEYCODE_AC_FORWARD, KEYCODE_AC_STOP, KEYCODE_AC_REFRESH, KEYCODE_AC_BOOKMARK,

    KEYCODE_BRIGHTNESS_DOWN, KEYCODE_BRIGHTNESS_UP, KEYCODE_DISPLAY_SWITCH, KEYCODE_KEYBOARD_ILLUM_TOGGLE, KEYCODE_KEYBOARD_ILLUM_DOWN,
    KEYCODE_KEYBOARD_ILLUM_UP, KEYCODE_EJECT, KEYCODE_SLEEP,
};

static const char *scancode_names[512] = {
    NULL, NULL, NULL, NULL, "A",                             "B", "C", "D",        "E", "F",
    "G", "H", "I", "J", "K",                                 "L", "M", "N",        "O", "P",
    "Q", "R", "S", "T", "U",                                 "V", "W", "X",        "Y", "Z",
    "1", "2", "3", "4", "5",                                 "6", "7", "8",        "9", "0",
    "Return", "Escape", "Backspace", "Tab", "Space",         "-", "=", "[",        "]",  "\\",

    "#",      ";",      "'",         "`",   ",",             ".", "/", "CapsLock", "F1", "F2", 
    "F3",     "F4",     "F5",        "F6",  "F7",            "F8", "F9", "F10", "F11", "F12", 
    "PrintScreen", "ScrollLock", "Pause", "Insert", "Home",   "PageUp", "Delete", "End", "PageDown", "Right",
    "Left", "Down", "Up", "Numlock", "Keypad /",      "Keypad *", "Keypad -", "Keypad +", "Keypad Enter", "Keypad 1", 
    "Keypad 2", "Keypad 3", "Keypad 4", "Keypad 5", "Keypad 6",      "Keypad 7", "Keypad 8", "Keypad 9", "Keypad 0", "Keypad .",

    NULL, "Application", "Power", "Keypad =", "F13",           "F14", "F15", "F16", "F17", "F18",
    "F19", "F20", "F21", "F22", "F23",                 "F24", "Execute", "Help", "Menu", "Select",
    "Stop", "Again", "Undo", "Cut", "Copy",            "Paste", "Find", "Mute", "VolumeUp", "VolumeDown",
    NULL, NULL, NULL, "Keypad ,", "Keypad = (AS400)",           NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,                                  NULL, NULL, NULL, NULL, NULL, 

    NULL, NULL, NULL, "AltErase","SysReq",                        "Cancel", "Clear", "Prior", "Return", "Separator",
    "Out", "Oper", "Clear / Again", "CrSel", "ExSel",            NULL, NULL, NULL, NULL, NULL, 
    NULL, NULL, NULL, NULL, NULL,                               NULL, "Keypad 00", "Keypad 000", "ThousandsSeparator", "DecimalSeparator",
    "CurrencyUnit", "CurrencySubUnit", "Keypad (", "Keypad )", "Keypad {",            "Keypad }", "Keypad Tab", "Keypad Backspace", "Keypad A", "Keypad B",
    "Keypad C", "Keypad D", "Keypad E", "Keypad F", "Keypad XOR",                    "Keypad ^", "Keypad %%", "Keypad <", "Keypad >", "Keypad &",


    "Keypad &&", "Keypad |", "Keypad ||", "Keypad :", "Keypad #",                  "Keypad Space", "Keypad @", "Keypad !", "Keypad MemStore", "Keypad MemRecall",
    "Keypad MemClear", "Keypad MemAdd", "Keypad MemSubtract", "Keypad MemMultiply", "Keypad MemDivide",                 "Keypad +/-", "Keypad Clear", "Keypad ClearEntry", "Keypad Binary", "Keypad Octal",
    "Keypad Decimal", "Keypad Hexadecimal", NULL, NULL, "Left Ctrl",                    "Left Shift", "Left Alt", "Left GUI", "Right Ctrl", "Right Shift",
    "Right Alt", "Right GUI", NULL, NULL, NULL,                                NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,                      NULL, NULL, NULL, NULL, NULL, 
    
    
    NULL, NULL, NULL, NULL, NULL,                      NULL, NULL, "ModeSwitch", "AudioNext", "AudioPrev",
    "AudioStop", "AudioPlay", "AudioMute", "MediaSelect", "WWW",              "Mail", "Calculator", "Computer", "AC Search", "AC Home",
    "AC Back", "AC Forward", "AC Stop", "AC Refresh", "AC Bookmarks",         "BrightnessDown", "BrightnessUp", "DisplaySwitch", "KBDIllumToggle", "KBDIllumDown",
    "KBDIllumUp", "Eject", "Sleep",
};

#undef NULL
#define NULL (char)0

//이거 ''라서 포인터 아님!!!!!!! 고쳐라!!!
static const char scancode_to_char[512] = {
    NULL, NULL, NULL, NULL, 'a',     'b',  'c',  'd',  'e',  'f',
    'g',  'h',  'i',  'j',  'k',     'l',  'm',  'n',  'o',  'p',
    'q',  'r',  's',  't',  'u',     'v',  'w',  'x',  'y',  'z',
    '1',  '2',  '3',  '4',  '5',     '6',  '7',  '8',  '9',  '0',
    '\n', '\e', '\b', '\t', ' ',     '-',  '=',  '[',  ']',  '\\',

    '#',  ';',  '\'', '`',  ',',     '.',  '/',  NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, '/',     '*',  '-',  '+',  '\n', '1',
    '2',  '3',  '4',  '5',  '6',     '7',  '8',  '9',  '0',  '.',

    NULL, NULL, NULL, '=',  NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, ',',  '=',     NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,

    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, '(',  ')',  '{',     '}',  '\t', '\b', 'a',  'b',
    'c',  'd',  'e',  'f',  NULL,    '^',  '%',  '<',  '>',  '&',

    NULL, '|',  NULL, ':',  '#',     ' ',  '@',  '!',  NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,

    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,                                              //283
};

static const char scancode_to_char_shift[512] = {
    NULL, NULL, NULL, NULL, 'A',     'B',  'C',  'D',  'E',  'F',
    'G',  'H',  'I',  'J',  'K',     'L',  'M',  'N',  'O',  'P',
    'Q',  'R',  'S',  'T',  'U',     'V',  'W',  'X',  'Y',  'Z',
    '!',  '@',  '#',  '$',  '%',     '^',  '&',  '*',  '(',  ')',
    '\n', '\e', '\b', '\t', ' ',     '_',  '+',  '{',  '}',  '|',

    '#',  ':',  '\"', '~',  '<',     '>',  '?',  NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, '/',     '*',  '-',  '+',  '\n', '1',
    '2',  '3',  '4',  '5',  '6',     '7',  '8',  '9',  '0',  '.',

    NULL, NULL, NULL, '=',  NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, ',',  '=',     NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,

    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, '(',  ')',  '{',     '}',  '\t', '\b', 'a',  'b',
    'c',  'd',  'e',  'f',  NULL,    '^',  '%',  '<',  '>',  '&',

    NULL, '|',  NULL, ':',  '#',     ' ',  '@',  '!',  NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,

    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,                                              //283
};

#undef NULL
#define NULL (void*)0

#endif //__KBD_SCANCODE__