#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "lwip/ip.h"
#include "ite/itp.h"
#include "scene.h"
#include "ctrlboard.h"
#include "cststring.h"
#include "tsctcfg.h"
#include "tsctcommon.h"

/* widgets:
mainLayer
mainBackground
mainLayer_icon
MemberCardBtn
*/

static ITUBackground* smainBackground;
static ITUBackgroundButton* sMainStartBtn;

static bool bHomeLayerChk;   // True : Home Layer Display / False : Other Layer Display

static bool bCnctLed = false;
static int enterPressed = 0;

bool GetHomeLayer(void)
{
	return bHomeLayerChk;
}

void SetHomeLayer(bool bset)
{	
	bHomeLayerChk = bset;
}

void touchkeyHomePressed(bool longPush)
{
	// static bool sPLCOn = true;
	// if(CstGetMcstatus())
	// 	MagneticContactorOff();
	// else 
	// 	MagneticContactorOn();

	// if(sPLCOn == false)
	// {
	// 	sPLCOn = true;
	// 	ithGpioSet(GPIO_PLC_PWR_RELAY_CTL);	
	// }
	// else
	// {
	// 	sPLCOn = false;
	// 	ithGpioClear(GPIO_PLC_PWR_RELAY_CTL);	
	// }

	//powerControlPLC(longPush);


	// if(GetServerCon() == false)
	// {
	// 	ShowInfoDialogBox(EVENT_WAIT_SERVER);
	// 	return;
	// }
	// if(GetMeterCon() == false)
	// {
	// 	ShowInfoDialogBox(EVENT_WAIT_AMI);
	// 	return;
	// }
	if(RFIDCardReaderCheck() == true)  // false means no issue (success)
	{
		ShowInfoDialogBox(EVENT_WAIT_RFID);
		return;
	}
	//Remove condition if credit source is added and have a way to check
	#if USE_CREDIT
	if()
	{
		ShowInfoDialogBox(EVENT_WAIT_CREDIT);
		return;
	}
	#endif
	#if USE_SECC
	if(checkSECC() == false)
	{
		ShowInfoDialogBox(EVENT_WAIT_PLC);
		return;
	}
	#endif
	#if USE_OBD
	if()
	{
		ShowInfoDialogBox(EVENT_WAIT_OBD);
		return;
	}
	#endif
	if(BL0939Check() == false)
	{
		ShowInfoDialogBox(EVENT_WAIT_BL);
		return;
	}

	GotoNextAuthLayer(AUTH_WAIT_LAYER);

	return;
}

void keyboardOnMain(uint32_t flag, uint32_t code)
{
	if(flag == ITP_KEYUP) return;
	if(code == KEYCODE_RETURN || code == KEYCODE_KEYPAD_ENTER || code == KEYCODE_RETURN2)
	{
		++enterPressed;
		printf("[keyboardFunction] Keyboard Hooker Run! %d\n", enterPressed);
		if(enterPressed > 2)
		{
			if(!sChCharging)
					ituLayerGoto(ituSceneFindWidget(&theScene, "AdminKeyboardLayer"));	
		}
	}
}

void CPListenerOnAhead(int ch, unsigned char nAdcValue, CPVoltage voltage)
{
	ChannelType activeCh = CstGetUserActiveChannel();

	switch (voltage)
	{
		case CP_VOLTAGE_12V:

			if(bCnctLed == true){
				bCnctLed = false;
				LEDOff();
			}

			cpStatChk = false;

			SetCpStatus(CP_STATUS_CODE_AVAIL, bDevChannel+1);
					
			break;
			
		case CP_VOLTAGE_9V:
		case CP_VOLTAGE_6V:

			if(bCnctLed == false){
				bCnctLed = true;
				LEDOn();
			}

			cpStatChk = true;

			SetCpStatus(CP_STATUS_CODE_PREPARE, bDevChannel+1);
			
			break;

	}
}

/**
 * @brief Initialize Charging Status and Functions
 * 
 * @param ch 
 */
void ChagerInitStatus(int ch)
{
	WattHourMeterStopMonitoring(ch);				
	// ControlPilotSetListener(ch, NULL);
	// ControlPilotDisablePower(ch);		
	gAmiStartWatt = 0.0;
	gAmiChargeWatt = 0.0;
	gChargeWatt[ch] = 0.0;
}

void StopCharge2ch(int ch)
{	
	if(sChCharging == true)
		CstPlayAudioMsg(AUDIO_ENDGHARGE);//14;

	printf("StopCharge2ch :: %d \n", ch);
	shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;
	ScreenOnScenario(); // _dsAn 200228
	
	LEDStopBlink();
	MagneticContactorOff();

	StopPwm(ch);
	WattHourMeterStopMonitoring(ch);	

	sChCharging = false;
}

bool setting2Layer(ITUWidget* widget, char* param)
{
	if(!sChCharging)
		ituLayerGoto(ituSceneFindWidget(&theScene, "AdminLoginLayer"));	
}

bool MainStartOnPress(ITUWidget* widget, char* param)
{
    char *nextLayer = "authUserLayer";

    if(!GetMeterCon())
    {	
        ShowWhmErrorDialogBox(ERR_AMI_DISCON);
        return true;
    }
    // if(!GetServerCon())
    // {
    //     ShowWhmErrorDialogBox(ERR_SERVER_DISCON);
    //     return true;
    // }

	gAmiChargeWatt = 0.0;
	gAmiStartWatt = 0.0;
	gChargeWatt[CH1] = 0.0;		
	
	CstGotoAuthOrNextLayer();	

    ituLayerGoto(ituSceneFindWidget(&theScene, nextLayer));
    return true;
}

bool MainOnEnter(ITUWidget* widget, char* param)
{
	CtLogRed("Enter main layer..\n");

	AudioPlay("A:/sounds/welcom.wav", NULL);	usleep(100*1000);

	TopInitLayer();	

	bCnctLed = false;

	ControlPilotEnablePower(0);
	ControlPilotSetListener(0, CPListenerOnAhead);
	CstSetUserActiveChannel(CH1);
	
	sleep(1);

	#if USE_SECC
	SeccTxData.status_fault = 1<<SECC_STAT_EVSE_READY;
	#endif
	
    if(!smainBackground)
    {
        smainBackground = ituSceneFindWidget(&theScene, "mainBackground");
		sMainStartBtn = ituSceneFindWidget(&theScene, "MainStartBtn");
    }
	if(EmgControl)
	{			
		printf("Ch2OnEnter :: EmgControl %d \n", EmgControl);
		WattHourMeterInit();		
		ControlPilotInit();		
	}	
	// Initialize Status

	StopCharge2ch(CH1);

	LEDOff();
	ChagerInitStatus(CH1);			
	ChagerInitStatus(CH2);
	shmDataAppInfo.app_order = APP_ORDER_WAIT;
	shmDataIfInfo.card_auth = CARD_AUTH_WAIT;

	memset(CsConfigVal.parentId, '\0', sizeof(CsConfigVal.parentId));

	if((GetCpStatus(0) != CP_STATUS_CODE_NONE) && (GetCpStatus(0) != CP_STATUS_CODE_UNAVAIL)){
		for(int i=0;i<=MAX_CONECTOR_ID;i++)
		{
			if(i > 0)
			{
				if(cpStatChk)
				{
					SetCpStatus(CP_STATUS_CODE_PREPARE, i);
				}
				else
				{
					SetCpStatus(CP_STATUS_CODE_AVAIL, i);
				}				
			}
			else 	SetCpStatus(CP_STATUS_CODE_AVAIL, i);
		}
	}

	CsConfigVal.bReqStartTsNo = 0;

  	CtLogRed("shmDataAppInfo.app_order == APP_ORDER_WAIT   ==> %d\n", shmDataAppInfo.app_order );

	SetHomeLayer(true);

	TopHomeBtnVisible(false);

	TopBackBtnVisible(false);
	
    // sTriggerBtn = ituSceneFindWidget(&theScene, "triggerButton");
    // assert(sTriggerBtn);
	
	setTouchKeyListener(touchkeyHomePressed, APP_ORDER_WAIT);
	hookKeyboard(keyboardOnMain);

	// For PWM Test
	// StartPwm(CH1);
	// LEDOn();
	MagneticContactorOff();
    return true;
}

bool MainOnLeave(ITUWidget* widget, char* param)
{
	// setTouchKeyListener(NULL);
	hookKeyboard(NULL);
	SetHomeLayer(false);
	CtLogRed("Exit Main Layer %d", bHomeLayerChk);    

	BuzzerBeep();

    return true;
}