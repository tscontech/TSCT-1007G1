#ifndef __TSCT_TOUCHKEY_H__
#define __TSCT_TOUCHKEY_H__

#include "ctrlboard.h"

#define TOUCHKEY_LONG_PUSH_TIME 5  //per 0.1s
#define TOUCHKEY_LISTENER_STACK_SIZE 10

typedef void (*TouchKeyListener)(bool);
typedef struct
{
    bool isTouched;
    bool isLongPush;
}TouchKeyEvent;

void dummyListener(bool longPush);

void setTouchKeyListener(TouchKeyListener l, APP_ORDER appOrder);

void setPrivilegedTouchKeyListener(TouchKeyListener l);

void touchKeyTask(void);

void touchKeyInit(void);

#endif //__TSCT_TOUCHKEY_H__