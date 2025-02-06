#ifndef __USB_KEYBOARD__H__
#define __USB_KEYBOARD__H__

#include "kbd_scancode.h"

//const char* DEVIP = "";
/*!
 * @name KeyboardListener
 * @brief 키보드를 통해 입력을 받을 함수
 * @param[in] flag 키가 눌린 건지(ITP_KEYDOWN), 떼진 건지(ITP_KEYUP) 판단하는 플래그
 * @param[in] code 이벤트가 일어난 키의 키코드 값
 * 
 */
typedef void (*KeyboardListener)(uint32_t flag, uint32_t code);

void hookKeyboard(KeyboardListener func);


const char scancodeToChar(const char input);


/**
 * @brief Determind that shifted
 * 
 * @param capslockIncluded - Is capslock state have any effect? (like alphabet characters)
 * @return true - need to shift
 * @return false - no shift needed
 */
const bool isShift(bool capslockIncluded);

/**
 * @brief Determind that pressed key is alphabet character
 * 
 * @param code pressed key's code
 * @return true - Able to translated to alphabet character
 * @return false - Not able to translated to alphabet character
 */

const bool isAlpha(const char code);
/**
 * @brief Determind that pressed key is numeric character
 * 
 * @param code pressed key's code
 * @return true - Able to translated to numeric character
 * @return false - Not able to translated to numeric character
 */
const bool isNumber(const char code);

/**
 * @brief Determind that pressed key is special character
 * 
 * @param code pressed key's code
 * @return true - Able to translated to special character
 * @return false - Not able to translated to special character
 */
const bool isSpecial(const char code);

char keyboardFilter(uint32_t code);


typedef enum 
{
    KEYBOARD_FILTER_NONE        = 0,
    KEYBOARD_FILTER_NUMBER      = 1,   //0b000 00000001
    KEYBOARD_FILTER_DOT         = 2,   //0b000 00000010,
    KEYBOARD_FILTER_IP          = 3,   //0b000 00000011,
    KEYBOARD_FILTER_HEXALPHA    = 4,   //0b000 00000100,
    KEYBOARD_FILTER_HEX         = 5,   //0b000 00000101,
    KEYBOARD_FILTER_COLON       = 8,   //0b000 00001000,
    KEYBOARD_FILTER_MAC         = 13,  //0b000 00001101,
    KEYBOARD_FILTER_ALPHA       = 16,  //0b000 00010000,
    KEYBOARD_FILTER_SLASH       = 32,  //0b000 00100000,
    KEYBOARD_FILTER_SPECIAL     = 64,  //0b000 01000000,
    KEYBOARD_FILTER_SPACE       = 128, //0b000 10000000,
    KEYBOARD_FILTER_HIPHEN      = 256, //0b001 00000000,
    KEYBOARD_FILTER_UNDERSCORE  = 512, //0b010 00000000,
    KEYBOARD_FILTER_ADDRESS     = 831, //0b011 00111111,
    KEYBOARD_FILTER_BOOLEAN     = 1024,//0b100 00000000
    KEYBOARD_FILTER_FREE        = 2047,//0b111 11111111,
} KEYBOARD_FILTER;
void setKeyboardFilter(KEYBOARD_FILTER filter);

void usbKeyboardInit();

#endif //__USBKEYBOARD__H__
