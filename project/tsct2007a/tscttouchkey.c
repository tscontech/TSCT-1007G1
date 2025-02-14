#include "tscttouchkey.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "ctrlboard.h"
#include "tsctcommon.h"
#include "tsctcfg.h"
#include "ite/ith.h"
#include "ite/itp.h"

void dummyListener(bool longPush);

uint32_t lastPush;
pthread_t touchKeyTaskID;
// TouchKeyListener privilegedListener;
// TouchKeyListener listenerArray[APP_ORDER_END] = {dummyListener, };

extern TouchKeyEvent   UITouchKeyInput;
extern TouchKeyListener UITouchKeyListenerArray[APP_ORDER_END];
extern TouchKeyListener privilegedUITouchKeyListener;


void dummyListener(bool longPush)
{
    printf("[TOUCHKEY] Dummy Listener Run! longPush = %d\n", longPush);
}

void setTouchKeyListener(TouchKeyListener l, APP_ORDER appOrder)
{
    if(UITouchKeyListenerArray[appOrder] == NULL)
        UITouchKeyListenerArray[appOrder] = l;
}

void setPrivilegedTouchKeyListener(TouchKeyListener l)
{
    privilegedUITouchKeyListener = l;
}

//touch key task. run event listener when key up event.
void touchKeyTask(void)
{
    bool now, screenOn = false;

    //Wait for enter main layer. 
    //Triggering touchkey before entered main layer makes system stucked at boot time.
    while(shmDataAppInfo.app_order != APP_ORDER_WAIT) usleep(50*1000);;
    usleep(50*1000);

    while(1)
    {
        now = ithGpioGet(GPIO_TOUCHKEY1);
        
        if(now)
        {   //Pushing key
            if(ScreenIsOff())
            {
                ScreenOnScenario();  //Turn on screen
                screenOn = true;
                usleep(100 * 1000);
                continue;
            }
            ++lastPush;
            printf("[TOUCHKEY] Touching! %d\n", lastPush);
        }
        else 
        {   //Release key
            if(lastPush)
            {
                if(screenOn)
                {   //Skip any action for this touch (Just turn on screen)
                    screenOn = false;
                }
                else if(lastPush >= TOUCHKEY_LONG_PUSH_TIME)
                {//long push
                    printf("[TOUCHKEY] Long Touch! %d\n", lastPush);
                    // BuzzerDoubleBeep();
                    BuzzerBeep();
                    UITouchKeyInput.isLongPush = true;
                }
                else
                {//short push
                    printf("[TOUCHKEY] Short Touch! %d\n", lastPush);
                    BuzzerBeep();
                    UITouchKeyInput.isLongPush = false;
                }

                lastPush = 0;
                UITouchKeyInput.isTouched = true;
            }
        }

        usleep(100 * 1000);
    };
}

void touchKeyInit(void)
{
	#if defined(GPIO_TOUCHKEY1)
    ithGpioSetMode(GPIO_TOUCHKEY1, ITH_GPIO_MODE0);
    ithGpioSetIn(GPIO_TOUCHKEY1);

    // listener = dummyListener;

	if (touchKeyTaskID==0)
	{
		CtLogYellow("[Obd] create touch key thread..\n");
		pthread_create(&touchKeyTaskID, NULL, touchKeyTask, NULL);
		pthread_detach(touchKeyTaskID);
	}		

	#endif
}