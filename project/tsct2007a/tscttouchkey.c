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
TouchKeyListener listenerStack[TOUCHKEY_LISTENER_STACK_SIZE+1] = {dummyListener, };
uint8_t listenerStackTop = 0;

bool MCStatus;

void dummyListener(bool longPush)
{
    printf("[TOUCHKEY] Dummy Listener Run! longPush = %d\n", longPush);
}

void pushTouchKeyListener(TouchKeyListener l)
{
    if(l != NULL)
    {
        if(listenerStackTop == TOUCHKEY_LISTENER_STACK_SIZE)
        {
            CtLogRed("[TOUCHKEY] pushTouchKeyListener - Listener Stack is Full!!!\n");
            return;
        }
        ++listenerStackTop;
        listenerStack[listenerStackTop] = l;
        printf("[TOUCHKEY] New Listener Pushed! %p\n", l);
    }
}

TouchKeyListener popTouchKeyListener()
{
    TouchKeyListener ret;
    if(listenerStackTop == 0)
    {
        CtLogRed("[TOUCHKEY] popTouchKeyListener - Listener Stack is Empty!!!\n");
        return;
    }
    ret = listenerStack[listenerStackTop];
    listenerStack[listenerStackTop] = NULL;
    --listenerStackTop;
    printf("[TOUCHKEY] Listener Popped! %p\n", ret);

    return ret;
}

void setTouchKeyListener(TouchKeyListener l)
{
    // if(l != NULL) listener = l;
    // else listener = dummyListener;
    // printf("[TOUCHKEY] New Listener Set! %p\n", l);

    if(l != NULL) pushTouchKeyListener(l);
    else popTouchKeyListener();
    // printf("[TOUCHKEY] New Listener Set! %p\n", l);
}

void setPrivilegedTouchKeyListener(TouchKeyListener l)
{
    privilegedListener = l;
}

//touch key task. run event listener when key up event.
void touchKeyTask(void)
{
    bool now;

    while(1)
    {
        now = ithGpioGet(GPIO_TOUCHKEY1);
        
        if(now)
        {
            ++lastPush;  //Pushing key
            printf("[TOUCHKEY] Touching! %d\n", lastPush);
        }
        else if(lastPush)    //Release key
        {
            if(lastPush >= TOUCHKEY_LONG_PUSH_TIME)
            {//long push
                printf("[TOUCHKEY] Long Touch! %d\n", lastPush);
                // BuzzerDoubleBeep();
                // listener(true);
                if(privilegedListener) privilegedListener(true);
                else listenerStack[listenerStackTop](true);
            }
            else
            {//short push
                printf("[TOUCHKEY] Short Touch! %d\n", lastPush);
                // BuzzerBeep();
                // listener(false); 
                if(privilegedListener) privilegedListener(false);
                else listenerStack[listenerStackTop](false);
            }

            ScreenOnScenario();  //Turn on screen
            lastPush = 0;
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