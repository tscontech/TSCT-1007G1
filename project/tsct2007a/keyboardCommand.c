#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ite/itp.h>
#include <ite/itu.h>
#include "cstlog.h"
#include "ctrlboard.h"
#include "scene.h"
#include "keyboardCommand.h"

/*
우선순위 : 
	1. 키보드 입력이 되는가?                                            가능
	2. 키보드 입력을 의미있는 단위로 모을 수 있는가?(명령어 한 줄 단위)  가능
	3. 의미 있는 단위로 모아 기능을 실행할 수 있는가?                    가능
        1. 파싱하여 명령어와 데이터로 분리할 수 있는가?                  가능
        2. 데이터를 파싱하여 원하는 형태로 저장할 수 있는가?             각 함수 별로 구현해야 함
        3. 데이터를 검증할 수 있는가?                                   가능
        4. 명령어 목록에 있는 올바른 명령을 실행할 수 있는가?            가능
        5. 데이터 검증을 입력받으면서 같이 진행할 수 있는가?             안함
    4. 결과 메세지를 띄울 수 있는가?                                     가능
    5. 도움말을 띄울 수 있는가?                                          가능
    6. 위아래화살표 키로 이전 명령을 탐색할 수 있는가?                   몰?루
    7. 좌우화살표 키로 커서를 옮길 수 있는가?                            가능

데모 명령 : srvaddr com-me.tsct.com
            srvaddr com-me.tscontect.com
            dev
            devmac 4d:
*/

//TODO: 현재 구현은 각 test 함수나 exec함수에서 resultTextBox를 조작. 이것을 바꿀 것.

extern Config theConfig;

char* command, *data;  //명령과 데이터로 나누었을 때 각각의 문자열 시작점.

ITUTextBox* focusedTextBox;
ITUTextBox* resultTextBox;

extern char* resultBuffer;
extern const size_t resultBufferSize;
extern size_t resultLine;

//입력 버퍼
extern char* inputBuffer;
extern const size_t inputBufferSize;
extern size_t inputBufferTop;

void clearInputBuffer()
{
    memset(inputBuffer, 0, inputBufferSize);
    inputBufferTop = 0;
    command = data = NULL;
    if(focusedTextBox)
        ituTextBoxSetString(focusedTextBox, inputBuffer);
}

void bindTextBox(ITUTextBox* input, ITUTextBox* result)
{
    focusedTextBox = input;
    resultTextBox = result;
    clearInputBuffer();
    resultLine = 0;
}

void updateResultBox(const char* newText)
{
    ++resultLine;
    if(resultLine > 35)
    {
        strcpy(resultBuffer, strchr(resultBuffer, '\n') + 1);
        resultLine = 35;
    }
    ituTextBoxSetString(resultTextBox, resultBuffer);
}

/* ----------------------Data Vaildation Functions(Optional)-------------------------*/
const int testNumber(const char* newNumber, const int length, const int min, const int max, const char* funcname)
{
    //Is Digit?
    if(!isdigit(newNumber))
    {
        printf("Wrong value for %s : %s\n", funcname, newNumber);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "%s는 %d자리 숫자입니다.(%s)\n", funcname, length, newNumber);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return TEST_FAILED_WRONG_VALUE;
    }

    //길이 검사 - 값의 길이가 짧아도 키가 입력되지 않았을 경우를 대비해 오류 처리
    if(strlen(newNumber) != length)
    {
        printf("Wrong! %d numbers required for DEVID, but %d inputted : %s\n", length, strlen(newNumber), newNumber);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "%s는 %d자리 숫자입니다.(%s)\n", funcname, length, newNumber);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return TEST_FAILED_WRONG_LENGTH;  
    }

    //Min Max Test
    int translated = atoi(newNumber);
    if(translated > max || translated < min)
    {
        printf("Wrong value for %s : %s\n", funcname, newNumber);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "%s의 최대 값은 %d, 최소 값은 %d입니다.(%s)\n", funcname, max, min, newNumber);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return TEST_FAILED_WRONG_VALUE;
    }

    return TEST_SUCCESS;
}

/*!
 * @brief Validate IP Address
 * @param[in] newIP New IP for device requiring validation
 * @return If 0, it returns SUCCESS, otherwise it returns FAILED value of union KBD_RETURN
*/
const int testIP(const char* newIP)
{
    int ipaddr[4] = {0, };
    uint8_t* ptr;

    //Length test
    if(strlen(newIP) > 15)
    {
        printf("Wrong! Inputted value is too long : %s\n", newIP);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "입력 값이 너무 깁니다.(%s)\n", newIP);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }

        return TEST_FAILED_WRONG_LENGTH;  
    }
    if(strlen(newIP) < 7)
    {
        printf("Wrong! Inputted value is too short : %s\n", newIP);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "입력 값이 너무 짧습니다.(%s)\n", newIP);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }

        return TEST_FAILED_WRONG_LENGTH;  
    }

    //Character test
    if(strspn(newIP, "0123456789.") != strlen(newIP))
    {
        printf("Wrong! IP must consist of 4 numbers separated by 3 periods. : %s\n", newIP);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "IP는 135.254.089.7과 같이 3개의 점으로 구별된 최대 3자리 수 4개로 이루어져있습니다.(%s)\n", newIP);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return TEST_FAILED_WRONG_VALUE;
    }

    //Separate IP string to int list;
    ptr = newIP;

    for(int i = 0; i < 3; ++i)
    {//FIXME: 이거 안잡힘!
        //Number test -> Is this number between 0 and 255?
        if(!isdigit(*ptr))
        {
            printf("Wrong! Each number must be in 0 to 255, but %d inputted : %s\n", ipaddr[i], newIP);
            if(resultTextBox)
            {
                sprintf(resultBuffer, "IP의 각 숫자는 0에서 255 사이의 숫자로 되어있습니다.(%s)\n", newIP);
                ituTextBoxInput(resultTextBox, resultBuffer);
            }
            return TEST_FAILED_WRONG_VALUE;
        }

        ipaddr[i] = atoi(ptr);
        if(ipaddr[i] < 0 || ipaddr[i] > 255)
        {
            printf("Wrong! Each number must be in 0 to 255, but %d inputted : %s\n", ipaddr[0], newIP);
            if(resultTextBox)
            {
                sprintf(resultBuffer, "IP의 각 숫자는 0에서 255 사이의 숫자로 되어있습니다.(%s)\n", newIP);
                ituTextBoxInput(resultTextBox, resultBuffer);
            }
            return TEST_FAILED_WRONG_VALUE;
        }

        //Move -> If it fails, it means too few numbers were entered.
        if((ptr = strchr(ptr, '.')) == NULL)
        {
            printf("Wrong! IP must consist of four numbers separated by three periods. : %s\n", newIP);
            if(resultTextBox)
            {
                sprintf(resultBuffer, "IP는 3개의 점으로 구별된 4개의 숫자로 되어있습니다.(%s)\n", newIP);
                ituTextBoxInput(resultTextBox, resultBuffer);
            }
            return TEST_FAILED_WRONG_VALUE;
        }
        ++ptr;
    }

    //Number test for last number
    ipaddr[3] = atoi(ptr);
    if(ipaddr[3] < 0 || ipaddr[3] > 255)
    {
        printf("Wrong! Each number must be in 0 to 255, but %d inputted : %s\n", ipaddr[0], newIP);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "IP는 0에서 255 사이의 숫자로 되어있습니다.(%s)\n", newIP);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return TEST_FAILED_WRONG_VALUE;
    }

    return TEST_SUCCESS;
}

const int testMAC(const char* newMAC)
{
    //길이 검사 - 값의 길이가 짧아도 키가 입력되지 않았을 경우를 대비해 오류 처리
    if(strlen(newMAC) != 17)
    {
        printf("Wrong! 17 numbers required for CSID, but %d inputted : %s\n", strlen(newMAC), newMAC);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "MAC 주소는 01:23:45:67:89:AB처럼 2자리 16진수 6개로 이루어져있습니다.(%s)\n", newMAC);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }

        return TEST_FAILED_WRONG_LENGTH;  
    }
    const char* ptr = newMAC;

    //TODO: 나중에 깨끗하게 정리할 것
    for(int i = 0; i < 5; ++i)
    {  //올바른 값이 아님
        if( isxdigit(*(ptr++)) && isxdigit(*(ptr++)))
        {
            printf("Wrong value for CSID : %s\n", newMAC);
            if(resultTextBox)
            {
                sprintf(resultBuffer, "16진수 숫자와 :만 입력 가능합니다.(%s)\n", newMAC);
                ituTextBoxInput(resultTextBox, resultBuffer);
            }
            return TEST_FAILED_WRONG_VALUE;
        }
        if( *(ptr++) != ':')
        {
            printf("Wrong value for CSID : %s\n", newMAC);
            if(resultTextBox)
            {
                sprintf(resultBuffer, "16진수 숫자와 :만 입력 가능합니다.(%s)\n", newMAC);
                ituTextBoxInput(resultTextBox, resultBuffer);
            }
            return TEST_FAILED_WRONG_VALUE;
        }
        ++ptr;
    }
    if( isxdigit(*(ptr++)) && isxdigit(*(ptr++)))
    {
        printf("Wrong value for CSID : %s\n", newMAC);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "16진수 숫자와 :만 입력 가능합니다.(%s)\n", newMAC);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return TEST_FAILED_WRONG_VALUE;
    }
    return TEST_SUCCESS;
}

/* ----------------------Command Executation Functions(Required)-------------------------*/

//test inputted CSID, set new CSID, and show on GUI text wigets.
//if new csid is "" or NULL, just show current CSID on GUI resultTextBox.
const int setCSID(const char* newCSID)
{
    if(newCSID == NULL || *newCSID == '\0')
    {  //Show current CSID when NULL or "" inputted.
        printf("CSID : %s\n", theConfig.siteid);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 CSID는 %s입니다.\n", theConfig.siteid);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    // int testResult = testCSID(newCSID);  //실행 전 검증
    int testResult = testNumber(newCSID, 8, 0, 99999999, "CSID");

    if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
        return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old CSID : %s\n", theConfig.siteid);
    strncpy(theConfig.siteid, newCSID, sizeof(theConfig.siteid));
    printf("New CSID : %s\n", theConfig.siteid);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "CSID를 %s로 변경했습니다.\n", theConfig.siteid);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setDEVID(const char* newDEVID)
{
    if(newDEVID == NULL || *newDEVID == '\0')
    {  //Show current DEVID when NULL or "" inputted.
        printf("DEVID : %s\n", theConfig.devid1);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 DEVID는 %s입니다.\n", theConfig.devid1);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testNumber(newDEVID, 2, 0, 99, "DEVID");  //실행 전 검증

    if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
        return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old DEVID : %s\n", theConfig.devid1);
    strncpy(theConfig.devid1, newDEVID, sizeof(theConfig.devid1));
    printf("New DEVID : %s\n", theConfig.devid1);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "DEVID를 %s로 변경했습니다.\n", theConfig.devid1);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setMaxPower(const char* newMaxPower)
{
    // if(newMaxPower == NULL || *newMaxPower == '\0')
    // {  //Show current MaxPower when NULL or "" inputted.
    //     printf("MaxPower : %s\n", theConfig.chargingstatus);
    //     if(resultTextBox)
    //     {
    //         sprintf(resultBuffer, "현재 최대 출력은 %d Kw입니다.\n", theConfig.chargingstatus);
    //         ituTextBoxInput(resultTextBox, resultBuffer);
    //     }
    //     return RUN_SUCCESS;
    // }

    // int testResult = testNumber(newMaxPower, 2, 99, 0, "MaxPower");  //실행 전 검증

    // if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
    //     return testResult;

    // //값 변경(변경 전후 값 보여주기)
    // printf("Old MaxPower : %s\n", theConfig.chargingstatus);
    // strncpy(theConfig.chargingstatus, newMaxPower, sizeof(theConfig.chargingstatus));
    // printf("New MaxPower : %s\n", theConfig.chargingstatus);

    // if(resultTextBox)
    // {
    //     sprintf(resultBuffer, "최대 출력을 %d Kw로 변경했습니다.\n", newMaxPower);
    //     ituTextBoxInput(resultTextBox, resultBuffer);
    // }

    printf("MaxPower - Not Supported now.\n");
    sprintf(resultBuffer, "현재 최대 출력 변경은 지원하지 않습니다.\n");
    ituTextBoxInput(resultTextBox, resultBuffer);

    return RUN_SUCCESS;
}

const int setCard(const char* newCard)
{
    // if(newMaxPower == NULL || *newMaxPower == '\0')
    // {  //Show current MaxPower when NULL or "" inputted.
    //     printf("MaxPower : %s\n", theConfig.chargingstatus);
    //     if(resultTextBox)
    //     {
    //         sprintf(resultBuffer, "현재 최대 출력은 %d Kw입니다.\n", theConfig.chargingstatus);
    //         ituTextBoxInput(resultTextBox, resultBuffer);
    //     }
    //     return RUN_SUCCESS;
    // }

    // int testResult = testNumber(newMaxPower, 2, 99, 0, "MaxPower");  //실행 전 검증

    // if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
    //     return testResult;

    // //값 변경(변경 전후 값 보여주기)
    // printf("Old MaxPower : %s\n", theConfig.chargingstatus);
    // strncpy(theConfig.chargingstatus, newMaxPower, sizeof(theConfig.chargingstatus));
    // printf("New MaxPower : %s\n", theConfig.chargingstatus);

    // if(resultTextBox)
    // {
    //     sprintf(resultBuffer, "최대 출력을 %d Kw로 변경했습니다.\n", newMaxPower);
    //     ituTextBoxInput(resultTextBox, resultBuffer);
    // }

    printf("Card - Not Supported now.\n");
    sprintf(resultBuffer, "현재 카드 변경은 지원하지 않습니다.\n");
    ituTextBoxInput(resultTextBox, resultBuffer);

    return RUN_SUCCESS;
}

const int setAuthPW(const char* newAuthPW)
{
    // if(newMaxPower == NULL || *newMaxPower == '\0')
    // {  //Show current MaxPower when NULL or "" inputted.
    //     printf("MaxPower : %s\n", theConfig.chargingstatus);
    //     if(resultTextBox)
    //     {
    //         sprintf(resultBuffer, "현재 최대 출력은 %d Kw입니다.\n", theConfig.chargingstatus);
    //         ituTextBoxInput(resultTextBox, resultBuffer);
    //     }
    //     return RUN_SUCCESS;
    // }

    // int testResult = testNumber(newMaxPower, 2, 99, 0, "MaxPower");  //실행 전 검증

    // if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
    //     return testResult;

    // //값 변경(변경 전후 값 보여주기)
    // printf("Old MaxPower : %s\n", theConfig.chargingstatus);
    // strncpy(theConfig.chargingstatus, newMaxPower, sizeof(theConfig.chargingstatus));
    // printf("New MaxPower : %s\n", theConfig.chargingstatus);

    // if(resultTextBox)
    // {
    //     sprintf(resultBuffer, "최대 출력을 %d Kw로 변경했습니다.\n", newMaxPower);
    //     ituTextBoxInput(resultTextBox, resultBuffer);
    // }

    printf("AuthPW - Not Supported now.\n");
    sprintf(resultBuffer, "현재 인증 비밀번호 변경은 지원하지 않습니다.\n");
    ituTextBoxInput(resultTextBox, resultBuffer);

    return RUN_SUCCESS;
}

const int setMode(const char* newMode)
{
    char* modes[]={"일반", "점검",  "무료"};
    if(newMode == NULL || *newMode == '\0')
    {  //Show current Mode when NULL or "" inputted.
        printf("Mode : %s\n", theConfig.OperationMode);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 운용 모드는 %s 입니다.\n", modes[theConfig.OperationMode]);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testNumber(newMode, 1, 0, 2, "Mode");  //실행 전 검증

    if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
        return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old Mode : %s\n", modes[theConfig.OperationMode]);
    theConfig.OperationMode = newMode;
    printf("New Mode : %s\n", modes[theConfig.OperationMode]);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "운용 모드를 %s로 변경했습니다.\n", modes[theConfig.OperationMode]);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setAdminPW(const char* newAdminPW)
{
    if(newAdminPW == NULL || *newAdminPW == '\0')
    {  //Show current AdminPW when NULL or "" inputted.
        printf("AdminPW : %s\n", theConfig.adminpassword);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 관리자 비밀번호는 %s 입니다.\n",theConfig.adminpassword);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    // int testResult = testNumber(newAdminPW, 1, 2, 0, "AdminPW");  //TODO: 관리자 비밀번호 검증 만들 것

    // if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
    //     return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old AdminPW : %s\n", theConfig.adminpassword);
    strncpy(theConfig.adminpassword, newAdminPW, sizeof(theConfig.adminpassword));
    printf("New AdminPW : %s\n", theConfig.adminpassword);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "관리자 비밀번호를 %s로 변경했습니다.\n", theConfig.adminpassword);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setFreeTime(const char* newFreeTime)
{
    if(newFreeTime == NULL || *newFreeTime == '\0')
    {  //Show current FreeTime when NULL or "" inputted.
        printf("FreeTime : %s\n", theConfig.FreeChargingTime);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 무료 충전 시간은 %d%s 입니다.\n",
            theConfig.FreeChargingTime, theConfig.FreeChargingTime ? "분" : "분(무제한)");
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testNumber(newFreeTime, 2, 0, 99, "FreeTime");  //실행 전 검증

    if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
        return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old FreeTime : %s\n", theConfig.FreeChargingTime);
    theConfig.FreeChargingTime = atoi(newFreeTime);
    printf("New FreeTime : %s\n", theConfig.FreeChargingTime);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "무료 충전 시간을 %d%s으로 변경했습니다.\n", 
        theConfig.FreeChargingTime, theConfig.FreeChargingTime ? "분" : "분(무제한)");
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setScrSaver(const char* newScrSaver)
{
    if(newScrSaver == NULL || *newScrSaver == '\0')
    {  //Show current ScreenSaverTime when NULL or "" inputted.
        printf("ScrSaver : %s\n", theConfig.screensaver_time);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 화면 자동 꺼짐 시간은 %d분 입니다.\n", theConfig.screensaver_time);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testNumber(newScrSaver, 2, 0, 99, "ScrSaver");  //실행 전 검증

    if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
        return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old ScrSaver : %s\n", theConfig.screensaver_time);
    theConfig.screensaver_time = newScrSaver;
    printf("New ScrSaver : %s\n", theConfig.screensaver_time);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "화면 자동 꺼짐 시간을 %d분으로 변경했습니다.\n", theConfig.screensaver_time);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setAudioLev(const char* newAudioLev)
{
    if(newAudioLev == NULL || *newAudioLev == '\0')
    {  //Show current AudioLev when NULL or "" inputted.
        printf("AudioLev : %s\n", theConfig.audiolevel);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 음량은 %d 입니다.\n", theConfig.audiolevel);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testNumber(newAudioLev, 2, 0, 100, "AudioLevel");  //실행 전 검증

    if(testResult != TEST_SUCCESS) //실행 전 검증 실패!
        return testResult;

    //값 변경(변경 전후 값 보여주기)
    printf("Old AudioLev : %s\n", theConfig.audiolevel);
    theConfig.audiolevel = newAudioLev;
    printf("New AudioLev : %s\n", theConfig.audiolevel);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "음량을 %d로 변경했습니다.\n", theConfig.audiolevel);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setDevIP(const char* newDevIP)
{
    if(newDevIP == NULL || *newDevIP == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("DevIP : %s\n", theConfig.ipaddr);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 기기의 IP는 %s입니다.\n", theConfig.ipaddr);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testIP(newDevIP);

    if(testResult != TEST_SUCCESS)
        return testResult;

    printf("Old DevIP : %s\n", theConfig.ipaddr);
    strncpy(theConfig.ipaddr, newDevIP, sizeof(theConfig.ipaddr));
    printf("New DevIP : %s\n", theConfig.ipaddr);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "기기의 IP를 %s로 변경했습니다.\n", theConfig.ipaddr);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setDevGW(const char* newDevGW)
{
    if(newDevGW == NULL || *newDevGW == '\0')
    {  //Show current device's GW when NULL or "" inputted.
        printf("DevGW : %s\n", theConfig.gw);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 기기의 GateWay는 %s입니다.\n", theConfig.gw);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testIP(newDevGW);  //같이 써도 무방

    if(testResult != TEST_SUCCESS)
        return testResult;

    printf("Old DevGW : %s\n", theConfig.gw);
    strncpy(theConfig.gw, newDevGW, sizeof(theConfig.gw));
    printf("New DevGW : %s\n", theConfig.gw);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "기기의 GateWay를 %s로 변경했습니다.\n", theConfig.gw);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setDevSM(const char* newDevSM)
{
    if(newDevSM == NULL || *newDevSM == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("DevIP : %s\n", theConfig.netmask);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 기기의 Subnet Mask는 %s입니다.\n", theConfig.netmask);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testIP(newDevSM);  //TODO: SM용 함수 만들 것!

    if(testResult != TEST_SUCCESS)
        return testResult;

    printf("Old DevIP : %s\n", theConfig.netmask);
    strncpy(theConfig.netmask, newDevSM, sizeof(theConfig.netmask));
    printf("New DevIP : %s\n", theConfig.netmask);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "기기의 Subnet Mask를 %s로 변경했습니다.\n", theConfig.netmask);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setDevMAC(const char* newDevMAC)
{
    if(newDevMAC == NULL || *newDevMAC == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("DevMAC : %s\n", theConfig.chargermac);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 기기의 MAC 주소는 %s입니다.\n", theConfig.chargermac);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    // int testResult = testDevMAC(newDevMAC);  TODO: 만들 것!

    // if(testResult != TEST_SUCCESS)
    //     return testResult;

    printf("Old DevMAC : %s\n", theConfig.chargermac);
    strncpy(theConfig.chargermac, newDevMAC, sizeof(theConfig.chargermac));
    printf("New DevMAC : %s\n", theConfig.chargermac);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "기기의 MAC 주소를 %s로 변경했습니다.\n", theConfig.chargermac);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setDHCP(const char* newDHCP)
{
    bool dhcpStatus;

    if(newDHCP == NULL || *newDHCP == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("DHCP : %c\n", theConfig.dhcp);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 기기의 DHCP설정은 %s 입니다.\n", theConfig.dhcp ? "ON" : "OFF");
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }


    if(strlen(newDHCP) != 1)
    {
        sprintf(resultBuffer, "setDHCP 입력 오류 : 0 또는 1만 입력해주세요.\n");
        ituTextBoxInput(resultTextBox, resultBuffer);
        return TEST_FAILED_WRONG_VALUE;
    }
    if(*newDHCP == '0') dhcpStatus = false;
    else if (*newDHCP == '1') dhcpStatus = true;
    else
    {
        sprintf(resultBuffer, "setDHCP 입력 오류 : 0 또는 1만 입력해주세요.\n");
        ituTextBoxInput(resultTextBox, resultBuffer);
        return TEST_FAILED_WRONG_VALUE;
    }


    printf("Old DHCP : %c\n", theConfig.dhcp);
    theConfig.dhcp = newDHCP;
    printf("New DHCP : %c\n", theConfig.dhcp);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "기기의 DHCP를 %s로 변경했습니다.\n", theConfig.dhcp ? "ON" : "OFF");
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}


const int setSrvAddr(const char* newSrvAddr)
{
    if(newSrvAddr == NULL || *newSrvAddr == '\0')
    {  //Show current server's address when NULL or "" inputted.
        printf("SrvAddr : %s\n", theConfig.serverip);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 서버 주소는 %s입니다.\n", theConfig.serverip);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    //TODO: 주소 검사 함수 만들기
    // int testResult = testIP(newSrvAddr);

    // if(testResult != TEST_SUCCESS)
    //     return testResult;

    printf("Old SrvAddr : %s\n", theConfig.serverip);
    strncpy(theConfig.serverip, newSrvAddr, sizeof(theConfig.serverip));
    printf("New SrvAddr : %s\n", theConfig.serverip);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "서버 주소를 %s로 변경했습니다.\n", theConfig.serverip);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}


const int setSrvPort(const char* newSrvPort)
{
    if(newSrvPort == NULL || *newSrvPort == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("SrvPort : %s\n", theConfig.serverport);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 서버의 Port 번호는 %s입니다.\n", theConfig.serverport);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    int testResult = testNumber(newSrvPort, 5, 0, 65535, "SRVPORT");

    if(testResult != TEST_SUCCESS)
        return testResult;

    printf("Old SrvPort : %d\n", theConfig.serverport);
    theConfig.serverport = atoi(newSrvPort);
    printf("New SrvPort : %d\n", theConfig.serverport);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "서버의 Port 번호를 %s로 변경했습니다.\n", theConfig.serverport);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setFtpAddr(const char* newFtpAddr)
{
    if(newFtpAddr == NULL || *newFtpAddr == '\0')
    {  //Show current server's address when NULL or "" inputted.
        printf("FtpAddr : %s\n", theConfig.ftpDns);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 FTP 서버 주소는 %s입니다.\n", theConfig.ftpDns);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    //TODO: 주소 검사 함수 만들기
    // int testResult = testIP(newFtpAddr);

    // if(testResult != TEST_SUCCESS)
    //     return testResult;

    printf("Old FtpAddr : %s\n", theConfig.ftpDns);
    strncpy(theConfig.ftpDns, newFtpAddr, sizeof(theConfig.ftpDns));
    printf("New FtpAddr : %s\n", theConfig.ftpDns);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "FTP 서버 주소를 %s로 변경했습니다.\n", theConfig.ftpDns);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}


const int setFtpID(const char* newFtpID)
{

    if(newFtpID == NULL || *newFtpID == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("FtpID : %s\n", theConfig.ftpId);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 FTP 서버의 접속 ID는 %s입니다.\n", theConfig.ftpId);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    //TODO: ID검사 만들기
    // int testResult = testID(newFtpID);

    // if(testResult != TEST_SUCCESS)
    //     return testResult;

    printf("Old FtpID : %d\n", theConfig.ftpId);
    strncpy(theConfig.ftpId, newFtpID, sizeof(theConfig.ftpId));
    printf("New FtpID : %d\n", theConfig.ftpId);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "FTP 서버의 접속 ID를 %s로 변경했습니다.\n", theConfig.ftpId);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int setFtpPW(const char* newFtpPW)
{

    if(newFtpPW == NULL || *newFtpPW == '\0')
    {  //Show current device's IP when NULL or "" inputted.
        printf("FtpPW : %s\n", theConfig.ftpPw);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "현재 FTP 서버의 접속 비밀번호는 %s입니다.\n", theConfig.ftpPw);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
        return RUN_SUCCESS;
    }

    //TODO: PW검사 만들기
    // int testResult = testID(newFtpPW);

    // if(testResult != TEST_SUCCESS)
    //     return testResult;

    printf("Old FtpPW : %d\n", theConfig.ftpPw);
    strncpy(theConfig.ftpPw, newFtpPW, sizeof(theConfig.ftpPw));
    printf("New FtpPW : %d\n", theConfig.ftpPw);

    if(resultTextBox)
    {
        sprintf(resultBuffer, "FTP 서버의 접속 비밀번호를 %s로 변경했습니다.\n", theConfig.ftpPw);
        ituTextBoxInput(resultTextBox, resultBuffer);
    }

    return RUN_SUCCESS;
}

const int runClear(const char* NOTUSE)
{
    resultLine = 0;
    ituTextBoxSetString(resultTextBox, "");
    memset(resultBuffer, 0, resultBufferSize);
    return RUN_SUCCESS;
}

const int runExit(const char* NOTUSE)
{
    if(resultTextBox)
    {
        ituTextBoxInput(resultTextBox, "현재 값을 저장하고 종료합니다.");
    }
	AdminSetupMenuExit(true);
    return RUN_SUCCESS;
}

/* ----------------------키보드 구동 함수-------------------------*/

void runInputted()
{
    int result = FIND_FAILED;

    command = inputBuffer;
    while(*command == ' ') ++command;

    data = strchr(command, ' ');  //command에서부터 첫 ' '을 찾아 위치를 char*로 반환, 없으면 NULL
    if(data != NULL)
    {
        *(data++) = '\0';  //data와 command를 NULL로 구분짓기
        while(*data == ' ') ++data;  //data앞에 ' '가 더 있다면 무시
    }

    for(int i = 0; i < strlen(command); ++i)
        if(isupper(command[i])) command[i] = command[i] + 32;
    
    //DEBUG: 아래의 printf는 디버그용!
    printf("COMMAND : %s\nDATA : %s\n", command, (data != NULL) ? data : "\\\\null\\\\");

    //COMMAND_LIST에서 일치하는 명령 찾아 실행
    if(strcmp("csid", command) == 0)
    {
        result = setCSID(data);
    }
    if(strcmp("devid", command) == 0)
    {
        result = setDEVID(data);
    }
    else if(strcmp("maxpower", command) == 0)
    {
        result = setMaxPower(data);
    }
    else if(strcmp("card", command) == 0)
    {
        result = setCard(data);
    }
    else if(strcmp("authpw", command) == 0)
    {
        result = setAuthPW(data);
    }
    else if(strcmp("mode", command) == 0)
    {
        result = setMode(data);
    }
    else if(strcmp("adminpw", command) == 0)
    {
        result = setAdminPW(data);
    }
    else if(strcmp("freetime", command) == 0)
    {
        result = setFreeTime(data);
    }
    else if(strcmp("scrsaver", command) == 0)
    {
        result = setScrSaver(data);
    }
    else if(strcmp("audiolev", command) == 0)
    {
        result = setAudioLev(data);
    }
    else if(strcmp("devip", command) == 0)
    {
        result = setDevIP(data);
    }
    else if(strcmp("devgw", command) == 0)
    {
        result = setDevGW(data);
    }
    else if(strcmp("devsm", command) == 0)
    {
        result = setDevSM(data);
    }
    else if(strcmp("devmac", command) == 0)
    {
        result = setDevMAC(data);
    }
    else if(strcmp("devdhcp", command) == 0)
    {
        result = setDHCP(data);
    }
    else if(strcmp("srvaddr", command) == 0)
    {
        result = setSrvAddr(data);
    }
    else if(strcmp("srvport", command) == 0)
    {
        result = setSrvPort(data);
    }
    else if(strcmp("ftpaddr", command) == 0)
    {
        result = setFtpAddr(data);
    }
    else if(strcmp("ftpid", command) == 0)
    {
        result = setFtpID(data);
    }
    else if(strcmp("ftppw", command) == 0)
    {
        result = setFtpPW(data);
    }
    else if(strcmp("exit", command) == 0)
    {
        result = runClear(data);
    }
    else if(strcmp("exit", command) == 0)
    {
        result = runExit(data);
    }

    if(result == FIND_FAILED)
    {
        printf("No command found : %s\n", inputBuffer);
        if(resultTextBox)
        {
            sprintf(resultBuffer, "해당 명령은 없습니다.(%s)\n", command);
            ituTextBoxInput(resultTextBox, resultBuffer);
        }
    }

    //입력 버퍼 초기화
    clearInputBuffer();
}

void writeInputBuffer(const char inputted)
{
    int cursor = focusedTextBox->cursorIndex;
    if(inputted == '\b')
    {   //백스페이스 입력 시 -> 버퍼에서 하나 지움; 버퍼가 비었으면 무시!
        if(inputBufferTop > 0 && cursor != 0) 
        {
            strcpy(inputBuffer + cursor - 1, inputBuffer + cursor);
            --cursor;
            inputBuffer[--inputBufferTop] = '\0';
        }
    }
    else
    {   //정상 입력; 버퍼가 꽉찼으면 입력 무시!
        if(inputBufferTop < inputBufferSize)
        {
            if(inputBufferTop != 0)
            {
                for(size_t i = inputBufferTop; i >= cursor; --i)
                {
                    inputBuffer[i+1] = inputBuffer[i];
                }
            }
            inputBuffer[cursor++] = inputted;
            ++inputBufferTop;
        }
    }

    if(focusedTextBox)
    {
        ituTextBoxSetString(focusedTextBox, inputBuffer);
        focusedTextBox->cursorIndex = cursor;
        ituWidgetSetDirty(focusedTextBox, true);
    }
}

ITUTextBox* sHelpPopupBox = NULL;
int popupPage = -1;

void popupHelp(uint8_t code)
{
    if(code == KEYCODE_ESCAPE)
    {
        popupPage = -1;
        ituWidgetSetVisible(&(sHelpPopupBox->text.widget), false);
        return;
    }
    ++popupPage;

	if(sHelpPopupBox == NULL) 
        sHelpPopupBox = ituSceneFindWidget(&theScene, "HelpTextBox");

    //ituTextSetBackColor(&(sHelpPopupBox->text), 0xff, 0x40, 0x40, 0x40);

    switch(popupPage)
    {
        case -1:
            ituWidgetSetVisible(&(sHelpPopupBox->text.widget), false);
            break;
        case 0:
            ituTextBoxSetString(sHelpPopupBox, 
"기기 네트워크 설정\n\n\
devip     기기의 IP 주소를 설정합니다.\n\
  예) devip 192.168.0.156\n\n\
devgw     기기의 Gateway를 설정합니다.\n\
  예) devgw 192.168.001.001\n\n\
devsm     기기의 Subnet Mask를 설정합니다.\n\
  예) devsm 255.255.255.0\n\n\
devmac    기기의 MAC 주소를 설정합니다.\n\
  예) devmac 01:12:34:56:78:90\n\n\
devdhcp   기기의 MAC 주소를 설정합니다.\n\
  예) devmac 12:34:56:78:90:af\n\n\n\
  --> ESC키를 누르면 도움말 창을 닫습니다.\n\
  --> F1키를 누르면 다음 페이지로 넘어갑니다. (1/4)");
            ituWidgetSetVisible(&(sHelpPopupBox->text.widget), true);
            break;
        case 1:
            ituTextBoxSetString(sHelpPopupBox, 
"서버 네트워크 설정\n\n\
srvaddr   서버의 주소를 설정합니다.\n\
  예) srvaddr com-me.tscontech.com\n\n\
srvport   서버의 Port 번호를 설정합니다.\n\
  예) srvport 5000\n\n\
ftpaddr   업데이트용 FTP 서버의 주소를 설정합니다.\n\
  예) ftpaddr csms-ftp.tscontech.com\n\
ftpid     업데이트용 FTP 서버의 접속용 ID를 설정합니다.\n\
  예) ftpid cptsct\n\n\
ftppw     업데이트용 FTP 서버의 접속용 비밀번호를 설정합니다.\n\
  예) ftppw cp01340\n\n\n\
  --> ESC키를 누르면 도움말 창을 닫습니다.\n\
  --> F1키를 누르면 다음 페이지로 넘어갑니다. (2/4)");
            //ituWidgetSetVisible(true);
            break;
        case 2:
            ituTextBoxSetString(sHelpPopupBox,
"기기 내부 설정\n\n\
csid      8자리 숫자로 구성된 충전소 ID를 설정합니다.\n\
  예) csid 00000001\n\n\
devid     2자리 숫자로 구성된 충전기 ID를 설정합니다.\n\
  예) devid 00\n\n\
maxpower  충전기의 최대 출력을 설정합니다. 단위는 KW입니다.\n\
  예) maxpower 30\n\n\
mode      충전기의 운용모드를 설정합니다.\n\
    \t\tonline : 일반모드입니다.\n\
    \t\toffline : 비공용모드입니다.\n\
    \t\tfree : 무료충전모드입니다.\n\
  예) mode online\n\n\
card      비공용모드에서 사용할 카드번호를 등록합니다.\n\
          번호를 입력하지 않으면 저장된 카드번호가 삭제됩니다.\n\
  예) card 1234-5678-9012-3456\n\n\
freetime  무료충전모드일 때 최대 충전시간을 분 단위로 설정합니다.\n\
          0을 입력하면 무제한입니다. 최대 99분까지 가능합니다.\n\
  예) freetime 50\n\n\n\
  --> ESC키를 누르면 도움말 창을 닫습니다.\n\
  --> F1키를 누르면 다음 페이지로 넘어갑니다. (3/4)");
            //ituWidgetSetVisible(true);
            break;
        case 3:
            ituTextBoxSetString(sHelpPopupBox,
"테스트 및 기타\n\n\
testnet   네트워크 테스트를 시작합니다.\n\
  예) testnet\n\n\
testio    입출력테스트를 시작합니다.\n\
  예) testio\n\n\
help      해당 명령의 자세한 설명을 표시합니다.\n\
  예) testio\n\n\
각 설정의 현재 값을 알고 싶다면 명령만 입력합니다.\n\
  예) 현재 충전소 ID를 알고 싶다면\n\
      csid\n\
      만 입력합니다.\n\n\n\
  --> F1키 또는 ESC키를 누르면 도움말 화면을 닫습니다. (4/4)");
            //ituWidgetSetVisible(true);
#if 0
        case 3:
            ituTextSetString(sHelpPopupBox, 
"OCPP 설정\n\n\
ocppaddr  OCPP 프로토콜을 통해 데이터를 전송할 서버의 주소를 설정합니다. \n\
      예) ws://www.w3.org/2005/08/addressing/anonymous \n\n\
ocppport  OCPP 프로토콜을 통해 데이터를 전송할 서버의 port 번호를 설정합니다.\n\
          0에서 65535까지 가능합니다. \n\
      예) 22555 \n\n\
");
            //ituWidgetSetVisible(true);
            break;
#endif
        default:
            ituWidgetSetVisible(&(sHelpPopupBox->text.widget), false);
            popupPage = -1;
    }
}

void moveCursor(uint32_t code)
{
    if(focusedTextBox)
    {
        if(code == KEYCODE_LEFT)  //Left Arrow Key
        {   
            if(focusedTextBox->cursorIndex > 0)
                focusedTextBox->cursorIndex--;
        }
        else  //Right Arrow Key
        {
            if(focusedTextBox->cursorIndex < strlen(ituTextBoxGetString(focusedTextBox)))
                focusedTextBox->cursorIndex++;
        }
        ituWidgetSetDirty(focusedTextBox, true);
    }
}

void commandInput(uint32_t flag, uint32_t code)
{
    if(flag == ITP_KEYUP) return;  

    char inputted = scancodeToChar(code);

    printf("%c, %d\n", inputted, code);

    if(inputted == NULL)
    {  //Not caractorizable key inputted -> Some special function or return immediately
        if(code == KEYCODE_F1 || (popupPage != -1 && code == KEYCODE_ESCAPE)) popupHelp(code); //F1 & ESC
        if(code == KEYCODE_LEFT || code == KEYCODE_RIGHT) moveCursor(code);
        return;
    }
    if(inputted == '\n')
    {  //Press Return key -> run command
        runInputted();
    }
    else
    {  //Press character key + some function key -> write buffer
        writeInputBuffer(inputted);
    }
    printf("%s\n", inputBuffer);
}
