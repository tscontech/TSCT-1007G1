#ifndef __USB_KEYBOARD__H__
#define __USB_KEYBOARD__H__

#include "kbd_scancode.h"

//const char* DEVIP = "";
/*!
 * @name KeyboardBindFunc
 * @brief 키보드를 통해 입력을 받을 함수
 * @param[in] flag 키가 눌린 건지(ITP_KEYDOWN), 떼진 건지(ITP_KEYUP) 판단하는 플래그
 * @param[in] code 이벤트가 일어난 키의 키코드 값
 * 
 */
typedef void (*KeyboardBindFunc)(uint32_t flag, uint32_t code);

void hookKeyboard(KeyboardBindFunc func);


const char scancodeToChar(const char input);
/** 
typedef void (*KeyboardBindFunc)(uint32_t flag, uint32_t input);

void keyboardBinder(KeyboardBindFunc func);
void defaultBinder(uint32_t flag, uint32_t input);
*/


void usbKeyboardInit();

#endif //__USBKEYBOARD__H__
