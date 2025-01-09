#ifndef __KEYBOARD_COMMAND_H__
#define __KEYBOARD_COMMAND_H__

#include "scene.h"

enum TEST_COLOR
{
    TEST_COLOR_RED = 0,
    TEST_COLOR_GREEN,
    TEST_COLOR_BLUE,
    TEST_COLOR_YELLOW,
    TEST_COLOR_CYAN,
    TEST_COLOR_MAGENTA,
    TEST_COLOR_WHITE,
    TEST_COLOR_GRAY,
    TEST_COLOR_BLACK,
    TEST_GRADATION_RED_LEFT,
    TEST_GRADATION_RED_RIGHT,
    TEST_GRADATION_RED_UP,
    TEST_GRADATION_RED_DOWN,
    TEST_GRADATION_GREEN_LEFT,
    TEST_GRADATION_GREEN_RIGHT,
    TEST_GRADATION_GREEN_UP,
    TEST_GRADATION_GREEN_DOWN,
    TEST_GRADATION_BLUE_LEFT,
    TEST_GRADATION_BLUE_RIGHT,
    TEST_GRADATION_BLUE_UP,
    TEST_GRADATION_BLUE_DOWN,
    TEST_GRADATION_WHITE_LEFT,
    TEST_GRADATION_WHITE_RIGHT,
    TEST_GRADATION_WHITE_UP,
    TEST_GRADATION_WHITE_DOWN,
    TEST_IMAGE,
    TEST_COLOR_END
};

enum COMMAND_RETURN
{
    TEST_FAILED_UNKNOWN = -99,
    TEST_FAILED_WRONG_LENGTH = -1,
    TEST_FAILED_WRONG_VALUE = -2,
    
    RUN_FAILED_UNKNOWN = 99,

    FIND_FAILED = -1000,

    //FIND_SUCCESS = 0,
    TEST_SUCCESS = 0,
    RUN_SUCCESS = 0,
};

#define MAX_LINE    35 //현재 창 설정에 맞는 값. 글자 크기나 줄 간격 달라지면 확인하고 변경 24.11.27 JGLEE

/*---------- bindFunction ----------*/

void bindTextBox(ITUTextBox* input, ITUTextBox* result, ITUBackground* background);

/*---------- execCommand ----------*/

/*!
 * @brief CSID(충전소 사이트 ID)의 값을 조회하거나 변경하는 함수
 * @param[in] newCSID 새 CSID 값. NULL이면 현재 값 조회.
 * @return 성공 시 RUN_SUCCESS(0), 이외에는 union COMMAND_RETURN의 FAILED 값으로 반환
*/
const int setCSID(const char* newCSID);

/*!
 * @brief 관리자 모드를 종료하는 함수
 * @param[in] NOTUSE 사용하지 않음. 형식을 맞추기 위해 존재.
 * @return 무조건 RUN_SUCCESS(0)를 반환하지만 기기가 재부팅되어 확인할 수는 없을 것임
*/
const int runExit(const char* NOTUSE);

void runInputted();
void writeInputBuffer(const char inputted);
/*!
 * @name commandInput
 * @brief 키보드를 통해 명령 입력을 받는 함수.
 */
void commandInput(uint32_t flag, uint32_t code);

#endif //__KEYBOARD_COMMAND_H__