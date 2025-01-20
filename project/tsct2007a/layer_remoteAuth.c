#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ite/itp.h"
#include "ite/itu.h"
#include "SDL/SDL.h"

//#include "qrencode.h"
#include "tsctqrencode/qrenc.h"
#include "scene.h"
#include "ctrlboard.h"
// #include "tsctclient.h"

/* widgets:
remoteAuthLayer
remouteBackground
chkRemoteNumText
remoteNum2TextBox
remoteNum1TextBox
remoteAuthLayer_frame
*/

//-----------------------------------------------------------------------
// MACRO
//-----------------------------------------------------------------------
#define REMOTE_TIMEOUT	90

#define TSCT_OP_DOMAIN         "https://tscs.tscontech.com/api/kakaopay/call_one"
#define TSCT_DEV_DOMAIN      "https://dev-tscs.tscontech.com/api/kakaopay/call_one"

//#define QRCODE_SIZE     8                                                    //Show Output size 

//-----------------------------------------------------------------------
// Local Variable
//-----------------------------------------------------------------------
static ITUBackground* sremouteBackground;

static ITUText* sQRGentxt1;
//static ITUText* sQRGentxt2;

static ITUIcon* sQR_icon;

uint8_t Security_NO = 0;
char Security_NO_buf[3];

char qrcode_buf[256];

static uint16_t WaitQRCnt = 0;

static pthread_t sWaitQRMonitoringTask;
static bool sDLsWaitQRMonitoring = false;

void touchkeyQRPressed(bool longPush)
{
	GotoNextAuthLayer(AUTH_QR_LAYER);

	return;
}

void Tsct_GenQR(void)
{
    ituWidgetSetVisible(sQRGentxt1, false);
    //ituWidgetSetVisible(sQRGentxt2, false);
    ituWidgetSetVisible(sQR_icon, true);
}

static void* sWaitQRMonitoringTaskFuntion(void* arg)
{
    // TSCT Protocol
	// while(sDLsWaitQRMonitoring)
	// {
    //     sleep(1);
    //     if(b1q_RcvFlg){
    //         WaitQRCnt = 0;
    //         b1q_RcvFlg = false;
    //         sDLsWaitQRMonitoring = false;
    //         Tsct_GenQR();
    //     }else {
    //         WaitQRCnt++;
    //         if(WaitQRCnt>5){
    //             //QRGotoSatartLayer();
    //             QR_Cancel_Code = QR_CANCLE_TIMEOUT;
    //             ShowWhmErrorDialogBox(PAY_ERR);
    //         }
    //     }
    // }
    // sWaitQRMonitoringTask = NULL;

    //TODO: OCPP Protocol version
}

void QRGotoSatartLayer(void)
{
    
	// QR_Cancel_Code = QR_CANCLE_TIMEOUT;
	GotoStartLayer();
	//OkDialogSetOkListener(GotoStartLayer);
	// ShowWhmErrorDialogBox(PAY_ERR);  //If q1 sent, Error message must pop up.
}

void QRTimerTimeout(void)
{
    ShowInfoDialogBox(EVENT_TIMEOUT_TIMER);
}

static void QRStopOnDialog(void)
{
    ChannelType activeCh = CstGetUserActiveChannel();

    /*QR_Cancel_Code = QR_CANCLE_BTN;*/

    shmDataAppInfo.app_order = APP_ORDER_WAIT;
	
	TopCloseTimer();
	//UpdateStopGui();
    GotoStartLayer();
}

// Press Cancel Button
static void QRStopOffDialog(void)
{
    //TopResumeTimer();
}

bool remoteOnEnter(ITUWidget* widget, char* param)
{
    
    uint8_t indx;
    ChannelType activeCh = CstGetUserActiveChannel();  
    shmDataAppInfo.app_order = APP_ORDER_QR_WAIT;
    
	CtLogRed("Enter Remote Auth layer..\n");

    if (!sremouteBackground)
    {
        sremouteBackground = ituSceneFindWidget(&theScene, "remouteBackground");
        assert(sremouteBackground);

        sQRGentxt1 = ituSceneFindWidget(&theScene, "QRGentxt1");
		assert(sQRGentxt1);

        //sQRGentxt2 = ituSceneFindWidget(&theScene, "QRGentxt2");
		//assert(sQRGentxt2);

        sQR_icon = ituSceneFindWidget(&theScene, "QR_icon");
		assert(sQR_icon);	
    }

    ituWidgetSetVisible(sQR_icon, false);
    ituWidgetSetVisible(sQRGentxt1, true);
    //ituWidgetSetVisible(sQRGentxt2, true);
    // 1. if Receive Remote Start's Nack or None data display Aram and go to Main.
    // 2. After Server Complete Bil process, Receive Check Security NO.
    // 3. Send a1 and process Charge flow.

    shmDataAppInfo.secure_no[0] = rand() % 10;
    shmDataAppInfo.secure_no[1] = rand() % 10;

    if(theConfig.dns[0] == 'd') 
        sprintf(qrcode_buf, "https://dev-tscs.tscontech.com/api/kakaopay/call_one?cs_id=%s&cp_id=%s&cnctr_id=%02d&no=%d%d", 
                theConfig.siteid, theConfig.devid1, 1, shmDataAppInfo.secure_no[0], shmDataAppInfo.secure_no[1]); //shmDataAppInfo.connector_id = 1
    else
        sprintf(qrcode_buf, "https://tscs.tscontech.com/api/kakaopay/call_one?cs_id=%s&cp_id=%s&cnctr_id=%02d&no=%d%d", 
                theConfig.siteid, theConfig.devid1, 1, shmDataAppInfo.secure_no[0], shmDataAppInfo.secure_no[1]); //shmDataAppInfo.connector_id = 1


    qrencode(qrcode_buf, strlen(qrcode_buf), CFG_PUBLIC_DRIVE ":/media/qren-code-test1.png");

    ituIconLoadPngFileSync(sQR_icon, CFG_PUBLIC_DRIVE ":/media/qren-code-test1.png");

    ituWidgetSetVisible(sQRGentxt1, false);
    ituWidgetSetVisible(sQR_icon, true);

	shmDataAppInfo.charge_request_type = 0x01;	
    
    WaitQRCnt = 0;

    TopHomeBtnVisible(false);
    TopBackBtnVisible(false);

	if (sWaitQRMonitoringTask == NULL)
	{
		sDLsWaitQRMonitoring = true;
		pthread_create(&sWaitQRMonitoringTask, NULL, sWaitQRMonitoringTaskFuntion, NULL);
		pthread_detach(sWaitQRMonitoringTask);
	}	

	usleep(500*1000);


    TopSetTimer(REMOTE_TIMEOUT, QRTimerTimeout);

    setTouchKeyListener(touchkeyQRPressed, APP_ORDER_QR_WAIT);

    return true;
}

bool QRCancelOnPress(ITUWidget* widget, char* param)
{
    // need popup timer    
    //TopPauseTimer();
	OkCancelDialogSetOkListener(QRStopOnDialog, NULL);	
	OkCancelDialogShow_Cal();

	return true;
}

bool remoteOnLeave(ITUWidget* widget, char* param)
{
    // setTouchKeyListener(NULL);
    TopCloseTimer();
    sDLsWaitQRMonitoring = false;
    //sWaitQRMonitoringTask = NULL;
    return true;
}

