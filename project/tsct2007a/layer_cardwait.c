#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "scene.h"
#include "ctrlboard.h"
#include "Cststring.h"
#include "cstsamwonfa.h"

#include "ite/itp.h"


static ITUBackground* sLcwBackground;

static ITUIcon* scardWaiticon_succ;
static ITUIcon* scardWaiticon_fail;

static ITUIcon* scardWaitTxtIcon_ing;
static ITUIcon* scardWaitTxtIcon_succ;
static ITUIcon* scardWaitTxtIcon_fail;

// static ITUText* scardwordText;
// static ITUText* scardword2Text;
static pthread_t sWaitMonitoringTask;
static bool sDLsWaitMonitoring = false;
static ITUSprite* scardconfirmSprite;

static bool GotoWaittoStartLayer;
static bool GotoWaittoConnectLayer;
static bool scardconfirmSpritebool;

char *CardWaitStrMsg[6][2] = {
	{STR_AUTH_WAIT , STR_WAIT_MSG},    // 0 	
	{STR_AUTH_WAIT , STR_WAIT_MSG},    // 1
	{STR_AUTH_WAIT , STR_WAIT_MSG},    // 2
	{STR_PASS_AUTH_WAIT, STR_WAIT_MSG},  // 3
	{STR_CREDIT_WAIT, STR_WAIT_MSG},	 // 4
	{STR_CAR_AUTH_WAIT, STR_WAIT_MSG}    // 5
};

char *StrCardAuthFailMsg[5] = {
	STR_CARD_AUTH_FAIL_1, 
	STR_CARD_AUTH_FAIL_2, 
	STR_CARD_AUTH_FAIL_3, 
	STR_CARD_AUTH_FAIL_4, 
	STR_CARD_AUTH_FAIL_4
};	

void WaitToStartLayer(void)
{	
	if(GotoWaittoStartLayer)
	{
		GotoWaittoStartLayer = false;
		// GotoStartLayer();
		ShowInfoDialogBox(EVENT_TIMEOUT_TIMER);
	}
}

void WaitToConnectLayer(void)
{	
	if(GotoWaittoConnectLayer)
	{
		GotoWaittoConnectLayer = false;
		ituLayerGoto(ituSceneFindWidget(&theScene, "connectLayer"));	
	}
}

static void StopWaitMonitoringTask(void)
{
	sDLsWaitMonitoring = false;
	TopCloseTimer();		
	if(scardconfirmSpritebool)
	{
		ituSpriteStop(scardconfirmSprite);
		scardconfirmSpritebool = false;
	}
}

static void* sWaitMonitoringTaskFuntion(void* arg)
{
	while(sDLsWaitMonitoring)
	{
		usleep(1000 * 1000);
		ChannelType activeCh = CstGetUserActiveChannel();
		unsigned int var = (int)shmDataAppInfo.auth_type[0];

		if(bConnect)
		{
			if(shmDataIfInfo.card_auth == CARD_AUTH_OK)
			{
				// StopWaitMonitoringTask();
		
				ituWidgetSetVisible(scardWaitTxtIcon_ing, false);
				ituWidgetSetVisible(scardWaiticon_succ, true);
				ituWidgetSetVisible(scardWaitTxtIcon_succ, true);

				// if First Connection, Request Start Transaction
				// if(GetCpStatus(bDevChannel+1) == CP_STATUS_CODE_PREPARE)
				// {	
				// 	if(CsConfigVal.bReqStartTsNo == 0)
				// 		CsConfigVal.bReqStartTsNo = bDevChannel + 1;
				// 	// be Rejected Requesting Start Transaction
				// 	// else if(CsConfigVal.bReqStartTsNo == (2*MAX_CONECTOR_ID + bDevChannel + 1)){	
				// 	// 	printf("#########Receive Error Go to Main [%d]#######\r\n",CsConfigVal.bReqStartTsNo);
				// 	// 	// if(PlayConnectSprite_flag) StopConnectSprite();	
				// 	// 	StopWaitMonitoringTask();
				// 	// 	CsConfigVal.bReqStartTsNo = 0;
				// 	// 	GotoStartLayer();				
				// 	// }
				// 	printf("#########Waitting Ok Receive [%d]#######\r\n",CsConfigVal.bReqStartTsNo);
				// }
				// else if(GetCpStatus(bDevChannel+1) == CP_STATUS_CODE_CHARGING){				// for Accept to Start Transaction
				// 	printf("#########Receive Ok Go to Charge [%d]#######\r\n",CsConfigVal.bReqStartTsNo);
				// 	// if(PlayConnectSprite_flag) StopConnectSprite();	
				// 	StopWaitMonitoringTask();
				// 	CsConfigVal.bReqStartTsNo = 0;
				// 	GotoChargeLayer();
				// }
				// else{
					SetCpStatus(CP_STATUS_CODE_PREPARE, bDevChannel+1);
					StopWaitMonitoringTask();
					sleep(1);
					WaitToConnectLayer();
				// }
			}
			else if(shmDataIfInfo.card_auth == CARD_AUTH_FAILD)
			{
				StopWaitMonitoringTask();
				// ituTextSetString(scardwordText,STR_CARD_AUTH_FAIL); 	
				// ituTextSetString(scardword2Text,STR_CARD_AUTH_FAIL_1); 
				ituWidgetSetVisible(scardWaitTxtIcon_ing, false); 	
				ituWidgetSetVisible(scardWaiticon_fail, true);
				ituWidgetSetVisible(scardWaitTxtIcon_fail, true);

				sleep(1);			
				ShowInfoDialogBox(EVENT_FAILED_AUTH_RFID);
				// WaitToStartLayer();
			}									
		}
		else
		{
			StopWaitMonitoringTask();
			sleep(1);
			WaitToConnectLayer();
		}

	}
	sWaitMonitoringTask = NULL;
}

static void CardAuth1Timer1(void)
{	
	sDLsWaitMonitoring = false;		
	WaitToStartLayer();
}

bool CardWaitEnter(ITUWidget* widget, char* param)
{	
	ChannelType activeCh = CstGetUserActiveChannel();

	TopHomeBtnVisible(false);
	TopBackBtnVisible(false);
	
	GotoWaittoStartLayer = true;
	GotoWaittoConnectLayer = true;
	
	int var = (int)shmDataAppInfo.auth_type[0];

	#if USE_SECC
	SeccTxData.status_fault = 1<<SECC_STAT_EVSE_READY;
	
	SeccInitStep = SECC_INIT_STEP_NONE;
	#endif
	
	CtLogRed("CardWaitEnter.. shmDataAppInfo.app_order = [%d]\n", shmDataAppInfo.app_order);
		
	if( !(shmDataAppInfo.app_order == APP_ORDER_CUSTOMER_CONFIRM && var == PW_INPUT_CARNUM) && 
		!(shmDataAppInfo.app_order == APP_ORDER_CANCEL_PREPAY  && var == PW_INPUT_CARNUM) && 
         (shmDataAppInfo.app_order != APP_ORDER_CAR_NUMAUTH))
	{
		shmDataAppInfo.app_order = APP_ORDER_CUSTOMER_AUTH;
		if(!CfgKeyVal[0].CfgKeyDataInt && CsConfigVal.bReqRmtStartTsNo){
			CsConfigVal.bReqAuthNo = false;	
		}
		else
			CsConfigVal.bReqAuthNo = true;
		CsConfigVal.bReqRmtStartTsNo = 0;
	}

	if(!sLcwBackground)
	{		
		sLcwBackground = ituSceneFindWidget(&theScene, "CardWait_Background");
		assert(sLcwBackground);

		scardconfirmSprite = ituSceneFindWidget(&theScene, "cardconfirmSprite");
        assert(scardconfirmSprite);

		scardWaiticon_succ = ituSceneFindWidget(&theScene, "cardWaiticon_succ");
        assert(scardWaiticon_succ);

		scardWaiticon_fail = ituSceneFindWidget(&theScene, "cardWaiticon_fail");
        assert(scardWaiticon_fail);

		scardWaitTxtIcon_ing = ituSceneFindWidget(&theScene, "cardWaitTxtIcon_ing");
        assert(scardWaitTxtIcon_ing);

		scardWaitTxtIcon_succ = ituSceneFindWidget(&theScene, "cardWaitTxtIcon_succ");
        assert(scardWaitTxtIcon_succ);

		scardWaitTxtIcon_fail = ituSceneFindWidget(&theScene, "cardWaitTxtIcon_fail");
        assert(scardWaitTxtIcon_fail);
		
		// scardwordText = ituSceneFindWidget(&theScene, "cardwordText");
        // assert(scardwordText);
		
		// scardword2Text = ituSceneFindWidget(&theScene, "cardword2Text");
        // assert(scardword2Text);	
				
	}	
	
	// ituWidgetSetVisible(scardwordText, true);	
	// ituWidgetSetVisible(scardword2Text, true);

	ituWidgetSetVisible(scardWaiticon_succ, false);
	ituWidgetSetVisible(scardWaiticon_fail, false);
	ituWidgetSetVisible(scardWaitTxtIcon_ing, true);
	ituWidgetSetVisible(scardWaitTxtIcon_succ, false);
	ituWidgetSetVisible(scardWaitTxtIcon_fail, false);
	
	if(!scardconfirmSpritebool)
	{
		ituSpritePlay(scardconfirmSprite, 0);	
		scardconfirmSpritebool = true;
	}
	
	if(var <= CHECK_CARNUM)
	{
		// ituTextSetString(scardwordText , CardWaitStrMsg[var][0]); 		
		// ituTextSetString(scardword2Text , CardWaitStrMsg[var][1]); 
	}

	sDLsWaitMonitoring = true;
	if (sWaitMonitoringTask == NULL)
	{
		sDLsWaitMonitoring = true;
		pthread_create(&sWaitMonitoringTask, NULL, sWaitMonitoringTaskFuntion, NULL);
		pthread_detach(sWaitMonitoringTask);
	}	

	//TODO: 10초 지났을 때 지연 안내 어떻게 보내지...?
	TopSetTimer(30, CardAuth1Timer1);
	
    return true;
}


bool CardWaitLeave(ITUWidget* widget, char* param)
{		
	CtLogRed("Enter CardWaitLeave layer..\n");

	CsConfigVal.bReqAuthNo = false;

	CsConfigVal.bReqStartTsNo = 0;

	StopWaitMonitoringTask();

	GotoWaittoStartLayer = false;
	GotoWaittoConnectLayer = false;

	TopStopStepAnimation();
	sleep(1);

	// setTouchKeyListener(NULL);
	
    return true;
}

void CardWaitReset(void)
{
	sLcwBackground = NULL;
}
