/**
*       @file
*               cstmagneticcontactor.c
*       @brief
*   		Copyright (c) Costel All rights reserved. <br>
*               date: 2017.12.05 <br>
*               author: dyhwang <br>
*               description: <br>
*/
#include <string.h>
#include <unistd.h>
#include "ite/audio.h"
#include "audio_mgr.h"
#include "ctrlboard.h"

#include "cstled.h"

//-----------------------------------------------------------------------
// MACRO
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// Local Variable
//-----------------------------------------------------------------------
static bool MCOn = false;

//-----------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------

#if defined(GPIO_MAGNET_CTL1)
void MagneticContactorOn(void)
{
//	printf("[MC#0] Magnetic contactor on\n");
	ithGpioSetMode(GPIO_MAGNET_CTL1, ITH_GPIO_MODE0);
	ithGpioSetOut(GPIO_MAGNET_CTL1);
	ithGpioSet(GPIO_MAGNET_CTL1);
	
	MCOn = true;
}

void MagneticContactorOff(void)
{
//	printf("[MC#0] Magnetic contactor off\n");
	ithGpioSetMode(GPIO_MAGNET_CTL1, ITH_GPIO_MODE0);
	ithGpioSetOut(GPIO_MAGNET_CTL1);
	ithGpioClear(GPIO_MAGNET_CTL1);

	MCOn = false;
}

bool CstGetMcstatus(void)
{	
	return MCOn;
}

void MCInit(void){}

#else

void MagneticContactorOn(void)
{
	MCOn = true;
	// printf("[MC#0] Magnetic contactor on\n");

	//Turn on Neutral line first for safety.

	ithGpioSet(MC_CTRL_NEUTRAL);
	// printf("[MC#0] MC N HIGH on\n");
	usleep(400*1000);

	if(MCOn == false) return;	//Check MC status
	ithGpioClear(MC_CTRL_NEUTRAL);
	ithGpioSet(MC_CTRL_NEUTRAL_R);
	// printf("[MC#0] MC N LOW on\n");
	
	usleep(100*1000);

	if(MCOn == false) return;	//Check MC status
	ithGpioSet(MC_CTRL_LIVE);
	// printf("[MC#0] MC L HIGH on\n");
	usleep(400*1000);

	if(MCOn == false) return;	//Check MC status
	ithGpioClear(MC_CTRL_LIVE);
	ithGpioSet(MC_CTRL_LIVE_R);
	// printf("[MC#0] MC L LOW on\n");
}

void MagneticContactorOff(void)
{
	// printf("[MC#0] Magnetic contactor off\n");
	
	MCOn = false;
	//Works similar to 1007B but 20ms delay for prevent inrush current 24.12.30
	ithGpioClear(MC_CTRL_LIVE_R);
	ithGpioClear(MC_CTRL_LIVE);
	usleep(20*1000);
	ithGpioClear(MC_CTRL_NEUTRAL_R);
	ithGpioClear(MC_CTRL_NEUTRAL);
}

bool CstGetMcstatus(void)
{	
	return MCOn;
}

void MCInit(void)
{
	ithGpioSetMode(MC_CTRL_LIVE, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_LIVE);
	ithGpioClear(MC_CTRL_LIVE);
	ithGpioSetMode(MC_CTRL_LIVE_R, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_LIVE_R);
	ithGpioClear(MC_CTRL_LIVE_R);
	ithGpioSetMode(MC_CTRL_NEUTRAL, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_NEUTRAL);
	ithGpioClear(MC_CTRL_NEUTRAL);
	ithGpioSetMode(MC_CTRL_NEUTRAL_R, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_NEUTRAL_R);
	ithGpioClear(MC_CTRL_NEUTRAL_R);


	MagneticContactorOff();
}

#endif
