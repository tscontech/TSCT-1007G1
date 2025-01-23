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

TouchKeyListener privilegedListener;
uint32_t lastPush;
pthread_t touchKeyTaskID;
TouchKeyListener listenerArray[APP_ORDER_END] = {dummyListener, };


void dummyListener(bool longPush)
{
    printf("[TOUCHKEY] Dummy Listener Run! longPush = %d\n", longPush);
}

void setTouchKeyListener(TouchKeyListener l, APP_ORDER appOrder)
{
    if(listenerArray[appOrder] == NULL)
        listenerArray[appOrder] = l;
}

void setPrivilegedTouchKeyListener(TouchKeyListener l)
{
    privilegedListener = l;
}

//touch key task. run event listener when key up event.
void touchKeyTask(void)
{
    bool now, screenOn;

    while(1)
    {
        now = ithGpioGet(GPIO_TOUCHKEY1);

        if(screenOff)
        {
            ScreenOnScenario();  //Turn on screen
            screenOn = true;
            usleep(100 * 1000);
            continue;
        }
        
        if(now)
        {   //Pushing key
            ++lastPush;
            printf("[TOUCHKEY] Touching! %d\n", lastPush);
        }
        else 
        {   //Release key
            if(lastPush)
            {
                if(screenOn)
                {
                    screenOn = false;
                }
                else if(lastPush >= TOUCHKEY_LONG_PUSH_TIME)
                {//long push
                    printf("[TOUCHKEY] Long Touch! %d\n", lastPush);
                // BuzzerDoubleBeep();
                    if(privilegedListener) privilegedListener(true);
                    else listenerArray[shmDataAppInfo.app_order](true);
                }
                else
                {//short push
                    printf("[TOUCHKEY] Short Touch! %d\n", lastPush);
                // BuzzerBeep();
                    if(privilegedListener) privilegedListener(false);
                    else listenerArray[shmDataAppInfo.app_order](false);
                }

                lastPush = 0;
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