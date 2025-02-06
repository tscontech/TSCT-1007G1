/**
*       @file
*               cstled.c
*       @brief
*   		Copyright (c) Costel All rights reserved. <br>
*               date: 2017.12.05 <br>
*               author: dyhwang <br>
*               description: <br>
*/
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "ite/ith.h"
#include "ctrlboard.h"
#include "tsctcfg.h"

//-----------------------------------------------------------------------
// MACRO
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// Local Variable
//-----------------------------------------------------------------------
static pthread_t sChBlinkTask;
static bool sLedOn = false;
static bool sBlinkRunning = false;

// unsigned int ledPin = GPIO_LED_RAMP1; 
unsigned int redledPin = GPIO_LED_RED; 
unsigned int greenledPin = GPIO_LED_GREEN; 
unsigned int blueledPin = GPIO_LED_BLUE; 
	
//-----------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------

bool LEDStatus(void)
{
	return sLedOn;
}

void LEDOn(void)
{
	ithGpioSet(redledPin);
	ithGpioSet(greenledPin);
	ithGpioSet(blueledPin);
	sLedOn = true;	
}

void LEDOff(void)
{
	ithGpioClear(redledPin);
	ithGpioClear(greenledPin);
	ithGpioClear(blueledPin);
	sLedOn = false;		
}

static void* ChBlinkTask(void* arg)
{
	while(1)
	{
		if(sBlinkRunning)
		{
			if (sLedOn) 	LEDOff();
			else			LEDOn();
		}
		sleep(1);
	}

	CtLogYellow("[LED#0] exit blink thread..\n");
}

void LEDStartBlink(void)
{
	sBlinkRunning = true;
	CtLogYellow("[LED#] Start blink LED ..\n");
}

void LEDStopBlink(void)
{
	sBlinkRunning = false;
	LEDOff();
	CtLogYellow("[LED#] Stop blink LED.\n");
}

void LEDInit(void)
{
	ithGpioSetMode(redledPin, ITH_GPIO_MODE0);
	ithGpioSetOut(redledPin);
	ithGpioClear(redledPin);

	ithGpioSetMode(greenledPin, ITH_GPIO_MODE0);
	ithGpioSetOut(greenledPin);
	ithGpioClear(greenledPin);

	ithGpioSetMode(blueledPin, ITH_GPIO_MODE0);
	ithGpioSetOut(blueledPin);
	ithGpioClear(blueledPin);
	sLedOn = false;	

	// ithGpioSetMode(ledPin[CH2], ITH_GPIO_MODE0);
	// ithGpioSetOut(ledPin[CH2]);
	// ithGpioClear(ledPin[CH2]);
	// sLedOn[CH2] = false;	

	CtLogYellow("[LED] create blink thread..\n");
	pthread_create(&sChBlinkTask, NULL, ChBlinkTask, NULL);
	pthread_detach(sChBlinkTask);
}

