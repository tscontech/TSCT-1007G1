#ifndef __TSCT_TOUCHKEY_H__
#define __TSCT_TOUCHKEY_H__

#define TOUCHKEY_LONG_PUSH_TIME 5  //per 0.1s
#define TOUCHKEY_LISTENER_STACK_SIZE 10

typedef void (*TouchKeyListener)(bool);

void setTouchKeyListener(TouchKeyListener l);

void setPrivilegedTouchKeyListener(TouchKeyListener l);

void touchKeyTask(void);

void touchKeyInit(void);

#endif //__TSCT_TOUCHKEY_H__