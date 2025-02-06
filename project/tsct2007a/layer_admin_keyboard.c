#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ite/itp.h"
#include "tre/tre.h"
#include "scene.h"
#include "ctrlboard.h"
#include "cststring.h"
#include "tsctcfg.h"

/* widgets:
AdminKeyboardLayer
AdminSetListBackground
InfoPrice3Txt
InfoPrice2Txt
InfoPrice1Txt
ModelTxt
VersionTxt
AdminSetTestBackground
AdminSetchargerBackground
ForceLimitTxt
MaxPowerTxt
FreeChargeTimeTxt
Modetxt
AudioVolumnTxt
tSoctxt
LcdOnOffTimetxt
AuthTxt
DevIDTxt
StationIDTxt
AdminSetIpBackground
AdminSetIpContainer
Serverporttxt
ServerIPtxt
Macaddrtxt
chargersmtxt
chargergwtxt
chargeriptxt
Dhcptxt
*/

#if 1
//printf function for debug
#define DEBUGPRINT(x, ...) printf("[%s]" x "\n", __func__, ## __VA_ARGS__)  
#else
//no debug prints
#define DEBUGPRINT(x, ...) 
#endif

//Regular expressions for validation (POSIX Extended RegEx)
#define REGEX_SERVADDR  "((http(s)?|ws(s)?):\\/\\/)?([a-zA-Z0-9_-]+\\.)+[a-zA-Z0-9]+\\/?"   //Server URL RegEx
#define REGEX_FTPADDR   "(ftp:\\/\\/)?([a-zA-Z0-9_-]+\\.)+[a-zA-Z0-9]+\\/?"               //FTP Update Server URL RegEx
#define REGEX_IPV4ADDR  "(([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3}))"       //IPv4 Address RegEx
#define REGEX_MACADDR   "(([0-9a-fA-F]{2}\\:){5}([0-9a-fA-F]{2}))"                          //MAC Address RegEx

#define REGEX_MAXMATCH  6   //0 : Match, 1-5 : Group 1-5
regex_t regServAddr, regFTPAddr, regIPv4Addr, regMACAddr;
regmatch_t regMatch[REGEX_MAXMATCH];


#define SERVADDR_OP         "com-me.tscontech.com"      //Operation Server Address
#define SERVADDR_DEV        "dev-com-me.tscontech.com"  //Development Server Address
#define SERVADDR_OCTT       "ws://6631c3220041dc71.octt.openchargealliance.org"
#define SERVADDR_DEVOCPP    "ws://ocpp.tscontech.com"

//Keyboard
extern ITPKeyboardEvent UIKeyboardInput;   //Keyboard Input value
typedef void (*UIKeyboardBindFunc)(uint32_t flag, uint32_t code);   //Keyboard Bind function type for UI
extern UIKeyboardBindFunc UIKeyboardFunc;                                  //Keyboard Bind function for UI

//String and keyboard filter values using in setValueBackground
static char* settingMap[18][3] = 
{
    //Title, Info
    {   //Default
        "충전기 설정 변경", "충전기 설정을 변경합니다.\n모든 설정마다 입력 가능한 문자가 다릅니다.\n설명을 보고 바르게 입력해주세요.", (char*)KEYBOARD_FILTER_NONE
    },
    
    //Network
    {//Failback - Toggle
        "DHCP 설정 변경", "충전기의 DHCP 사용 여부를 결정합니다.\n0과 1만 입력할 수 있습니다.\n0이면 사용하지 않습니다.\n1이면 사용합니다.\n기본 값은 1입니다.", (char*)KEYBOARD_FILTER_BOOLEAN
    },
    {
        "충전기 IP 변경", "충전기 IP를 변경합니다.\n000.000.000.000 ~ 255.255.255.255\n사이의 값을 입력해주세요.",  (char*)KEYBOARD_FILTER_IP
    },
    {
        "충전기 GW 변경", "충전기 GW를 변경합니다.\n000.000.000.000 ~ 255.255.255.255\n사이의 값을 입력해주세요.",  (char*)KEYBOARD_FILTER_IP
    },
    {
        "충전기 SM 변경", "충전기 SM를 변경합니다.\n000.000.000.000 ~ 255.255.255.255\n사이의 값을 입력해주세요.", (char*)KEYBOARD_FILTER_IP
    },
    {
        "충전기 MAC 주소 변경", "충전기 MAC 주소를 변경합니다.\n00:00:00:00:00:00 ~ FF:FF:FF:FF:FF:FF\n사이의 값을 입력해주세요.", (char*)KEYBOARD_FILTER_MAC
    },
    {
        "서버 주소 변경", "서버 주소를 변경합니다.\nhttp:// 또는 ws://로 시작하는 웹 주소를 입력해주세요.\nF1키를 누르면 운영서버, F2키를 누르면 개발서버가 자동으로 입력됩니다.", (char*)KEYBOARD_FILTER_ADDRESS
    },
    {
        "서버 PORT 변경", "서버 PORT를 변경합니다.\n1 ~ 65535 사이의 값을 입력해주세요.", (char*)KEYBOARD_FILTER_NUMBER
    },

    //System
    {
        "충전소 ID 변경", "충전소ID를 변경합니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n정상적인 충전소 ID는 00000000 ~ 99999999 사이의 값입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {
        "충전기 ID 변경", "충전기ID를 변경합니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n정상적인 충전기 ID는 00 ~ 99 사이의 값입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {//Failback
        "인증 방식 변경", "인증 방식을 변경합니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n1은 서버 인증입니다.\n2는 로컬 RFID 카드 인증입니다.\n3은 로컬 비밀번호 인증입니다.", KEYBOARD_FILTER_NUMBER
    },
    {
        "화면 자동 꺼짐 변경", "화면이 꺼지는 시간을 분 단위로 변경합니다.\n0 ~ 99 사이의 값을 입력해주세요.\n기본 값은 10입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {
        "충전 SOC 변경", "15118 표준으로 충전 시 적용되는 목표 SOC를 변경합니다.\n배터리 SoC가 설정된 값에 도달하면 충전이 종료됩니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n0 ~ 100 사이의 값을 입력해주세요.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {
        "음량 변경", "스피커의 음량을 변경합니다.\n0 ~ 99 사이의 값을 입력해주세요.\n기본 값은 70입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {//Failback
        "운영 모드 변경", "충전기의 운영 모드을 변경합니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n0은 정상 운영입니다.\n1은 점검 모드입니다.\n2는 무료 충전 모드입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {
        "무료 충전 시간 변경", "무료 충전 모드에서의 최대 충전 시간을 변경합니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n0 ~ 99 사이의 값을 입력해주세요.\n0은 무제한입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {
        "최대 충전 전력 변경", "최대 충전 전력을 변경합니다.\n잘 모르겠다면 담당자에게 문의 바랍니다.\n0 ~ 7 사이의 값을 입력해주세요.\n기본 값은 7입니다.", (char*)KEYBOARD_FILTER_NUMBER
    },
    {//Failback - Toggle
        "강제 전력 제한 변경", "최대 충전 전력 초과 시 전력을 강제로 제어할지 여부를 결정합니다.\n0과 1만 입력할 수 있습니다.\n0이면 강제 제어를 하지 않습니다.\n1이면 강제 제어를 시도합니다.\n0은 기본 값은 0입니다.", (char*)KEYBOARD_FILTER_BOOLEAN
    }
};

typedef enum{
    CHANGE_CONFIG_NONE,
    
    CHANGE_CONFIG_START_NETWORK,
    CHANGE_CONFIG_DHCP = 1,
    CHANGE_CONFIG_DEVIP,
    CHANGE_CONFIG_DEVGW,
    CHANGE_CONFIG_DEVSM,
    CHANGE_CONFIG_DEVMAC,
    CHANGE_CONFIG_SERVADDR,
    CHANGE_CONFIG_SERVPORT,

    CHANGE_CONFIG_START_SYSTEM,
    CHANGE_CONFIG_STATIONID = 8,
    CHANGE_CONFIG_DEVID,
    CHANGE_CONFIG_AUTH,
    CHANGE_CONFIG_SCREENSAVER,
    CHANGE_CONFIG_SOC,
    CHANGE_CONFIG_VOLUMN,
    CHANGE_CONFIG_OPERATION,
    CHANGE_CONFIG_FREETIME,
    CHANGE_CONFIG_MAXPOWER,
    CHANGE_CONFIG_FORCELIMIT,

    // CHANGE_CONFIG_START_TEST,
    // CHANGE_CONFIG_COMMTEST = 18,
    // CHANGE_CONFIG_IOTEST,

    // CHANGE_CONFIG_START_FTP,
    // CHANGE_CONFIG_FTP = 20,

    CHANGE_CONFIG_END
} CHANGE_CONFIG;
CHANGE_CONFIG nowConfig;  //Which data is editing now? CHANGE_CONFIG_NONE means not editing.

//Main backgrounds
ITUBackground* backgrounds[4]; //{networkBackground, systemBackground, testBackground, infoBackground}

ITUBackground* nowBackground;  //Showing background.

//ITUBackground* networkBackground;
ITUText* dhcpText, *devIPText, *devGWText, *devSMText, *devMACText, *serverIPText, *serverPortText;

//ITUBackground* systemBackground;
ITUText* stationIDText, *devIDText, *authText, *lcdOnOffTimeText, *socText, 
            *audioVolumnText, *modeText, *freeChargeTimeText, *maxPowerText, *forceLimitText;

//ITUBackground* testBackground;

//ITUBackground* infoBackground;
ITUText* versionText, *modelText, *infoPrice1Text, *infoPrice2Text, *infoPrice3Text;


ITUBackground* setValueBackground;      //Background that can edit config data
ITUText* titleText;                     //Edit value title
ITUTextBox* infoTextBox, *valueTextBox;

ITUBackground* testCommBackground, *testIOBackground;           //Backgrounds that can test some functions.
ITUText* amiTestText, *rfidTestText, *amiWattText;
ITUText* ledTestText, *outputMCTestText, *plcPowerTestText, *backlightTestText;

ITUBackground* ftpUpdateBackground;     //Background that can update via ftp
ITUTextBox* ftpTextBoxes[3];            //ftpAddrTextBox, ftpIDTextBox, ftpPWTextBox;
int focusedFTPTextBoxIndex = 0;         //Which textbox is selected and can edit now?

ITUTextBox* focusedTextBox;

typedef enum ADMIN_PAGE
{
    ADMIN_PAGE_NETWORK =    1,  //0b0000'0001,
    ADMIN_PAGE_SYSTEM =     2,  //0b0000'0010,
    ADMIN_PAGE_TEST =       4,  //0b0000'0100,
    ADMIN_PAGE_INFO =       8,  //0b0000'1000,
    ADMIN_PAGE_CHANGE_NET = 17, //0b0001'0001,
    ADMIN_PAGE_CHANGE_SYS = 18, //0b0001'0010,
    ADMIN_PAGE_TEST_COMM =  20, //0b0001'0100,
    ADMIN_PAGE_TEST_IO =    36, //0b0010'0100,
    ADMIN_PAGE_FTPUPDATE =  24, //0b0001'1000,
} ADMIN_PAGE;

int page;                   //Which page is showing?

extern bool bAmiErrChk;     //for check ami test

void resetMatch()
{
    for(int i = 0; i < REGEX_MAXMATCH; ++i)
    {
        regMatch[i].rm_so = -1;
        regMatch[i].rm_eo = -1;
    }
}

void printMatch()
{
    printf("[printMatch]\nMatch = (%d, %d)\n", regMatch[0].rm_so, regMatch[0].rm_eo);
    for(int i = 1; i < REGEX_MAXMATCH; ++i)
    {
        printf("Group %d = (%d, %d)\n", i, regMatch[i].rm_so, regMatch[i].rm_eo);
    }
}

void passKeyboard(uint32_t flag, uint32_t code)
{
    UIKeyboardInput.code = code;
    UIKeyboardInput.flags = flag;
}

inline AdminPageToIntPage(ADMIN_PAGE ap)
{
    switch(ap)
    {
        case ADMIN_PAGE_NETWORK : return 0;
        case ADMIN_PAGE_SYSTEM : return 1;
        case ADMIN_PAGE_TEST : return 2;
        case ADMIN_PAGE_INFO : return 3;

        case ADMIN_PAGE_CHANGE_NET : //fall-through
        case ADMIN_PAGE_CHANGE_SYS : return 4;

        case ADMIN_PAGE_TEST_COMM : return 5;
        case ADMIN_PAGE_TEST_IO : return 6;

        case ADMIN_PAGE_FTPUPDATE : return 7;
        
    }
}


/**
 * @brief Make values in screen to up-to-date
 * 
 */
void updateUI()
{
    char buf[100];
    uint32_t amiBuf;

    DEBUGPRINT("Enter");
    
    switch(page)
    {
        case ADMIN_PAGE_NETWORK : 
            ituTextSetString(devIPText, theConfig.ipaddr);

            ituTextSetString(devGWText, theConfig.gw);

            ituTextSetString(devSMText, theConfig.netmask);

            if(theConfig.dhcp == true)
            {
                ituTextSetString(dhcpText, "ON");
                ituWidgetSetVisible(devIPText, false);
                ituWidgetSetVisible(devGWText, false);
                ituWidgetSetVisible(devSMText, false);
            }
            else
            {
                ituTextSetString(dhcpText, "OFF");
                ituWidgetSetVisible(devIPText, true);
                ituWidgetSetVisible(devGWText, true);
                ituWidgetSetVisible(devSMText, true);
            }

            ituTextSetString(devMACText, theConfig.chargermac);

            ituTextSetString(serverIPText, theConfig.serverip);

            sprintf(buf, "%d", theConfig.serverport);
            ituTextSetString(serverPortText, buf);

            break;

        case ADMIN_PAGE_SYSTEM : 
            ituTextSetString(stationIDText, theConfig.siteid);

            ituTextSetString(devIDText, theConfig.devid1);

            switch(theConfig.ConfirmSelect)
            {
                case USER_AUTH_CARD : 
                    ituTextSetString(authText, "RFID");
                    break;
                case USER_AUTH_PASSWORD : 
                    ituTextSetString(authText, "비밀번호");
                    break;
                case USER_AUTH_NET_QR : 
                    ituTextSetString(authText, "QR");
                    break;
                case USER_AUTH_NET_REMOTE : 
                    ituTextSetString(authText, "원격");
                    break;
                case USER_AUTH_NET_BOTH : 
                    ituTextSetString(authText, "QR원격");
                    break;
                default :
                    theConfig.ConfirmSelect = USER_AUTH_NET;
                // case USER_AUTH_NET :
                    ituTextSetString(authText, "서버");
                    break;
            }

            sprintf(buf, "%d 분", theConfig.screensaver_time);
            ituTextSetString(lcdOnOffTimeText, buf);

            sprintf(buf, "%d %%", theConfig.targetSoc);
            ituTextSetString(socText, buf);

            sprintf(buf, "%d", theConfig.audiolevel);
            ituTextSetString(audioVolumnText, buf);

            switch(theConfig.OperationMode)
            {
                case OP_CHECK_MODE : 
                    ituTextSetString(modeText, "점검");
                    break;
                case OP_FREE_MODE : 
                    ituTextSetString(modeText, "무료");
                    break;
                default : 
                    theConfig.OperationMode = OP_NORMAL_MODE;
                // case OP_NORMAL_MODE : 
                    ituTextSetString(modeText, "일반");
                    break;
            }

            sprintf(buf, "%d 분", theConfig.FreeChargingTime);
            ituTextSetString(freeChargeTimeText, buf);

            sprintf(buf, "%d kW", theConfig.maxPower);
            ituTextSetString(maxPowerText, buf);

            if(theConfig.forcePowerLimit)
                ituTextSetString(forceLimitText, "ON");
            else
                ituTextSetString(forceLimitText, "OFF");
            
            break;

        case ADMIN_PAGE_TEST_COMM : 
            amiBuf = GetAMIValue();

            if(amiBuf == -1)
            {
                ituTextSetString(amiWattText, "0.00 kWh");
            }
            else 
            {
                sprintf(buf, "%d.%d kWh", amiBuf/100, amiBuf % 100);
                ituTextSetString(amiWattText, buf);
            }

            break;

        case ADMIN_PAGE_TEST_IO : 
            if(LEDStatus())
                ituTextSetString(ledTestText, "ON");
            else
                ituTextSetString(ledTestText, "OFF");

            if(CstGetMcstatus())
                ituTextSetString(outputMCTestText, "ON");
            else
                ituTextSetString(outputMCTestText, "OFF");

            if(PLCPowerStatus()) 
                ituTextSetString(plcPowerTestText, "ON");
            else 
                ituTextSetString(plcPowerTestText, "OFF");

            if(!ScreenIsOff()) 
                ituTextSetString(backlightTestText, "ON");
            else 
                ituTextSetString(backlightTestText, "OFF");
            
            break;
    }
}

/**
 * @brief Ready to use setValueBackground
 * 
 */
void prepareSetValueBackground()
{
    DEBUGPRINT("Show Background Enter");
    char buf[200];

    ituTextSetString(titleText, settingMap[nowConfig][0]);
    ituTextBoxSetString(infoTextBox, settingMap[nowConfig][1]);
    setKeyboardFilter((KEYBOARD_FILTER)settingMap[nowConfig][2]);
    focusedTextBox = valueTextBox;
    DEBUGPRINT("title = %s", settingMap[nowConfig][0]);
    DEBUGPRINT("info = %s", settingMap[nowConfig][1]);
    DEBUGPRINT("filter = %x", (KEYBOARD_FILTER)settingMap[nowConfig][2]);

    //Fix Textbox bug
    infoTextBox->text.stringSet->strings[infoTextBox->text.lang] = infoTextBox->text.string;
    ituDirtyWidget(infoTextBox, true);

    //reset Match
    resetMatch();

    switch(nowConfig)
    {
        case CHANGE_CONFIG_DHCP :
            if(theConfig.dhcp == true)
                ituTextBoxSetString(focusedTextBox, "1");
            else
                ituTextBoxSetString(focusedTextBox, "0");
            break;
        case CHANGE_CONFIG_DEVIP :
            ituTextBoxSetString(focusedTextBox, theConfig.ipaddr);
            break;
        case CHANGE_CONFIG_DEVGW : 
            ituTextBoxSetString(focusedTextBox, theConfig.gw);
            break;
        case CHANGE_CONFIG_DEVSM : 
            ituTextBoxSetString(focusedTextBox, theConfig.netmask);
            break;
        case CHANGE_CONFIG_DEVMAC : 
            ituTextBoxSetString(focusedTextBox, theConfig.chargermac);
            break;
        case CHANGE_CONFIG_SERVADDR : 
            ituTextBoxSetString(focusedTextBox, theConfig.serverip);
            break;
        case CHANGE_CONFIG_SERVPORT : 
            sprintf(buf, "%d", theConfig.serverport);
            ituTextBoxSetString(focusedTextBox, buf);
            break;

        case CHANGE_CONFIG_STATIONID : 
            ituTextBoxSetString(focusedTextBox, theConfig.siteid);
            break;
        case CHANGE_CONFIG_DEVID : 
            ituTextBoxSetString(focusedTextBox, theConfig.devid1);
            break;
        case CHANGE_CONFIG_AUTH : 
            sprintf(buf, "%d", theConfig.ConfirmSelect);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_SCREENSAVER : 
            sprintf(buf, "%d", theConfig.screensaver_time);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_SOC : 
            sprintf(buf, "%d", theConfig.targetSoc);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_VOLUMN : 
            sprintf(buf, "%d", theConfig.audiolevel);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_OPERATION : 
            sprintf(buf, "%d", theConfig.OperationMode);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_FREETIME : 
            sprintf(buf, "%d", theConfig.FreeChargingTime);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_MAXPOWER : 
            sprintf(buf, "%d", theConfig.maxPower);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        case CHANGE_CONFIG_FORCELIMIT : 
            sprintf(buf, "%d", theConfig.forcePowerLimit);
            ituTextBoxSetString(focusedTextBox, buf);
            break;
        default :
            break;
    }
    DEBUGPRINT("Show Background Exit");
}

/**
 * @brief Ready to use ftpUpdateBackground
 * 
 */
void prepareFTPUpdateBackground()
{
    //Set value in textboxes
    ituTextBoxSetString(ftpTextBoxes[0], theConfig.ftpDns);
    ituTextBoxSetString(ftpTextBoxes[1], theConfig.ftpId);
    ituTextBoxSetString(ftpTextBoxes[2], theConfig.ftpPw);

    //Make textbox cursor visible
    focusedFTPTextBoxIndex = 0;
    focusedTextBox = ftpTextBoxes[focusedFTPTextBoxIndex];
    focusedTextBox->textboxFlags |= ITU_TEXTBOX_CURSOR;
    ituWidgetSetVisible(focusedTextBox, true);

    //Set keyboard filter for first textbox
    setKeyboardFilter(KEYBOARD_FILTER_ADDRESS);
}

/**
 * @brief Input any value in Textbox
 * 
 * @param code Keycode
 */
void inputValue(uint32_t code)
{
    DEBUGPRINT("code = %d", code);

    char codeChar[2] = {0,0};
    if(code == KEYCODE_LEFT)
    {
        if(focusedTextBox->cursorIndex > 0)
        {
            focusedTextBox->cursorIndex -= 1;
            ituDirtyWidget(focusedTextBox, true);
        }
        return;
    }

    if(code == KEYCODE_RIGHT)
    {
        if(focusedTextBox->cursorIndex < strlen(focusedTextBox->text.string))
        {
            focusedTextBox->cursorIndex += 1;
            ituDirtyWidget(focusedTextBox, true);
        }
        return;
    }

    if(code == KEYCODE_BACKSPACE)
    {
        ituTextBoxInput(focusedTextBox, "\b");
        return;
    }

    if(nowConfig == CHANGE_CONFIG_SERVADDR)
    {
        if(code == KEYCODE_F1)
        {
            ituTextBoxSetString(focusedTextBox, SERVADDR_OP);
        }
        if(code == KEYCODE_F2)
        {
            ituTextBoxSetString(focusedTextBox, SERVADDR_DEV);
        }
        if(code == KEYCODE_F3)
        {
            ituTextBoxSetString(focusedTextBox, SERVADDR_OCTT);
        }
        if(code == KEYCODE_F4)
        {
            ituTextBoxSetString(focusedTextBox, SERVADDR_DEVOCPP);
        }
    }

    *codeChar = keyboardFilter(code);
    if(*codeChar)
        ituTextBoxInput(focusedTextBox, codeChar);
    
    return;
}

/**
 * @brief Save vaule when input enter key on setValueBackground
 * 
 */
void saveValue()
{
    DEBUGPRINT("Save Enter, nowConfig = %d", nowConfig);
    char* text = ituTextBoxGetString(focusedTextBox);
    int checkInt, treError;
    bool isPassed = true;

    switch(nowConfig)
    {
        case CHANGE_CONFIG_DHCP :
            checkInt = atoi(text);
            theConfig.dhcp = (checkInt == 0) ? false : true;
            break;

        case CHANGE_CONFIG_DEVIP :

            treError = tre_regexec(&regIPv4Addr, text, REGEX_MAXMATCH, regMatch, 0);
            DEBUGPRINT("DEV IP start = %d, end = %d, startch = %d, endch = %d, text = %s", regMatch[0].rm_so, regMatch[0].rm_eo, text[regMatch[0].rm_so], text[regMatch[0].rm_eo], text);
            if(treError == REG_NOMATCH)
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("DEV IP FAILED - Not Matched Rule");
                break;
            }

            if(treError != REG_OK)
            {   //RegEx Engine Error
                //TODO: 사용자 안내
                DEBUGPRINT("DEV IP FAILED - Error %d", treError);
                break;
            }

            if( (regMatch[0].rm_so != 0) || (text[regMatch[0].rm_eo] != '\0') )
            {   //Check the other value is in
                //TODO: 사용자 안내
                DEBUGPRINT("DEV IP FAILED - Other Value Detected");
                break;
            }

            //Check each class value is in 0-255
            //TODO: 사용자 안내
            for(int i = 1; i < 5; ++i)
            {
                checkInt = atoi(text[regMatch[i].rm_so]);
                if( (checkInt > 255) || (checkInt < 0) )
                {
                    DEBUGPRINT("DEV IP FAILED - Not Matched Rule(Not in 0-255 at %c CLASS)", (i + 'A' - 1));
                    isPassed = false;
                    break;
                }
            }
            if(!isPassed) break;


            //Pass
            strncpy(theConfig.ipaddr, text, sizeof(theConfig.ipaddr)-1);
            theConfig.ipaddr[sizeof(theConfig.ipaddr)-1] = 0;
            break;

        case CHANGE_CONFIG_DEVGW : 

            treError = tre_regexec(&regIPv4Addr, text, REGEX_MAXMATCH, regMatch, 0);
            DEBUGPRINT("DEV GW start = %d, end = %d, startch = %d, endch = %d, text = %s", regMatch[0].rm_so, regMatch[0].rm_eo, text[regMatch[0].rm_so], text[regMatch[0].rm_eo], text);
            if(treError == REG_NOMATCH)
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("DEV GW FAILED - Not Matched Rule");
                break;
            }

            if(treError != REG_OK)
            {   //RegEx Engine Error
                //TODO: 사용자 안내
                DEBUGPRINT("DEV GW FAILED - Error %d", treError);
                break;
            }

            if( (regMatch[0].rm_so != 0) || (text[regMatch[0].rm_eo] != '\0') )
            {   //Check the other value is in
                //TODO: 사용자 안내
                DEBUGPRINT("DEV GW FAILED - Other Value Detected");
                break;
            }

            //Check each class value is in 0-255
            //TODO: 사용자 안내
            for(int i = 1; i < 5; ++i)
            {
                checkInt = atoi(text[regMatch[i].rm_so]);
                if( (checkInt > 255) || (checkInt < 0) )
                {
                    DEBUGPRINT("DEV GW FAILED - Not Matched Rule(Not in 0-255 at %c CLASS)", (i + 'A' - 1));
                    isPassed = false;
                    break;
                }
            }
            if(!isPassed) break;


            //Pass
            strncpy(theConfig.gw, text, sizeof(theConfig.gw)-1);
            theConfig.gw[sizeof(theConfig.gw)-1] = 0;
            break;

        case CHANGE_CONFIG_DEVSM : 

            treError = tre_regexec(&regIPv4Addr, text, REGEX_MAXMATCH, regMatch, 0);
            DEBUGPRINT("DEV SM start = %d, end = %d, startch = %d, endch = %d, text = %s", regMatch[0].rm_so, regMatch[0].rm_eo, text[regMatch[0].rm_so], text[regMatch[0].rm_eo], text);
            if(treError == REG_NOMATCH)
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("DEV SM FAILED - Not Matched Rule");
                isPassed = false;
                break;
            }

            if(treError != REG_OK)
            {   //RegEx Engine Error
                //TODO: 사용자 안내
                DEBUGPRINT("DEV SM FAILED - Error %d", treError);
                isPassed = false;
                break;
            }

            if( (regMatch[0].rm_so != 0) || (text[regMatch[0].rm_eo] != '\0') )
            {   //Check the other value is in
                //TODO: 사용자 안내
                DEBUGPRINT("DEV SM FAILED - Other Value Detected");
                isPassed = false;
                break;
            }

            //Check each class value is in 0-255
            //TODO: 사용자 안내
            for(int i = 1; i < 5; ++i)
            {
                checkInt = atoi(text[regMatch[i].rm_so]);
                if( (checkInt > 255) || (checkInt < 0) )
                {
                    DEBUGPRINT("DEV SM FAILED - Not Matched Rule(Not in 0-255 at %c CLASS)", (i + 'A' - 1));
                    isPassed = false;
                    break;
                }
            }
            if(!isPassed) break;


            //Pass
            strncpy(theConfig.netmask, text, sizeof(theConfig.netmask)-1);
            theConfig.netmask[sizeof(theConfig.netmask)-1] = 0;
            break;

        case CHANGE_CONFIG_DEVMAC : 

            treError = tre_regexec(&regMACAddr, text, REGEX_MAXMATCH, regMatch, 0);
            DEBUGPRINT("DEV MAC start = %d, end = %d, startch = %d, endch = %d, text = %s", regMatch[0].rm_so, regMatch[0].rm_eo, text[regMatch[0].rm_so], text[regMatch[0].rm_eo], text);
            if(treError == REG_NOMATCH)
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("DEV MAC FAILED - Not Matched Rule");
                break;
            }

            if(treError != REG_OK)
            {   //RegEx Engine Error
                //TODO: 사용자 안내
                DEBUGPRINT("DEV MAC FAILED - Error %d", treError);
                break;
            }

            if( (regMatch[0].rm_so != 0) || (text[regMatch[0].rm_eo] != '\0') )
            {   //Check the other value is in
                //TODO: 사용자 안내
                DEBUGPRINT("DEV MAC FAILED - Other Value Detected");
                break;
            }

            //Pass
            strncpy(theConfig.chargermac, text, sizeof(theConfig.chargermac)-1);
            theConfig.chargermac[sizeof(theConfig.chargermac)-1] = 0;
            break;

        case CHANGE_CONFIG_SERVADDR : 

            treError = tre_regexec(&regServAddr, text, REGEX_MAXMATCH, regMatch, 0);
            DEBUGPRINT("SERV ADDR start = %d, end = %d, startch = %d, endch = %d, text = %s", regMatch[0].rm_so, regMatch[0].rm_eo, text[regMatch[0].rm_so], text[regMatch[0].rm_eo], text);
            if(treError == REG_NOMATCH)
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("SERV ADDR FAILED - Not Matched Rule");
                break;
            }

            if(treError != REG_OK)
            {   //RegEx Engine Error
                //TODO: 사용자 안내
                DEBUGPRINT("SERV ADDR FAILED - Error %d", treError);
                break;
            }

            if( (regMatch[0].rm_so != 0) || (text[regMatch[0].rm_eo] != '\0') )
            {   //Check the other value is in
                //TODO: 사용자 안내
                DEBUGPRINT("SERV ADDR FAILED - Other Value Detected");
                DEBUGPRINT("%s", text + regMatch[0].rm_so);
                printMatch();
                break;
            }

            //Pass
            strncpy(theConfig.serverip, text, sizeof(theConfig.serverip)-1);
            theConfig.serverip[sizeof(theConfig.serverip)-1] = 0;
            break;

        case CHANGE_CONFIG_SERVPORT : 
            checkInt = atoi(text);
            if( (checkInt > 65535) || (checkInt < 0))
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("SERV PORT FAILED - Not in 0-65535");
                break;
            }
            theConfig.serverport = atoi(text);
            break;

        case CHANGE_CONFIG_STATIONID : 
            if(strlen(text) != 8)
            {   //Check the value is matched to rule
                //TODO: 사용자 안내
                DEBUGPRINT("STATION ID FAILED - Not 8 digits");
                break;
            }
            strncpy(theConfig.siteid, text, sizeof(theConfig.siteid)-1);
            theConfig.siteid[sizeof(theConfig.siteid)-1] = 0;
            break;

        case CHANGE_CONFIG_DEVID : 
            if(strlen(text) != 2)
            {
                //TODO: 사용자 안내
                DEBUGPRINT("DEV ID FAILED - Not 2 digits");
                break;
            }
            // theConfig.devid1[0] = text[0];
            // theConfig.devid1[1] = text[1];
            strncpy(theConfig.devid1, text, sizeof(theConfig.devid1)-1);
            theConfig.devid1[sizeof(theConfig.devid1)-1] = 0;
            break;

        case CHANGE_CONFIG_AUTH : 
            checkInt = atoi(text);
            if(checkInt >= USER_AUTH_MAX) checkInt = USER_AUTH_NET;  //default to 1 - Server
            else if (checkInt <= USER_AUTH_NONE) checkInt = USER_AUTH_NET;  //default to 1 - Server
            theConfig.ConfirmSelect = atoi(text);
            break;

        case CHANGE_CONFIG_SCREENSAVER : 
            checkInt = atoi(text);
            if(checkInt > 99) checkInt = 99;
            else if (checkInt < 0) checkInt = 0;
            theConfig.screensaver_time = checkInt;
            break;

        case CHANGE_CONFIG_SOC : 
            checkInt = atoi(text);
            if(checkInt > 100) checkInt = 100;
            else if (checkInt < 0) checkInt = 0;
            theConfig.targetSoc = checkInt;
            break;

        case CHANGE_CONFIG_VOLUMN : 
            checkInt = atoi(text);
            if(checkInt > 100) checkInt = 100;
            else if (checkInt < 0) checkInt = 0;
            theConfig.audiolevel = checkInt;
            if(theConfig.audiolevel)
            break;

        case CHANGE_CONFIG_OPERATION : 
            checkInt = atoi(text);
            if(checkInt > OP_FREE_MODE) checkInt = OP_NORMAL_MODE;  //default to 0 - Normal
            else if (checkInt < OP_NORMAL_MODE) checkInt = OP_NORMAL_MODE; //default to 0 - Normal
            theConfig.OperationMode  = checkInt;
            break;

        case CHANGE_CONFIG_FREETIME : 
            checkInt = atoi(text);
            if(checkInt > 99) checkInt = 99;
            else if (checkInt < 0) checkInt = 0;
            theConfig.FreeChargingTime = checkInt;
            break;

        case CHANGE_CONFIG_MAXPOWER : 
            checkInt = atoi(text);
            if(checkInt > 7) checkInt = 7;
            else if (checkInt < 1) checkInt = 1;
            theConfig.maxPower = checkInt;
            break;

        case CHANGE_CONFIG_FORCELIMIT : 
            checkInt = atoi(text);
            theConfig.forcePowerLimit = (checkInt == 0) ? false : true;
            break;

        default :
            break;

    }

    DEBUGPRINT("text = %s, checkInt = %d", text, checkInt);

    // page &= 0b1111;

    // ituWidgetSetVisible(nowBackground, false);
    // if(page == ADMIN_PAGE_SYSTEM)
    // {
    //     nowBackground = backgrounds[1];
    // }
    // else
    // {   //Networt and any error values
    //     nowBackground = backgrounds[0];
    // }
    // ituWidgetSetVisible(nowBackground, true);
}

void updateViaFTP()
{
    DEBUGPRINT("Update Enter");

    char* text = ftpTextBoxes[0]->text.string;
    int treError;

    treError = tre_regexec(&regFTPAddr, text, REGEX_MAXMATCH, regMatch, 0);
    DEBUGPRINT("FTP ADDR start = %d, end = %d, startch = %d, endch = %d, text = %s", regMatch[0].rm_so, regMatch[0].rm_eo, text[regMatch[0].rm_so], text[regMatch[0].rm_eo], text);
    if(treError == REG_NOMATCH)
    {   //Check the value is matched to rule
        //TODO: 사용자 안내
        DEBUGPRINT("FTP ADDR FAILED - Not Matched Rule");
        ituWidgetSetVisible(nowBackground, false);
        page &= 0b1111;
        nowBackground = backgrounds[AdminPageToIntPage(page)];
        ituWidgetSetVisible(nowBackground, true);
        return;
    }

    if(treError != REG_OK)
    {   //RegEx Engine Error
        //TODO: 사용자 안내
        DEBUGPRINT("FTP ADDR FAILED - Error %d", treError);
        ituWidgetSetVisible(nowBackground, false);
        page &= 0b1111;
        nowBackground = backgrounds[AdminPageToIntPage(page)];
        ituWidgetSetVisible(nowBackground, true);
        return;
    }


    if( (regMatch[0].rm_so != 0) || (text[regMatch[0].rm_eo] != '\0') )
    {   //Check the other value is in
        //TODO: 사용자 안내
        DEBUGPRINT("FTP ADDR FAILED - Other Value Detected");
        ituWidgetSetVisible(nowBackground, false);
        page &= 0b1111;
        nowBackground = backgrounds[AdminPageToIntPage(page)];
        ituWidgetSetVisible(nowBackground, true);
        return;
    }

    //Change Config
	strcpy(theConfig.ftpDns, ftpTextBoxes[0]->text.string);
	strcpy(theConfig.ftpId, ftpTextBoxes[1]->text.string);
	strcpy(theConfig.ftpPw, ftpTextBoxes[2]->text.string);
	strcpy(theConfig.ftpPath, "/fw/1007g1/ITEPKG03.PKG");

	printf("FTP IP : %s, %s  \n", theConfig.ftpDns, ftpTextBoxes[0]->text.string);
	printf("FTP ID : %s, %s  \n", theConfig.ftpId, ftpTextBoxes[1]->text.string);
	printf("FTP PW : %s, %s  \n", theConfig.ftpPw, ftpTextBoxes[2]->text.string);
	
	ConfigSave();

    //Do update
	bFTPupgrade = FTP_FW_UD_WAIT_CON;	
	
	ituWidgetSetVisible(nowBackground, false);
	
	FtpFwUpdate_func();

	// reset
	printf("\n\n soon reset....\n\n");
	usleep(2000*1000);
	custom_reboot();
	
	while(1);
}

void runNumber_NET(uint32_t code)
{
    DEBUGPRINT("code = %d", code);
	switch(code)
	{
		case KEYCODE_1 : //Toggle menu
			if(theConfig.dhcp == true)
            {
                theConfig.dhcp = false;
                ituTextSetString(dhcpText, "OFF");
                ituWidgetSetVisible(devIPText, true);
                ituWidgetSetVisible(devGWText, true);
                ituWidgetSetVisible(devSMText, true);
            }
            else
            {
                theConfig.dhcp = true;
                ituTextSetString(dhcpText, "ON");
                ituWidgetSetVisible(devIPText, false);
                ituWidgetSetVisible(devGWText, false);
                ituWidgetSetVisible(devSMText, false);
            }
            nowConfig = CHANGE_CONFIG_NONE;
			return;

		//Dependent on DHCP
		case KEYCODE_2 : //Dev IP
		case KEYCODE_3 : //Dev GW
		case KEYCODE_4 : //Dev SM
            if(theConfig.dhcp == true)  //if DHCP enabled, Do nothing.
                return;
            //else, fall-through
		case KEYCODE_5 : //Dev MAC
		case KEYCODE_6 : //Server Addr
        case KEYCODE_7 : //Server Port
            nowConfig = CHANGE_CONFIG_START_NETWORK + code - KEYCODE_1;
            page = ADMIN_PAGE_CHANGE_NET;
            prepareSetValueBackground();

            ituWidgetSetVisible(nowBackground, false);
            page |= 0b00010000;
            nowBackground = setValueBackground;
            ituWidgetSetVisible(nowBackground, true);

            return;

		//No Action
		default : 
            nowConfig = CHANGE_CONFIG_NONE;
			break;
	}
    return;
}

void runNumber_SYS(uint32_t code)
{
    DEBUGPRINT("code = %d", code);
	switch(code)
	{
		//Toggle
		case KEYCODE_3 : //Auth
			++theConfig.ConfirmSelect;
			switch(theConfig.ConfirmSelect)
			{
				case USER_AUTH_CARD : 
					ituTextSetString(authText, "RFID");
					break;
				case USER_AUTH_PASSWORD : 
					ituTextSetString(authText, "비밀번호");
					break;
				case USER_AUTH_NET_QR : 
					ituTextSetString(authText, "QR");
					break;
				case USER_AUTH_NET_REMOTE : 
					ituTextSetString(authText, "원격");
					break;
				case USER_AUTH_NET_BOTH : 
					ituTextSetString(authText, "QR원격");
					break;
				default :
					theConfig.ConfirmSelect = USER_AUTH_NET;
				// case USER_AUTH_NET :
					ituTextSetString(authText, "서버");
					break;
			}
			nowConfig = CHANGE_CONFIG_NONE;
			return;

		case KEYCODE_7 : //Mode
			++theConfig.OperationMode;
			switch(theConfig.OperationMode)
			{
				case OP_CHECK_MODE : 
					ituTextSetString(modeText, "점검");
					break;
				case OP_FREE_MODE : 
					ituTextSetString(modeText, "무료");
					break;
				default : 
					theConfig.OperationMode = OP_NORMAL_MODE;
				// case OP_NORMAL_MODE : 
					ituTextSetString(modeText, "일반");
					break;
			}
			nowConfig = CHANGE_CONFIG_NONE;
			return;

		case KEYCODE_0 : //ForceLimit
			theConfig.forcePowerLimit = !theConfig.forcePowerLimit;

			if(theConfig.forcePowerLimit)
				ituTextSetString(forceLimitText, "ON");
			else
				ituTextSetString(forceLimitText, "OFF");
			nowConfig = CHANGE_CONFIG_NONE;
			return;

		// Move Background
		case KEYCODE_1 : //Station ID
		case KEYCODE_2 : //Dev ID
		case KEYCODE_4 : //Screen Saver Time
		case KEYCODE_5 : //Target SOC
		case KEYCODE_6 : //Audio Volumn
		case KEYCODE_8 : //Free Charging Time
		case KEYCODE_9 : //Max Power
            nowConfig = CHANGE_CONFIG_START_SYSTEM  + (code - KEYCODE_1);
            page = ADMIN_PAGE_CHANGE_SYS;
    		prepareSetValueBackground();

            ituWidgetSetVisible(nowBackground, false);
            page |= 0b00010000;
            nowBackground = setValueBackground;
            ituWidgetSetVisible(nowBackground, true);
			break;
		
		default :  //No Action
            nowConfig = CHANGE_CONFIG_NONE;
			break;
	}
    return;
}

void runNumber_TEST(uint32_t code)
{
    DEBUGPRINT("code = %d", code);
    switch(code)
    {
        case KEYCODE_1 : 
            page = ADMIN_PAGE_TEST_COMM;
            ituWidgetSetVisible(nowBackground, false);
            nowBackground = testCommBackground;
            updateUI();
            break;

        case KEYCODE_2 : 
            page = ADMIN_PAGE_TEST_IO;
            ituWidgetSetVisible(nowBackground, false);
            nowBackground = testIOBackground;
            updateUI();
            break;

        default : 
            return;
    }
    ituWidgetSetVisible(nowBackground, true);
    return;    
}

void runNumber_INFO(uint32_t code)
{
    DEBUGPRINT("code = %d", code);

    if(code == KEYCODE_1)
    {
        prepareFTPUpdateBackground();
        page = ADMIN_PAGE_FTPUPDATE;
        ituWidgetSetVisible(nowBackground, false);
        nowBackground = ftpUpdateBackground;
        ituWidgetSetVisible(nowBackground, true);
    }

    return;
}

void runNumber_TEST_COMM(uint32_t code)
{
    uint32_t amiBuf;
    DEBUGPRINT("code = %d", code);
    char buf[30];

	bool isOn;

    switch(code)
    {
        case KEYCODE_1 : 

			ituTextSetString(amiTestText, "Testing...");
			isOn = CstGetMcstatus();
			if(!isOn)
				MagneticContactorOn();

			sleep(5);  //Wait turn on...

			if(!bAmiErrChk)
            {   //false means no error.
				ituTextSetString(amiTestText, "OK");
                amiBuf = GetAMIValue();

                if(amiBuf == -1)
                {
                    ituTextSetString(amiWattText, "0.00 kWh");
                }
                else 
                {
                    sprintf(buf, "%d.%d kWh", amiBuf/100, amiBuf % 100);
                    ituTextSetString(amiWattText, buf);
                }
            }
			else
				ituTextSetString(amiTestText, "FAIL");

			if(!isOn)
				MagneticContactorOff();

            break;

        case KEYCODE_2 : 
			ituTextSetString(rfidTestText, "Testing...");
			isOn = PLCPowerStatus();
			if(!isOn)
				PLCPowerOn();
			
			sleep(5);  //Wait turn on...

			if(!RFIDCardReaderCheck())  //false means no error and pass the test.
				ituTextSetString(rfidTestText, "OK");
			else
				ituTextSetString(rfidTestText, "FAIL");

			if(!isOn)
				PLCPowerOff();

			break;
        // case KEYCODE_3 : 
		// 	ituTextSetString(sPingStatustxt, "Testing...");
		// 	#ifndef EN_CERTI
		// 		if(iteEthGetLink())
		// 			ping_init();
		// 		else
		// 			pingtestcheck = false;
		// 	#endif	
		// 	PingTeststart = true;

		// 	sleep(2);

		// 	if(PingTeststart == true && pingtestcheck == true)
		// 	{
		// 		ituTextSetString(pingTestText, "OK");
		// 		PingTeststart = false;
		// 	}
		// 	else if(PingTeststart == true && pingtestcheck == false)
		// 	{
		// 		ituTextSetString(pingTestText, "FAIL");
		// 		PingTeststart = false;
		// 	}
        //     break;
        default : 
            return;
    }
}

void runNumber_TEST_IO(uint32_t code)
{
    DEBUGPRINT("code = %d", code);
    switch(code)
    {
        case KEYCODE_1 : 
			if(LEDStatus())
			{
				LEDOff();
            	ituTextSetString(ledTestText, "OFF");
			}
			else
			{
				LEDOn();
            	ituTextSetString(ledTestText, "ON");
			}
            break;
        case KEYCODE_2 : 
			if(CstGetMcstatus())
			{
				MagneticContactorOff();
            	ituTextSetString(outputMCTestText, "OFF");
			}
			else
			{
				MagneticContactorOn();
            	ituTextSetString(outputMCTestText, "ON");
			}
            break;
        case KEYCODE_3 : 
			if(PLCPowerStatus())
			{
				PLCPowerOff();
            	ituTextSetString(plcPowerTestText, "OFF");
			}
			else
			{
				PLCPowerOn();
            	ituTextSetString(plcPowerTestText, "ON");
			}
            break;
        case KEYCODE_4 : 
			if(!ScreenIsOff()){
				ScreenOff();
				ituTextSetString(backlightTestText, "OFF");
			}
			else{
				ScreenOn();
				ituTextSetString(backlightTestText, "ON");
			}
            break;
        default : 
            return;
    }
}

void keyboardControl(uint32_t flag, uint32_t code)
{
    DEBUGPRINT("Flag : %d Code : %d\n", flag, code);

    UIKeyboardInput.flags = 0;
    UIKeyboardInput.code = 0;

    //if KEYUP event(0x2), do nothing
    if(flag == 0x2) 
        return; 


    switch(code)
    {
        case KEYCODE_TAB : 
            if( (page >= ADMIN_PAGE_NETWORK) && (page <= ADMIN_PAGE_INFO) )
            {   //Main Backgrounds
                ituWidgetSetVisible(nowBackground, false);
                page <<= 1;
                if(page > ADMIN_PAGE_INFO) page = ADMIN_PAGE_NETWORK;
                nowBackground = backgrounds[AdminPageToIntPage(page)];
                ituWidgetSetVisible(nowBackground, true);
                updateUI();
            }
            else if(page == ADMIN_PAGE_FTPUPDATE)
            {   //FTP update background
                //Make old textbox's cursor not visible
                focusedTextBox->textboxFlags &= ~ITU_TEXTBOX_CURSOR;
                ituWidgetSetDirty(focusedTextBox, true);

                //Move textbox
                ++focusedFTPTextBoxIndex;
                if(focusedFTPTextBoxIndex > 2) focusedFTPTextBoxIndex = 0;
                focusedTextBox = ftpTextBoxes[focusedFTPTextBoxIndex];

                //Make new textbox's cursor visible
                focusedTextBox->textboxFlags |= ITU_TEXTBOX_CURSOR;
                ituWidgetSetDirty(focusedTextBox, true);
                if(focusedFTPTextBoxIndex == 0)
                    setKeyboardFilter(KEYBOARD_FILTER_ADDRESS);
                else
                    setKeyboardFilter(KEYBOARD_FILTER_FREE);
            }
            break;

        case KEYCODE_RETURN : 
            if( (page == ADMIN_PAGE_CHANGE_NET) || (page == ADMIN_PAGE_CHANGE_SYS) )
            {   //Change config background
                saveValue();
                ituWidgetSetVisible(nowBackground, false);
                page = page & 0b1111;
                nowBackground = backgrounds[AdminPageToIntPage(page)];
                ituWidgetSetVisible(nowBackground, true);
                updateUI();
            }
            else if(page == ADMIN_PAGE_FTPUPDATE)
            {   //Update ftp background
                updateViaFTP();
            }
            break;

        case KEYCODE_ESCAPE : 
            focusedFTPTextBoxIndex = 0;
            nowConfig = CHANGE_CONFIG_NONE;

            if(page < 0b1111)
            {   //Main Backgrounds
                AdminSetupMenuExit(true);
            }
            else
            {   //Sub Backgrounds
                if(page == ADMIN_PAGE_FTPUPDATE)
                {   //FTP update background
                    //Make cursor not visible
                    focusedTextBox->textboxFlags &= ~ITU_TEXTBOX_CURSOR;
                    ituWidgetSetVisible(focusedTextBox, true);
                    
                    focusedTextBox = NULL;
                }
                if( (page == ADMIN_PAGE_CHANGE_NET) || (page == ADMIN_PAGE_CHANGE_SYS) )
                {   //Change config background
                    focusedTextBox = NULL;
                }
                ituWidgetSetVisible(nowBackground, false);
                page = page & 0b1111;
                nowBackground = backgrounds[AdminPageToIntPage(page)];
                ituWidgetSetVisible(nowBackground, true);
            }
            return;
                

        case KEYCODE_1 : 
        case KEYCODE_2 : 
        case KEYCODE_3 : 
        case KEYCODE_4 : 
        case KEYCODE_5 : 
        case KEYCODE_6 : 
        case KEYCODE_7 : 
        case KEYCODE_8 : 
        case KEYCODE_9 : 
        case KEYCODE_0 : 
            switch(page)
            {   //Run function
                case ADMIN_PAGE_NETWORK : 
                    runNumber_NET(code);
                    break;
                case ADMIN_PAGE_SYSTEM : 
                    runNumber_SYS(code);
                    break;
                case ADMIN_PAGE_TEST : 
                    runNumber_TEST(code);
                    break;
                case ADMIN_PAGE_INFO : 
                    runNumber_INFO(code);
                    break;
                case ADMIN_PAGE_TEST_COMM : 
                    runNumber_TEST_COMM(code);
                    updateUI();
                    break;
                case ADMIN_PAGE_TEST_IO : 
                    runNumber_TEST_IO(code);
                    updateUI();
                    break;

                //Input Textbox
                case ADMIN_PAGE_CHANGE_NET : //fall-through
                case ADMIN_PAGE_CHANGE_SYS : //fall-through
                case ADMIN_PAGE_FTPUPDATE : 
                    DEBUGPRINT("code = %d, getInto InputValue() from Number", code);
                    inputValue(code);
                    break;
            }  
            break;
            //else fall-through
        default: //Any other keys
            if( (page == ADMIN_PAGE_CHANGE_NET) || (page == ADMIN_PAGE_CHANGE_SYS) || (page == ADMIN_PAGE_FTPUPDATE))
            {   //Input Textbox
                DEBUGPRINT("code = %d, getInto InputValue() from Alpha", code);
                inputValue(code);
            }
    }
}




bool AdminKeyboardEnter(ITUWidget* widget, char* param)
{
    char buf[100];
    unsigned int priceBuf;

    bGloAdminStatus = true;

    page = ADMIN_PAGE_NETWORK;
    

    UIKeyboardFunc = keyboardControl;
    hookKeyboard(passKeyboard);

    DEBUGPRINT("Keyboard Hooked");

    backgrounds[0] = ituSceneFindWidget(&theScene, "NetworkBackground");
    assert(backgrounds[0]);
    ituWidgetSetVisible(backgrounds[0], true);
    nowBackground = backgrounds[0];

    backgrounds[1] = ituSceneFindWidget(&theScene, "SystemBackground");
    assert(backgrounds[1]);
    ituWidgetSetVisible(backgrounds[1], false);

    backgrounds[2] = ituSceneFindWidget(&theScene, "TestBackground");
    assert(backgrounds[2]);
    ituWidgetSetVisible(backgrounds[2], false);

    backgrounds[3] = ituSceneFindWidget(&theScene, "InfoBackground");
    assert(backgrounds[3]);
    ituWidgetSetVisible(backgrounds[3], false);

    setValueBackground = ituSceneFindWidget(&theScene, "SetValueBackground");
    assert(setValueBackground);
    ituWidgetSetVisible(setValueBackground, false);

    testCommBackground = ituSceneFindWidget(&theScene, "TestCommBackground");
    assert(testCommBackground);
    ituWidgetSetVisible(testCommBackground, false);

    testIOBackground = ituSceneFindWidget(&theScene, "TestIOBackground");
    assert(testIOBackground);
    ituWidgetSetVisible(testIOBackground, false);

    ftpUpdateBackground = ituSceneFindWidget(&theScene, "FTPUpdateBackground");
    assert(ftpUpdateBackground);
    ituWidgetSetVisible(ftpUpdateBackground, false);

    DEBUGPRINT("Background Set");

    //NetworkBackground
    dhcpText = ituSceneFindWidget(&theScene, "DHCPText");
    assert(dhcpText);

    devIPText = ituSceneFindWidget(&theScene, "DevIPText");
    assert(devIPText);
    ituTextSetString(devIPText, theConfig.ipaddr);

    devGWText = ituSceneFindWidget(&theScene, "DevGWText");
    assert(devGWText);
    ituTextSetString(devGWText, theConfig.gw);

    devSMText = ituSceneFindWidget(&theScene, "DevSMText");
    assert(devSMText);
    ituTextSetString(devSMText, theConfig.netmask);

    if(theConfig.dhcp == true)
    {
        ituTextSetString(dhcpText, "ON");
        ituWidgetSetVisible(devIPText, false);
        ituWidgetSetVisible(devGWText, false);
        ituWidgetSetVisible(devSMText, false);
    }
    else
    {
        ituTextSetString(dhcpText, "OFF");
        ituWidgetSetVisible(devIPText, true);
        ituWidgetSetVisible(devGWText, true);
        ituWidgetSetVisible(devSMText, true);
    }

    devMACText = ituSceneFindWidget(&theScene, "DevMACText");
    assert(devMACText);
    ituTextSetString(devMACText, theConfig.chargermac);

    serverIPText = ituSceneFindWidget(&theScene, "ServerIPText");
    assert(serverIPText);
    ituTextSetString(serverIPText, theConfig.serverip);

    serverPortText = ituSceneFindWidget(&theScene, "ServerPortText");
    assert(serverPortText);
    sprintf(buf, "%d", theConfig.serverport);
    ituTextSetString(serverPortText, buf);

    DEBUGPRINT("Network Background Set");

    //SystemBackground;
    stationIDText = ituSceneFindWidget(&theScene, "StationIDText");
    assert(stationIDText);
    ituTextSetString(stationIDText, theConfig.siteid);

    devIDText = ituSceneFindWidget(&theScene, "DevIDText");
    assert(devIDText);
    ituTextSetString(devIDText, theConfig.devid1);

    DEBUGPRINT("devid = %s", theConfig.devid1);

    authText = ituSceneFindWidget(&theScene, "AuthText");
    assert(authText);
    switch(theConfig.ConfirmSelect)
    {
        case USER_AUTH_CARD : 
            ituTextSetString(authText, "RFID");
            break;
        case USER_AUTH_PASSWORD : 
            ituTextSetString(authText, "비밀번호");
            break;
        case USER_AUTH_NET_QR : 
            ituTextSetString(authText, "QR");
            break;
        case USER_AUTH_NET_REMOTE : 
            ituTextSetString(authText, "원격");
            break;
        case USER_AUTH_NET_BOTH : 
            ituTextSetString(authText, "QR원격");
            break;
        default :
            theConfig.ConfirmSelect = USER_AUTH_NET;
        // case USER_AUTH_NET :
            ituTextSetString(authText, "서버");
            break;
    }

    lcdOnOffTimeText = ituSceneFindWidget(&theScene, "LcdOnOffTimeText");
    assert(lcdOnOffTimeText);
    sprintf(buf, "%d 분", theConfig.screensaver_time);
    ituTextSetString(lcdOnOffTimeText, buf);

    socText = ituSceneFindWidget(&theScene, "SOCText");
    assert(socText);
    sprintf(buf, "%d %%", theConfig.targetSoc);
    ituTextSetString(socText, buf);

    audioVolumnText = ituSceneFindWidget(&theScene, "AudioVolumnText");
    assert(audioVolumnText);
    sprintf(buf, "%d", theConfig.audiolevel);
    ituTextSetString(audioVolumnText, buf);

    modeText = ituSceneFindWidget(&theScene, "ModeText");
    assert(modeText);
    switch(theConfig.OperationMode)
    {
        case OP_CHECK_MODE : 
            ituTextSetString(modeText, "점검");
            break;
        case OP_FREE_MODE : 
            ituTextSetString(modeText, "무료");
            break;
        default : 
            theConfig.OperationMode = OP_NORMAL_MODE;
        // case OP_NORMAL_MODE : 
            ituTextSetString(modeText, "일반");
            break;
    }

    freeChargeTimeText = ituSceneFindWidget(&theScene, "FreeChargeTimeText");
    assert(freeChargeTimeText);
    sprintf(buf, "%d 분", theConfig.FreeChargingTime);
    ituTextSetString(freeChargeTimeText, buf);

    maxPowerText = ituSceneFindWidget(&theScene, "MaxPowerText");
    assert(maxPowerText);
    sprintf(buf, "%d kW", theConfig.maxPower);
    ituTextSetString(maxPowerText, buf);

    forceLimitText = ituSceneFindWidget(&theScene, "ForceLimitText");
    assert(forceLimitText);
    if(theConfig.forcePowerLimit)
        ituTextSetString(forceLimitText, "ON");
    else
        ituTextSetString(forceLimitText, "OFF");

    DEBUGPRINT("System Background Set");

    //InfoBackground
    versionText = ituSceneFindWidget(&theScene, "VersionText");
    assert(versionText);
    ituTextSetString(versionText, SW_VERSION);

    modelText = ituSceneFindWidget(&theScene, "ModelText");
    assert(modelText);
    ituTextSetString(modelText, SECC_MODEL);

    infoPrice1Text = ituSceneFindWidget(&theScene, "InfoPrice1Text");
    assert(infoPrice1Text);
    {
        priceBuf = FourByteOrder(&shmDataIfInfo.unit_cost[0][0][0]);
        sprintf(buf, "%d", priceBuf);
        ituTextSetString(infoPrice1Text, buf);
    }
    infoPrice2Text = ituSceneFindWidget(&theScene, "InfoPrice2Text");
    assert(infoPrice2Text);
    {
        priceBuf = FourByteOrder(&shmDataIfInfo.unit_cost[1][0][0]);
        sprintf(buf, "%d", priceBuf);
        ituTextSetString(infoPrice2Text, buf);
    }
    infoPrice3Text = ituSceneFindWidget(&theScene, "InfoPrice3Text");
    assert(infoPrice3Text);
    {
        priceBuf = FourByteOrder(&shmDataIfInfo.unit_cost[2][0][0]);
        sprintf(buf, "%d", priceBuf);
        ituTextSetString(infoPrice3Text, buf);
    }

    DEBUGPRINT("Info Background Set");

    //setValueBackground
    titleText = ituSceneFindWidget(&theScene, "TitleText");
    assert(titleText);

    infoTextBox = ituSceneFindWidget(&theScene, "InfoTextBox");
    assert(infoTextBox);

    valueTextBox = ituSceneFindWidget(&theScene, "ValueTextBox");
    assert(valueTextBox);

    //testCommBackground
    amiTestText = ituSceneFindWidget(&theScene, "AMITestText");
    assert(amiTestText);

    rfidTestText = ituSceneFindWidget(&theScene, "RFIDTestText");
    assert(rfidTestText);

    amiWattText = ituSceneFindWidget(&theScene, "AMIWattText");
    assert(amiWattText);

    //testIOBackground
    ledTestText = ituSceneFindWidget(&theScene, "LEDTestText");
    assert(ledTestText);
    ituTextSetString(ledTestText, (LEDStatus()) ? "ON" : "OFF");

    outputMCTestText = ituSceneFindWidget(&theScene, "OutputMCTestText");
    assert(outputMCTestText);
    ituTextSetString(outputMCTestText, (CstGetMcstatus()) ? "ON" : "OFF");

    plcPowerTestText = ituSceneFindWidget(&theScene, "PLCPowerTestText");
    assert(plcPowerTestText);
    ituTextSetString(plcPowerTestText, (PLCPowerStatus()) ? "ON" : "OFF");

    backlightTestText = ituSceneFindWidget(&theScene, "BacklightTestText");
    assert(backlightTestText);
    ituTextSetString(backlightTestText, (ScreenIsOff()) ? "OFF" : "ON");

    //ftpUpdateBackground
    ftpTextBoxes[0] = ituSceneFindWidget(&theScene, "FTPAddrTextBox");
    assert(ftpTextBoxes[0]);
    ituTextBoxSetString(ftpTextBoxes[0], theConfig.ftpDns);

    ftpTextBoxes[1] = ituSceneFindWidget(&theScene, "FTPIDTextBox");
    assert(ftpTextBoxes[1]);
    ituTextBoxSetString(ftpTextBoxes[1], theConfig.ftpId);

    ftpTextBoxes[2] = ituSceneFindWidget(&theScene, "FTPPWTextBox");
    assert(ftpTextBoxes[2]);
    ituTextBoxSetString(ftpTextBoxes[2], theConfig.ftpPw);


    tre_regcomp(&regServAddr, REGEX_SERVADDR, REG_EXTENDED);
    tre_regcomp(&regFTPAddr, REGEX_FTPADDR, REG_EXTENDED);
    tre_regcomp(&regIPv4Addr, REGEX_IPV4ADDR, REG_EXTENDED);
    tre_regcomp(&regMACAddr, REGEX_MACADDR, REG_EXTENDED);

    DEBUGPRINT("Enter exit");

    return true;
}

bool AdminKeyboardLeave(ITUWidget* widget, char* param)
{
    return true;
}