#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ite/itp.h>
#include <ite/itu.h>
#include "cstlog.h"
#include "ctrlboard.h"

#include "usbkeyboard.h"
#include "keyboardCommand.h"

#include "scene.h"

/*
USB Keyboard 입력

현재 폴더의 CMakeList.txt에 keyboardCommand.c와 usbkeyboard.c 등록(186줄 근처에 있는 #qrencode 아래에 넣으시면 됩니다)
현재 폴더의 ctrlboard.h에 usbkeyboard.h 등록 (1050줄 즈음에 include 모여있는 곳에 같이 넣어주세요)
현재 폴더의 main.c의 SDL_main()함수에서 usbKeyboardInit함수를 호출

빌드 설정에서 
    Peripheral -> USB0 Enable 체크
    Peripheral -> USB0 Enable -> USB Device Mode 체크 해제 확인(이거 체크하면 slave가 됩니다!!!!)
    Peripheral -> USBHID enable 체크
    Peripheral -> USBHID enable -> USB keyboard support 체크
하고 빌드해주세요.
빌드 설정이 올바르다면 펌웨어 다운로드 후 키보드를 연결했을 때, 디버그 창에 
    .....
    Found usbkbd_driver usb driver!
    .....
라고 뜰겁니다.
*/

pthread_t keyboardTask;

extern ITPKeyboardEvent uiKeyboardInput;        //Keyboard Input value
extern KeyboardListener uiKeyboardListener;     //Keyboard Listener Function

char* inputBuffer;
const size_t inputBufferSize = 200;
size_t inputBufferTop = 0;

const size_t resultBufferSize = 2000;
char* resultBuffer;
size_t resultLine;

typedef struct
{
    bool leftControl;
    bool leftShift;
    bool leftAlt;

    bool rightControl;
    bool rightShift;
    bool rightAlt;

    bool capslock;
} KEY_MODIFIER;

KEY_MODIFIER Modifier = {false, false, false, false, false, false, false};

extern bool bGloAdminStatus;

KEYBOARD_FILTER selectedFilter = KEYBOARD_FILTER_FREE;

const char scancodeToChar(const char input)
{
    if(input >= KEYCODE_A && input <= KEYCODE_Z) 
        return (((Modifier.leftShift || Modifier.rightShift) ^ Modifier.capslock) ? scancode_to_char_shift[(input)] : scancode_to_char[(input)]);
    else 
        return ((Modifier.leftShift || Modifier.rightShift)  ? scancode_to_char_shift[(input)] : scancode_to_char[(input)]);
}

const bool isShift(bool capslockIncluded)
{
    if(capslockIncluded)
        return (Modifier.leftShift || Modifier.rightShift) ^ Modifier.capslock;
    else
        return Modifier.leftShift || Modifier.rightShift;
}

const bool isAlpha(const char code)
{
    if(code >= KEYCODE_A && code <= KEYCODE_Z) return true;
    // else if(code >= KEYCODE_KEYPAD_A && code <= KEYCODE_KEYPAD_F) return true;
    return false;
}

const bool isNumber(const char code)
{
    if(!isShift(false) && code >= KEYCODE_1 && code <= KEYCODE_0) return true;
    // else if(code >= KEYCODE_KEYPAD_1 && code <= KEYCODE_KEYPAD_0) return true;
    return false;
}

const bool isSpecial(const char code)
{
    if(code >= KEYCODE_HIPHEN_MINUS && code <= KEYCODE_SLASH) return true;
    else if(isShift(false) && code >= KEYCODE_1 && code <= KEYCODE_0) return true;
    return false;
}


void hookKeyboard(KeyboardListener func)
{
    uiKeyboardListener = func;
    printf("[USBKeyboard] New Listener Set! %p\n", uiKeyboardListener);
}

char keyboardFilter(uint32_t code)
{   //Numpad is not supported
    printf("[keyboardFilter] Filter = %d, code = %d", selectedFilter, code);
    //No Input
    if(selectedFilter == KEYBOARD_FILTER_NONE) return 0;

    //Any input
    if(selectedFilter & KEYBOARD_FILTER_BOOLEAN)
    {   //Boolean(0, 1) only
        if((code == KEYCODE_1 || code == KEYCODE_0) && !isShift(false)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_NUMBER)
    {   //Digit(0-9) only
        if(code >= KEYCODE_1 && code <= KEYCODE_0 && !isShift(false)) return scancodeToChar(code);
    }
    
    if(selectedFilter & KEYBOARD_FILTER_DOT)
    {   //Dot(.) only (for IP)
        if(code == KEYCODE_DOT && !isShift(false)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_HEXALPHA)
    {   //Hex Alphabets (A-F)
        if( (code >= KEYCODE_A) && (code <= KEYCODE_F) ) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_COLON)
    {   //Colon(:) only (for MAC addr)
        if( (code == KEYCODE_SEMICOLON) && isShift(false)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_ALPHA)
    {   //Alphabet
        if( (code >= KEYCODE_A) && (code <= KEYCODE_Z) ) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_SLASH)
    {   //Slash(/) only
        if( (code == KEYCODE_SLASH) && !isShift(false)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_HIPHEN)
    {   //Hiphen(-) only
        if(code == KEYCODE_HIPHEN_MINUS && !isShift(false)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_UNDERSCORE)
    {   //Underscore(_) only
        if(code == KEYCODE_HIPHEN_MINUS && isShift(false)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_SPECIAL)
    {   //Special characters(   ~!@#$%^&*()_+`-=[];',./{}:"<>?"\|   )
        if(isSpecial(code)) return scancodeToChar(code);
    }

    if(selectedFilter & KEYBOARD_FILTER_SPACE)
    {
        if( (code == KEYCODE_TAB) || (code <= KEYCODE_SPACE) ) return scancodeToChar(code);
    }

    //No Vaild Input
    return 0;
}

void setKeyboardFilter(KEYBOARD_FILTER filter)
{
    selectedFilter = filter;
}

/*!
 * @name usbKeyboardTask
 * @brief system task for usb keyboard.
 * @param[in] arg NOTUSE.
 * @todo Add Hooker to change function
*/
static void usbKeyboardTask(void* arg)
{
    ITPKeyboardEvent ev;

    while(keyboardTask)
    {
        if(uiKeyboardListener == NULL)
        {
            sleep(5);
            continue;
        }
        if(read(ITP_DEVICE_USBKBD, &ev, sizeof(ITPKeyboardEvent)) == sizeof(ITPKeyboardEvent))
        {  
            printf("[USBKeyboard] flag : %d, code : %d\n", ev.flags, ev.code);
            switch(ev.code)
            {
                case KEYCODE_LEFT_CONTROL:  // Left Control
                    Modifier.leftControl = (ev.flags == ITP_KEYDOWN);
                    break;
                case KEYCODE_LEFT_SHIFT:  // Left Shift
                    Modifier.leftShift = (ev.flags == ITP_KEYDOWN);
                    break;
                case KEYCODE_LEFT_ALT:  // Left Alt
                    Modifier.leftAlt = (ev.flags == ITP_KEYDOWN);
                    break;

                case KEYCODE_RIGHT_CONTROL:  // Right Control
                    Modifier.rightControl = (ev.flags == ITP_KEYDOWN);
                    break;
                case KEYCODE_RIGHT_SHIFT:  // Right Shift
                    Modifier.rightShift = (ev.flags == ITP_KEYDOWN);
                    break;
                case KEYCODE_RIGHT_ALT:  // Right Alt
                    Modifier.rightAlt = (ev.flags == ITP_KEYDOWN);
                    break;

                case KEYCODE_CAPSLOCK:  //Caps lock
                    if(ev.flags == ITP_KEYDOWN)
                        Modifier.capslock = !Modifier.capslock;
                    break;
                default : 
                    // commandInput(ev.flags, ev.code);
                    uiKeyboardInput.code = ev.code;
                    uiKeyboardInput.flags = ev.flags;
            }
        }

        //처리할 것 없으면 바로 sleep
        //TODO: itp_usb_kbd.c와 ite_usbkbd.h, usbkbd.c 를 참고해 irq에서 처리하거나 Queue에서 꺼내와 처리하도록 해보기
        sleep(0);  
    }
    
    //이상하게 빠져나간 태스크 지우기
    CtLogMagenta("[USBKeyboard] Delete keyboard thread..\n");
    vTaskDelete( NULL );
}

void usbKeyboardInit()
{
    //입출력버퍼 초기화. Hoxy나 모를 상황을 대비해 \0을 맨 끝에 두기 위해 inputBufferSize + 1만큼 0으로 초기화
    // inputBuffer = calloc(inputBufferSize + 1, sizeof(char));
    // resultBuffer = calloc(resultBufferSize + 1, sizeof(char));
    CtLogYellow(" create keyboard thread..\n");
    
    if(pthread_create(&keyboardTask, NULL, usbKeyboardTask, NULL))
    {
        printf("[USBKeyboard] Failed to init keyboard thread!\n");
    }
    
}

