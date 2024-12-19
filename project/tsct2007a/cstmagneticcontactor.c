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
//	printf("[MC#0] Magnetic contactor on\n");

	ithGpioSet(MC_CTRL_LIVE);
	ithGpioSet(MC_CTRL_NEUTRAL);

	ithGpioSet(MC_CTRL_LIVE_R);
	ithGpioSet(MC_CTRL_NEUTRAL_R);
		
	MCOn = true;
}

void MagneticContactorOff(void)
{
//	printf("[MC#0] Magnetic contactor off\n");
	ithGpioClear(MC_CTRL_LIVE_R);
	ithGpioClear(MC_CTRL_NEUTRAL_R);

	ithGpioClear(MC_CTRL_LIVE);
	ithGpioClear(MC_CTRL_NEUTRAL);
	MCOn = false;
}

bool CstGetMcstatus(void)
{	
	return MCOn;
}

void MCInit(void)
{
	ithGpioSetMode(MC_CTRL_LIVE, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_LIVE);
	ithGpioSetMode(MC_CTRL_LIVE_R, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_LIVE_R);
	ithGpioSetMode(MC_CTRL_NEUTRAL, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_NEUTRAL);
	ithGpioSetMode(MC_CTRL_NEUTRAL_R, ITH_GPIO_MODE0);
	ithGpioSetOut(MC_CTRL_NEUTRAL_R);

	
	MagneticContactorOff();
}

#endif
