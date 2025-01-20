/**
*       @file
*               layer_charge.c
*       @brief
*   		Copyright (c) Costel All rights reserved. <br>
*               date: 2018.12.18 <br>
*               author: dyhwang <br>
*				modify: dsAhn <br>
*               description: <br>
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "scene.h"
#include "ctrlboard.h"
#include <time.h>
#include "ite/itp.h"
#include "cststring.h"
#include "tsctcfg.h"

//-----------------------------------------------------------------------
// MACRO
//-----------------------------------------------------------------------
#define UNIT_MS     33
#define LIMIT_OV_VOLT	26400		// +20%
#define LIMIT_UD_VOLT	19800		// -10%
#define LIMIT_OV_CURR	3500		// +10%

#define PRICE_LOG_PERIOD_SEC	15
//-----------------------------------------------------------------------
// Local Variable
//-----------------------------------------------------------------------
static ITUBackground* sLcgBackground;
static ITUBackground* sback_Power;
static ITUBackground* sback_Unit;
static ITUBackground* sback_Curr;
static ITUBackground* sback_Time;

static ITUSprite* sChargeSprite;
//static ITUText* sReadyText;
//static ITUText* seffectiveCurrentText;
static ITUIcon* sreadyTextIcon;
static ITUIcon* schargingTextIcon;
static ITUIcon* swaitTextIcon;
static ITUIcon* sstopTextIcon;
static ITUIcon* sreadyChgGageIcon;
static ITUButton* sStopChargeButton;


static ITUText* sEffectiveCurrentText;
static ITUText* sEnergyUsedText;
static ITUText* sChargeTimeText;
static ITUText* sUnitPricetxt;

//static ITUText* seffectiveCurrenttxt1;
static ITUText* sEffectiveCurrentText1;

static ITUText* schargeTimeText;
//static ITUText* schargeTimeTitleText;


static bool bLayerChargeFlg = false;
static bool sCharging = false;
static bool sChargingInfocheck = true;
static pthread_t sChargeMonitoringTask;
static pthread_t sChargeFaultMonitoringTask;
static bool sDLsChargeMonitoring = false;
static bool sDLsChargeFaultMonitoring = false;

struct timeval stv;
struct timeval etv;   
long sChargeTime;
// long chargeStart;
// long chargeEnd;
static long sChargeStartTime;
static long sChargeEndTime;
int charge_stop_btnState[2];
// static int sCh1CurrentZeroTime = 0;

bool chargecomp_stop; 	// Car Send Charging Stop to Charger

extern bool bAmiErrChk;

//23.10.13
uint32_t gAmiStartWatt;				// Setting AMI Energy once, when Start Charging.
uint32_t gAmiChargeWatt;			// Current Charging Energy.
static bool bChargeStopFlg;     	// Charging Stop Flag (for Preventing Charging Start).

static bool bWaitChargingStopFlg;  	// Car Send Charging Stop to Charger
time_t stime;  				// Stop Request Time

//-----------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------
static void StartCharge(void);

int hour=0, minute=0, second=0;
int charge_price = 0;
int charge_price_ori = 0;
uint32_t charge_watt = 0;
int iUnitprice = 0;
int iUnitprice_ori = 0;
static bool StartTimeCheck;
static int pricepollCount = 0;

static bool sChargeSpritebool;//, true);
static bool sReadyTextbool;//, true);
static bool sStopChargeButtonbool;//, true);
		
//static bool seffectiveCurrenttxt1bool;//, true);
static bool sEffectiveCurrentText1bool;//, true);
		
// static bool stimeIconbool;//, true);
static bool schargeTimeTextbool;//, true);
//static bool schargeTimeTitleTextbool;//, true);

static bool sChargeSpritePlaybool;

static bool bChkCardRxFlg;

char card_no2[21], PId_no2[21];

void touchkeyChargePress(bool longPush)
{
	if(longPush)
	{
		if (sCharging) 
		{		
			charge_stop_btnState[bDevChannel] = 1;
			shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;
			CtLogRed("Charge Stop by Button");
			StopCharge();
			WattHourMeterStopMonitoring(bDevChannel);
			shmDataAppInfo.charge_comp_status = END_BTN;
		}
	
	TopCloseTimer();
	UpdateStopGui();

	}
		// StopCharge();
}

static void GotoNextLayerOnCharge(void)
{
	if(bLayerChargeFlg){
		ituLayerGoto(ituSceneFindWidget(&theScene, "ch2FinishLayer"));
		bLayerChargeFlg = false;
	}
}

/**
 * @brief Get the Charge Price object
 * 
 * @return int Return Charge Price
 */
int GetChargePrice(void)
{
	return charge_price;
}

/**
 * @brief Get the Charge Watt object
 * 
 * @return uint32_t Return Charge Watt
 */
uint32_t GetChargeWatt(void)
{
	return charge_watt;
}

/**
 * @brief Set the Stop Charging Timer object
 * 
 * @param bSet 
 */
void SetStopChargingTimer(bool bSet)
{
	if(bSet)
	{	
		bWaitChargingStopFlg = true;
		stime = evc_get_time();
		printf("[SetStopChargingTimer] Start Time = %d\n", stime);
	}
	else
	{
		bWaitChargingStopFlg = false;
		printf("[SetStopChargingTimer] Start StopCharging End =%d\n", (evc_get_time() - stime));
		stime = 0;
	}
}

/**
 * @brief Check the Stop Charging Timer object
 * 
 */
void CheckStopChargingTimer(void)
{
	if(bWaitChargingStopFlg)
	{
		if((evc_get_time() - stime) > 59) 	// 1min CP Level 9V Charging
		{
			StopCharge();
			ituLayerGoto(ituSceneFindWidget(&theScene, "ch2FinishLayer"));
			shmDataAppInfo.charge_comp_status = END_CAR;	// Charging Stop Req from EV					

			bWaitChargingStopFlg = false;
		}
		else
		{
			printf("[CP 9V] Wait StopCharging = [%d/60]\n", (evc_get_time() - stime));
		}
	}
}

static int lowCurrent_Average=0, lowCurrent_StartTime=0;
static void lowCurrent_Check(int CurrentTime, uint16_t nCurrent)
{
	// nCurrent = 0.00 * 100;
	// float
	// (nCurrent <= 2A) -> 15min STOP
	// if(nCurrent > 200)	return;

	// lowCurrent_Average = (lowCurrent_Average * 99 + nCurrent) / 100;	//떨어지는 속도가 매우 느림
	lowCurrent_Average = (lowCurrent_Average * 9 + nCurrent) / 10;

	// printf("lowCurrent_Check ::::: <CurrentTime : %d>, <nCurrent : %d>, <lowCurrent_Average : %d> \n", CurrentTime, nCurrent, lowCurrent_Average);

	// if((lowCurrent_Average <= 500) && (shmDataAppInfo.app_order == APP_ORDER_CHARGING) && CstGetMcstatus(bDevChannel))	//5A Test
	if((lowCurrent_Average <= 200) && (shmDataAppInfo.app_order == APP_ORDER_CHARGING) && CstGetMcstatus())	//2A, 15min STOP
	{
		if(lowCurrent_StartTime == 0)
		{
			lowCurrent_StartTime = CurrentTime;

			// printf("lowCurrent_Check ::::: <lowCurrent_StartTime : %d> \n", lowCurrent_StartTime);
		}

		int lowCurrent_TotalTime = CurrentTime - lowCurrent_StartTime;

		if(lowCurrent_TotalTime > 900)	//900s / 60s = 15min
		{
			StopCharge();

			ituLayerGoto(ituSceneFindWidget(&theScene, "ch2FinishLayer"));

			shmDataAppInfo.charge_comp_status = END_ERR;
		}
	}
	else
	{
		lowCurrent_StartTime = 0;
	}
}

int charge_time_price(bool credit)
{
	int n_price = DEFAULT_UNITPRICE;
	unsigned short var_int = 0;
	time_t time = CstGetTime();
	struct tm *tm = localtime(&time);
	
	if(!credit)
	{
		if(shmDataIfInfo.connect_status)
		{
			/////////////  단가 계산
			n_price = shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][0] <<24;
			n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][1] <<16;
			n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][2] <<8;
			n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][3];
		}	
		
		/*if(n_price < 1)
			n_price = 24000;*/
		}

	else // creait card logic 
	{
		if(shmDataIfInfo.connect_status)
		{
		n_price = shmDataIfInfo.unit_cost[shmDataAppInfo.member_type+1][tm->tm_hour][0] <<24;
		n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type+1][tm->tm_hour][1] <<16;
		n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type+1][tm->tm_hour][2] <<8;
		n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type+1][tm->tm_hour][3];
		   
		n_price = n_price / 100; // only straffic _dsAn 190411
		}
			
		/*if(n_price < 1)
			n_price = 240;*/
	}

	memcpy(shmDataAppInfo.charge_price , shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour] ,sizeof(shmDataAppInfo.charge_price ));
	
	return n_price;
}

int charge_time_oriprice(void)
{
	int n_price = 100;
	time_t time = CstGetTime();
	struct tm *tm = localtime(&time);
	
	if(shmDataIfInfo.connect_status)
	{
		/////////////  ???? 계산
		n_price = shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][0] <<24;
		n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][1] <<16;
		n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][2] <<8;
		n_price |= shmDataIfInfo.unit_cost[shmDataAppInfo.member_type][tm->tm_hour][3];
	}
		
	/*if(n_price < 1)
		n_price = 100;*/
	return n_price;
}

static void WHMListenerOnCharge(int ch, float current, float volt, uint32_t energy)
{
	char buf[32];	
	
	// if(sChargingInfocheck)
	// {
	// 	if(current < 100.0)
	// 	{
	// 		memset(buf, 0, 32);
	// 		sprintf(buf, "%.1f A", current);
	// 		ituTextSetString(sEffectiveCurrentText1, buf);
	// 	}
	// }

	if (gAmiStartWatt == -1)
	{		
		printf(" sCh1StartEnergy == -1 ch1 energy when start %d0Wh\n", energy);
		gAmiStartWatt = (float)energy;
		gAmiChargeWatt = (float)energy;
	}
	else
	{
		gAmiChargeWatt = (float)energy;
	}

	// CheckCurrentIsZero(CH1, &sCh1CurrentZeroTime, sChargeTime,  current);
}

static void TimeOutChargeStartWait(void)
{	
	unsigned int zero_var = 0;
		
	// GotoNextLayerOnCharge();
	ShowInfoDialogBox(EVENT_TIMEOUT_CHARGE);
}

static void CardReaderListenerOnCharging(char *data, int size)
{
	if ( theConfig.ConfirmSelect == USER_AUTH_NET)
	{	
		printf("[ch]card uid: ");

		for (int i = 0; i < size; i++) 
		{
			printf("%c ", *(data+i));
		}

		printf("card size:[%d]\r\n",size);

		if(size > sizeof(CsConfigVal.scnd_card_no))
			return;

		memset(CsConfigVal.scnd_card_no, '0', size);
		memcpy(CsConfigVal.scnd_card_no , data , size);
		memset(&(CsConfigVal.scnd_card_no[size]), '\0', 1);

		if(!memcmp(shmDataAppInfo.card_no, data, size))
		{
			shmDataAppInfo.charge_comp_status = END_CARD;
			TSCT_ChargingStop();
		}
		else
		{
			if(PId_no2[0] != '\0')
			{
				CsConfigVal.bReqAuthNo = 2;
				printf("[with PID] Unknown Card Number : ");
				for(int j = 0; j<size; j++){
					if(!(j%4)) printf(" ");
					printf("%c", *(data+j));
				}
				printf("\r\n");
			}
			else
			{
				printf("[without PID] Unknown Card Number : ");
				for(int j = 0; j<size; j++){
					if(!(j%4)) printf(" ");
					printf("%c", *(data+j));
				}
				printf("\r\n");
			}
		}	
	}
}

static void* sChargeMonitoringTaskFuntion(void* arg)
{
	int hour=0, minute=0, second=0;		// Charging Time Value
	int pricepollCount = 0;
	uint16_t currbuf;

	char buf[32];
	uint8_t dbgcnt = 0;

	while(sDLsChargeMonitoring)
	{
		sleep(1);

		if(shmDataAppInfo.app_order == APP_ORDER_CHARGING && sCharging)
		{			    
			gettimeofday(&etv, NULL);

			sChargeStartTime = stv.tv_sec;
			sChargeEndTime = etv.tv_sec;	
			sChargeTime = sChargeEndTime - sChargeStartTime;
				
			hour = sChargeTime / 3600;
			minute = (sChargeTime % 3600) / 60;
			second = sChargeTime % 60;
							
			pricepollCount %= 8;
			if (pricepollCount == 0)	
			{	
				#if USE_CREDIT
				iUnitprice = charge_time_price(true);
				#else
				iUnitprice = charge_time_price(false);
				#endif
			}								   
		   charge_watt = gAmiChargeWatt - gAmiStartWatt;

			if(charge_watt < 0)		charge_watt = 0;	
			
			charge_price = charge_watt * iUnitprice / 100; 	//
				
			if((dbgcnt++) > PRICE_LOG_PERIOD_SEC)
			{
				printf("\n\n[ENERGY]======= ch%d energy used: %d -> %d (%d)", bDevChannel, gAmiStartWatt, gAmiChargeWatt, charge_watt);
				printf("\n[PRICE]========== price: %d ===========\n\n", charge_price);
				dbgcnt = 0;
			}

			char temp[6+1] = "";
			sprintf(temp, "%02d%02d%02d", hour, minute, second);
			
			shmDataAppInfo.charge_provide_time[0] = temp[0] << 4;
			shmDataAppInfo.charge_provide_time[0] |= (temp[1] &= 0x0F);
			shmDataAppInfo.charge_provide_time[1] = temp[2] << 4;;
			shmDataAppInfo.charge_provide_time[1] |= (temp[3] &= 0x0F);
			shmDataAppInfo.charge_provide_time[2] = temp[4] << 4;
			shmDataAppInfo.charge_provide_time[2] |= (temp[5] &= 0x0F);

			ValueOrderFourByte(shmDataAppInfo.eqp_watt, gAmiChargeWatt);
			ValueOrderFourByte(shmDataAppInfo.charge_watt, charge_watt);
			ValueOrderFourByte(shmDataAppInfo.charge_money, charge_price);

			CstSetUsedEnergy(charge_watt, sChargeTime);
			
			pricepollCount++;
		}

		CheckStopChargingTimer();

		currbuf = TSCTGetAMICurrent();

		// CheckCurrentIsZero(&sCurrentZeroTime, sChargeTime, currbuf);
		lowCurrent_Check(sChargeTime, currbuf);

		if(theConfig.OperationMode == OP_FREE_MODE \
		&& theConfig.FreeChargingTime != 0	\
		&& sChargeTime/60 >= theConfig.FreeChargingTime){
			StopCharge();
			ituLayerGoto(ituSceneFindWidget(&theScene, "ch2FinishLayer"));
		}

		memset(buf, 0, 32);
		sprintf(buf, "%d.%02d %s", iUnitprice/100, iUnitprice%100, STR_PRICE_WON);
		ituTextSetString(sUnitPricetxt, buf);

		memset(buf, 0, 32);
		sprintf(buf, "%d %s", charge_price/100, STR_PRICE_WON); ///mod (추후 프로토콜 전송 값 확인)
		ituTextSetString(sEffectiveCurrentText, buf);
		//충전??금

		memset(buf, 0, 32);
		sprintf(buf, "%.2f kWh",(float)charge_watt/100.0f ); ///mod
		ituTextSetString(sEnergyUsedText, buf);
		//충전??
		
		memset(buf, 0, 32);
		sprintf(buf, "%02d:%02d", hour, minute);
		ituTextSetString(sChargeTimeText, buf);	
	
		memset(buf, 0, 32);
		sprintf(buf, "%d.%d A", currbuf/100, (currbuf/10)%10);
		ituTextSetString(sEffectiveCurrentText1, buf);

		#if USE_SECC
		// if(	SeccTxData.status_fault & (1<<SECC_STAT_STOP)) {
		if(	SeccRxData.stcode == CSM_STAT_NARMALSTOP || SeccRxData.stcode == CSM_STAT_STOPCHRG) 
		{
			CtLogYellow("Charge Stop by SECC [%lu]", SeccRxData.stcode);
			TSCT_ChargingStop();
		}
		else if(SeccRxData.stcode == CSM_STAT_FAILURESTOP)
		{
			CtLogRed("Charge Stop by SECC/EV Failure");
			TSCT_ChargingStop();
		}
		else if(SeccTxData.status_fault & (1<<SECC_STAT_STOP))
		{
			CtLogYellow("Charge Stop by Charger Request [2]");
			TSCT_ChargingStop();
		}
		#endif
			
	}
	sChargeMonitoringTask = 0;
}
void TSCT_ChargingStop()
{
	sCharging = false;	
	sChCharging = false;
	sDLsChargeMonitoring = false;		
	sDLsChargeFaultMonitoring = false;
	MagneticContactorOff();
	printf("[TSCT_ChargingStop] Charging Stop %d\r\n",shmDataAppInfo.charge_comp_status);
	ituSpriteStop(sChargeSprite);
	//CstSetUserActiveChannel(CH1);
	// ControlPilotSetListener(bDevChannel, NULL);
	LEDStopBlink();	
	usleep(300*1000);
	StopPwm(CH1);
	WattHourMeterStopMonitoring(CH1);
	shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;				
	GotoNextLayerOnCharge();
}

/*
static bool CheckChrgPwr(uint16_t chrgCurr, uint16_t chrgVolt, uint8_t currLmtFlg)
{
	uint32_t currChrgPwrW = 0;

	currChrgPwrW = (uint32_t)(chrgCurr * chrgVolt) / 1000;

	if(currLmtFlg)
		return false;
	if(currChrgPwrW > (uint32_t)(theConfig.chargingstatus * 1000))
		return true;
	else
		return false;		
}
*/

static void ChargeFaultMonitoringTaskFuntion(void *arg)
{
	uint8_t Fault_Count[6] = {0,};						// [0]:Over Current, [1]:Over Voltage, [2]:Under Voltage, [3]:Over Temperture, [4]: Over Power Limit(Cut down), [5]: Over Power Limit(Stop Charging)
	uint16_t limit_over_current=0, chk_Current=0, chk_Volt=0, LimitVolt_Average=0;
	uint32_t limit_over_power=0, chk_Power=0;
	bool Power_Fault_Packet=false;

	// uint8_t currLmtFlg=false;	//x?
	// uint8_t currLmtAmp = 30;	//x?


	// uint_fast8_t checkerTick = 0;

	limit_over_current = (uint16_t)theConfig.maxPower * 500;
	limit_over_power = (uint32_t)theConfig.maxPower * 1000;

	while(sDLsChargeFaultMonitoring)
	{
		sleep(1);
		// Sever Connection Check

		if(CstGetMcstatus() == true)  //6V + Charging
			while(bAmiErrChk && !CstGetEpqStatus(TSCT_COM_WHM))
			{	//AMI not working
				CstSetEpqStatus(TSCT_COM_WHM, false);
				// EventCode_buf |= 1 << EVE_AMI;
				usleep(100*1000);				
			}
		else CstSetEpqStatus(TSCT_COM_WHM, true);

		chk_Volt = TSCTGetAMIVolt();
		LimitVolt_Average = (LimitVolt_Average * 4 + chk_Volt) / 5;
		chk_Current = TSCTGetAMICurrent();
		chk_Power = ((uint32_t)LimitVolt_Average * (uint32_t)chk_Current) / 1000;

		if(chk_Current > limit_over_current)	Fault_Count[0]++;	
		else	Fault_Count[0] = 0;
		if(chk_Volt > LIMIT_OV_VOLT)	Fault_Count[1]++;	
		else	Fault_Count[1] = 0;
		if(chk_Volt < LIMIT_UD_VOLT)	Fault_Count[2]++;	
		else	Fault_Count[2] = 0;
		if(ChargerTemperate > 80)		Fault_Count[3]++;
		else	Fault_Count[3] = 0;

		if(chk_Power > limit_over_power) Fault_Count[4]++;
		else Fault_Count[4] = 0;

		// if(CheckChrgPwr(chk_Current, chk_Volt, currLmtFlg)) Fault_Count[4]++;
		// else	Fault_Count[4] = 0;
		// if(currLmtFlg && (chk_Current > (uint16_t)((currLmtAmp+1) * 100))) Fault_Count[5]++;		// 설정 전류의 3% 초과해서 전류 가져가는 차량 존재
		// else	Fault_Count[5] = 0;		
		
		if(Fault_Count[0] > 10){ 
			CtLogRed("Charge Over Current Fault!!!!!!!!!!!!!!!!!!!");
			Fault_Count[0] = 0;
			Fault_Count[1] = 0;
			Fault_Count[2] = 0;
			shmDataAppInfo.charge_comp_status = END_ERR;
			CstSetEpqStatus(TSCT_OUT_OVER_CURRENT, false);
			ShowWhmErrorDialogBox(ERR_OV_CURT);
			// EventCode_buf |= 1 << EVE_OVC;
		}
		if(Fault_Count[1] > 10){ 
			CtLogRed("Charge Over Voltage Fault!!!!!!!!!!!!!!!!!!!");		
			Fault_Count[0] = 0;
			Fault_Count[1] = 0;
			Fault_Count[2] = 0;
			shmDataAppInfo.charge_comp_status = END_ERR;
			CstSetEpqStatus(TSCT_OUT_OVER_VOLTAGE, false);
			ShowWhmErrorDialogBox(ERR_OV_VOLT);
			// EventCode_buf |= 1 << EVE_OVV;
		}
		if(Fault_Count[2] > 10){ 
			CtLogRed("Charge Under Voltage Fault!!!!!!!!!!!!!!!!!!!");
			Fault_Count[0] = 0;
			Fault_Count[1] = 0;
			Fault_Count[2] = 0;
			shmDataAppInfo.charge_comp_status = END_ERR;
			CstSetEpqStatus(TSCT_IN_UNDER_VOLTAGE, false);
			ShowWhmErrorDialogBox(ERR_UD_VOLT);
			// EventCode_buf |= 1 << EVE_UDV;
		}
		if(Fault_Count[3] > 5){
			CtLogRed("Over Temperatrue Error!!!!!!!!!!!!!!!!!!!");		
			sDLsChargeFaultMonitoring = 0;
			Fault_Count[0] = 0;
			Fault_Count[1] = 0;
			Fault_Count[2] = 0;
			Fault_Count[3] = 0;
			shmDataAppInfo.charge_comp_status = END_ERR;
			CstSetEpqStatus(TSCT_OVER_TEMP, false);
			ShowWhmErrorDialogBox(ERR_TEMP);
			// EventCode_buf |= 1 << EVE_TEMP;			
		}

		if(Fault_Count[4] > 5)	//변화되는 시간만큼
		{
			// CtLogRed("Over Setting Power!! Current %u A / Volt %u V !!!!!!!!", chk_Current, chk_Volt);

			// currLmtFlg = true;

			if(theConfig.forcePowerLimit) // v1.4.1 - 강제전력제한 기능 ON/OFF  24.10.25 JGLEE
			{
				// test here
				// SetPwm(29);
				Powerlimit_current(bDevChannel, LimitVolt_Average);
				Power_Fault_Packet = false;
			}

			if(!Power_Fault_Packet)
			{
				Power_Fault_Packet = true;

				CstSetEpqStatus(TSCT_OVER_POWER, false);
				// EventCode_buf |= 1 << EVE_OVP;
			}

			Fault_Count[4] = 0;
		}

		//CtLogYellow("ChargeFault Cnt : %d / Current : %d, Volt : %d ", Fault_Count, chk_Current, chk_Volt);
	}
	sChargeFaultMonitoringTask = 0;
}

static void StartCharge(void)
{
	ChannelType activeCh = CstGetUserActiveChannel();
	// if(sReadyTextbool)
	// {
	// 	sReadyTextbool = false;
	// 	//ituWidgetSetVisible(sReadyText, false);
	// 	ituWidgetSetVisible(sreadyTextIcon, false);
	// 	ituWidgetSetVisible(sreadyChgGageIcon, false);
	// 	ituWidgetSetVisible(schargingTextIcon, true);
	// 	ituWidgetSetVisible(sEffectiveCurrentText, true);				
	// }	
	// ituSpritePlay(sChargeSprite, CH1);	
	// sChargeSpritePlaybool = true;
	
	// LEDStartBlink();
	MagneticContactorOn();
	usleep(3000*1000);
	sCharging = true;	
	
	if((theConfig.devtype == BC_TYPE)|| (theConfig.devtype == HBC_TYPE)||
		(theConfig.devtype == C_TYPE) || (theConfig.devtype == HC_TYPE) )
		TopCloseTimer();
}

void StopCharge(void)
{
	printf("Stop Charge~~~~\r\n");
	sCharging = false;	
	MagneticContactorOff();
	
	ScreenOnScenario(); // _dsAn 200228
	ituSpriteStop(sChargeSprite);
	sChargeSpritePlaybool = false;
	if(sReadyTextbool) 	{
		sReadyTextbool = false;
		//ituWidgetSetVisible(sReadyText, false);		
		ituWidgetSetVisible(sreadyTextIcon, false);
		ituWidgetSetVisible(sreadyChgGageIcon, false);
		ituWidgetSetVisible(swaitTextIcon, false);
		//ituWidgetSetVisible(schargingTextIcon, true);		
	}	
	shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;	
	ituSpriteGoto(sChargeSprite, 0);
	LEDStopBlink();
	TopHomeBtnVisible(false);
	usleep(300*1000);
	StopPwm(bDevChannel);	
	usleep(300*1000);
}

static void UpdateStartGui()
{	
	if(!sChargeSpritebool)
	{
		sChargeSpritebool = true;
		ituWidgetSetVisible(sChargeSprite, true);			
	}	
			
	if(!sStopChargeButtonbool)
	{
		sStopChargeButtonbool = true;
		// ituWidgetSetVisible(sStopChargeButton, true);			
		ituWidgetSetVisible(sstopTextIcon, true);
	}		

}

void UpdateStopGui()
{
	ituSpriteStop(sChargeSprite);
	sChargeSpritePlaybool = false;
	if(sChargeSpritebool)
	{
		sChargeSpritebool = false;
		ituWidgetSetVisible(sChargeSprite, false);			
	}	
	if(sStopChargeButtonbool)
	{
		sStopChargeButtonbool = false;
		// ituWidgetSetVisible(sStopChargeButton, false);			
		ituWidgetSetVisible(sstopTextIcon, false);
	}		

	//ituTextSetString(sEffectiveCurrentText1, "0.0 A");

	GotoNextLayerOnCharge();
}

static void CPListenerOnCharge(int ch, unsigned char nAdcValue, CPVoltage voltage)
{
	if(EmgControl) return;
	printf("[CPListenerOnCharge] Voltage = %d\n", voltage);
	switch(voltage)
	{
		case CP_VOLTAGE_UNKNOWN:
			chargecomp_stop = false;
		    break;

		case CP_VOLTAGE_12V:
			// if(CsConfigVal.StopTransactionOnEVSideDisconnect){
			if(CfgKeyVal[13].CfgKeyDataInt){
				ScreenOnScenario();
				chargecomp_stop = false;			
				shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;
				shmDataAppInfo.charge_comp_status = END_UNPLUG;
				// if(sCharging)	{
					CtLogRed("Charge Stop by Disconnect\n");
					StopCharge();
					UpdateStopGui(); 
					sleep(1); ///mod
				// }
				GotoNextLayerOnCharge();
			}
			else{
				// if (sCharging){ 	
					if(CstGetMcstatus()){	
						MagneticContactorOff();	
						StartStopCharging(1, ch);
						chargecomp_stop = true;
						SetCpStatus(CP_STATUS_CODE_SUSPENDEDEV,bDevChannel+1);
					}
				// }
			}
			break;
			
		case CP_VOLTAGE_9V:
		#if USE_SECC
			if(SeccRxData.stcode == CSM_STAT_PAUSESESSION)
			{  //Pause charging
				if (sCharging)
				{ 	
					SetStopChargingTimer(true);
					shmDataAppInfo.app_order = APP_ORDER_CHARGE_READY;
					if(CstGetMcstatus())
					{	
						MagneticContactorOff();	
						StartStopCharging(1, ch);
						chargecomp_stop = true;
					}
				}
			}
			//Stop charging
			else if( SeccRxData.stcode == CSM_STAT_FAILURESTOP )
			{   //TODO: Need to open MC in 100ms
				chargecomp_stop = false;			
				shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;
				shmDataAppInfo.charge_comp_status = END_ERR;
				// if(sCharging)	{
					CtLogRed("Charge Stop by Failure\n");
					StopCharge();
					UpdateStopGui(); 
					sleep(1); ///mod
				// }
				GotoNextLayerOnCharge();
			}
			else if(SeccRxData.stcode == CSM_STAT_NARMALSTOP || SeccRxData.stcode == CSM_STAT_STOPCHRG)
			{   // Stop by car?
				chargecomp_stop = false;			
				shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;
				shmDataAppInfo.charge_comp_status = END_CAR;
				// if(sCharging)	{
					CtLogYellow("Charge Stop by EV Request\n");
					StopCharge();
					UpdateStopGui(); 
					sleep(1); ///mod
				// }
				GotoNextLayerOnCharge();
			}
		#else
				if (sCharging)
				{ 	//Pause charging
					SetStopChargingTimer(true);
					shmDataAppInfo.app_order = APP_ORDER_CHARGE_READY;
					if(CstGetMcstatus())
					{	
						MagneticContactorOff();	
						StartStopCharging(1, ch);
						chargecomp_stop = true;
					}
				}
		#endif
			break;

		case CP_VOLTAGE_6V:
			if(!EmgControl && !CsConfigVal.bReqRmtStopTSFlg)
			{
				if (!sCharging && (charge_stop_btnState[bDevChannel] == 0)) 
				{
					// if(sChCharging == false)
					AudioPlay("A:/sounds/startCharge.wav", NULL);
						
					shmDataAppInfo.app_order = APP_ORDER_CHARGING;
					if(StartTimeCheck)
					{
						StartTimeCheck = false;
						gettimeofday(&stv, NULL);
					}		
					StartCharge();
					UpdateStartGui();
				}
				else
				{	
					if(!CstGetMcstatus() && (charge_stop_btnState[bDevChannel] == 0))	
					{
						chargecomp_stop = false;
						MagneticContactorOn();
						StartStopCharging(0, ch);
						SetStopChargingTimer(false);
					}
				}
			}
			break;			
	}
}

static void ChargeStopOnDialog(void)
{
	if (sCharging) 
	{		
		charge_stop_btnState[bDevChannel] = 1;
	    shmDataAppInfo.app_order = APP_ORDER_CHARGING_STOP;
		CtLogRed("Charge Stop by Button");
		StopCharge();
		WattHourMeterStopMonitoring(bDevChannel);
		shmDataAppInfo.charge_comp_status = END_BTN;
	}
	
	TopCloseTimer();
	UpdateStopGui();
}

bool InfoAddOnPress(ITUWidget* widget, char* param)
{	
	// ituWidgetSetVisible(seffectiveCurrenticon1, true);
	//ituWidgetSetVisible(seffectiveCurrenttxt1, true);
	ituWidgetSetVisible(sEffectiveCurrentText1, true);

	// ituWidgetSetVisible(stimeIcon, true);
	ituWidgetSetVisible(schargeTimeText, true);
	//ituWidgetSetVisible(schargeTimeTitleText, true);
	return true;
}

bool ChargeStopOnPress(ITUWidget* widget, char* param)
{	
	if(sReadyTextbool)
	{
		sReadyTextbool = false;
		//ituWidgetSetVisible(sReadyText, false);		
		ituWidgetSetVisible(sreadyTextIcon, false);
		ituWidgetSetVisible(sreadyChgGageIcon, false);
		//ituWidgetSetVisible(schargingTextIcon, true);		
	}	
	OkCancelDialogSetOkListener(ChargeStopOnDialog, NULL);	
	OkCancelDialogShow();
	
	return true;
}

bool ChargeOnEnter(ITUWidget* widget, char* param)
{
	char buf[32];
	stime = 0;
	ChannelType activeCh = CstGetUserActiveChannel();	
	CtLogRed("	[%d]  Enter charge layer..==========\n", bDevChannel);

	#if USE_SECC
	if(theConfig.OperationMode != OP_CHECK_MODE)
	{
		SeccTxData.status_fault &= ~(1<<SECC_STAT_STOP);
		SeccTxData.status_fault |= 1<<SECC_STAT_CHARG;
	}
	#endif

	bLayerChargeFlg = true;

	sChCharging = true;

	StartTimeCheck = true;
	hour=0; minute=0; second=0;
	sCharging = false;
	sChargeTime = 0;
	// sCh1CurrentZeroTime = 0;
	gAmiChargeWatt = -1;
	gAmiStartWatt = -1;
	charge_stop_btnState[bDevChannel] = 0;
	CstSetUsedEnergy(0.0, 0);

	theConfig.chargingstatus |= (1<<(MAX_CONECTOR_ID+1));

	StopTsConfig.Connector_No = bDevChannel;

	StopTsConfig.TrId = CsConfigVal.bTrId[bDevChannel+1];

	GetDateTime(StopTsConfig.Time_Stamp);

	StopTsConfig.MeterStop_Val = 0;

	for(int i =0;i<4;i++)
		StopTsConfig.MeterStop_Val += (uint32_t)(shmDataAppInfo.eqp_watt[i] << ((3-i)*8));

	memcpy(StopTsConfig.IdTag, shmDataAppInfo.card_no, sizeof(shmDataAppInfo.card_no));
	memset(&(StopTsConfig.IdTag[16]), '\0', 1);

	StopTsConfig.Stop_Reason = STOP_REASON_POWER;


	memset(shmDataAppInfo.charge_provide_time, 0x00, sizeof(shmDataAppInfo.charge_provide_time));
	memset(shmDataAppInfo.charge_watt, 0x00, sizeof(shmDataAppInfo.charge_watt));
	memset(shmDataAppInfo.charge_money, 0x00, sizeof(shmDataAppInfo.charge_money));
	
	shmDataAppInfo.app_order = APP_ORDER_CHARGE_READY;

	if (!sLcgBackground)
	{
		sLcgBackground = ituSceneFindWidget(&theScene, "chargeBackground");
		assert(sLcgBackground);

		sChargeSprite = ituSceneFindWidget(&theScene, "chargeSprite");
		assert(sChargeSprite);

		sback_Power = ituSceneFindWidget(&theScene, "back_Power");
		assert(sback_Power);
		sback_Unit = ituSceneFindWidget(&theScene, "back_Unit");
		assert(sback_Unit);
		sback_Curr = ituSceneFindWidget(&theScene, "back_Curr");
		assert(sback_Curr);
		sback_Time = ituSceneFindWidget(&theScene, "back_Time");
		assert(sback_Time);

		schargingTextIcon = ituSceneFindWidget(&theScene, "chargingTextIcon");
		sreadyTextIcon = ituSceneFindWidget(&theScene, "readyTextIcon");
		sreadyChgGageIcon = ituSceneFindWidget(&theScene, "readyChgGageIcon");
		swaitTextIcon = ituSceneFindWidget(&theScene, "waitTextIcon");
		sstopTextIcon = ituSceneFindWidget(&theScene, "stopTextIcon");

		sEffectiveCurrentText = ituSceneFindWidget(&theScene, "effectiveCurrentText");
		assert(sEffectiveCurrentText);					
	
		sUnitPricetxt = ituSceneFindWidget(&theScene, "UnitPricetxt");
		assert(sUnitPricetxt);		
		sEnergyUsedText = ituSceneFindWidget(&theScene, "energyUsedText");
		assert(sEnergyUsedText);
		sChargeTimeText = ituSceneFindWidget(&theScene, "chargeTimeText");
		assert(sChargeTimeText);
		sStopChargeButton = ituSceneFindWidget(&theScene, "stopChargeButton");
		assert(sStopChargeButton);		
		
		sEffectiveCurrentText1 = ituSceneFindWidget(&theScene, "effectiveCurrentText1");
		assert(sEffectiveCurrentText1);
						
	}
	iUnitprice = charge_time_price(0);
		
	memset(buf, 0, 32);
	sprintf(buf, "%d.%02d %s", iUnitprice/100, iUnitprice%100, STR_PRICE_WON);
	ituTextSetString(sUnitPricetxt, buf);
	charge_price = 0;
	
	memset(buf, 0, 32);
	sprintf(buf, "%d %s", charge_price/100, STR_PRICE_WON);
	ituTextSetString(sEffectiveCurrentText, buf);

	ituTextSetString(sEffectiveCurrentText1, "0.0 A");
	
	//충전??금
	charge_watt = 0;
	memset(buf, 0, 32);
	sprintf(buf, "%.2f kWh",(float)charge_watt/100.0f); ///mod
	ituTextSetString(sEnergyUsedText, buf);
	ituTextSetString(sChargeTimeText, "00:00");	
	
	ControlPilotSetListener(bDevChannel, CPListenerOnCharge);

	StartPwm(bDevChannel);
	WattHourMeterStartMonitoring(activeCh, WHMListenerOnCharge);

	// memcpy(card_no2, shmDataAppInfo.card_no, sizeof(shmDataAppInfo.card_no));
	// memset(&(card_no2[16]), '\0', 1);
	memcpy(PId_no2, CsConfigVal.parentId, sizeof(CsConfigVal.parentId));
	memset(CsConfigVal.scnd_card_no,'\0', sizeof(CsConfigVal.scnd_card_no));
	memset(CsConfigVal.parentId, '\0', sizeof(CsConfigVal.parentId));

	AudioPlay("A:/sounds/startCharge.wav", NULL);
	
	bChkCardRxFlg = false;
	CardReaderStartMonitoring(CardReaderListenerOnCharging);
	
	if(!sChargeSpritebool)
	{
		sChargeSpritebool = true;
		ituWidgetSetVisible(sChargeSprite, true);			
	}	
		
	if(!sStopChargeButtonbool)
	{
		sStopChargeButtonbool = true;
		// ituWidgetSetVisible(sStopChargeButton, true);			
		ituWidgetSetVisible(sstopTextIcon, true);
	}		
	// if(!sReadyTextbool)
	// {
	// 	sReadyTextbool = true;
	// 	//ituWidgetSetVisible(sReadyText, true);
	// 	ituWidgetSetVisible(sreadyChgGageIcon, true);
	// 	ituWidgetSetVisible(sreadyTextIcon, true);
	// 	ituWidgetSetVisible(schargingTextIcon, false);
	// 	ituWidgetSetVisible(sEffectiveCurrentText, false);
	// }

	// if(sReadyTextbool)
	// {
		// sReadyTextbool = false;
		//ituWidgetSetVisible(sReadyText, false);
		ituWidgetSetVisible(sreadyTextIcon, false);
		ituWidgetSetVisible(swaitTextIcon, false);
		ituWidgetSetVisible(sreadyChgGageIcon, false);
		ituWidgetSetVisible(schargingTextIcon, true);
		ituWidgetSetVisible(sstopTextIcon, true);
		ituWidgetSetVisible(sEffectiveCurrentText, true);				
	// }	
	ituSpritePlay(sChargeSprite, CH1);	
	sChargeSpritePlaybool = true;
	
	LEDStartBlink();

	TopHomeBtnVisible(false);

	sChargingInfocheck = true;
	// ituWidgetSetVisible(seffectiveCurrenticon1, true);
	//ituWidgetSetVisible(seffectiveCurrenttxt1, true);
	//ituWidgetSetVisible(sEffectiveCurrentText1, true);

	// ituWidgetSetVisible(stimeIcon, true);
	//ituWidgetSetVisible(schargeTimeText, true);
	//ituWidgetSetVisible(schargeTimeTitleText, true);
	
	if (sChargeMonitoringTask == 0)
	{
		sDLsChargeMonitoring = true;
		pthread_create(&sChargeMonitoringTask, NULL, sChargeMonitoringTaskFuntion, NULL);
		pthread_detach(sChargeMonitoringTask);
	}

	if (sChargeFaultMonitoringTask == 0)
	{
		sDLsChargeFaultMonitoring = true;
		pthread_create(&sChargeFaultMonitoringTask, NULL, ChargeFaultMonitoringTaskFuntion, NULL);
		pthread_detach(sChargeFaultMonitoringTask);
	}
	
	if(EmgControl && shmDataAppInfo.app_order <= APP_ORDER_CHARGING)
	{
		UpdateStopGui();

	}

	ConfigSave();

	setTouchKeyListener(touchkeyChargePress, APP_ORDER_CHARGING);

    return true;
}

bool ChargeOnLeave(ITUWidget* widget, char* param)
{	
	SetStopChargingTimer(false);
	StopCharge();
	
	char buf[8]={0x30,};

	#if USE_SECC
	SeccTxData.status_fault &= ~(1<<SECC_STAT_CHARG);
	SeccTxData.status_fault |= 1<<SECC_STAT_STOP;
	#endif

	sprintf(buf, "0%s", STR_PRICE_WON);
	ChannelType activeCh = CstGetUserActiveChannel();
	
	sChCharging = false;

	StopTsConfig.Connector_No = bDevChannel;

	StopTsConfig.TrId = CsConfigVal.bTrId[bDevChannel+1];

	GetDateTime(StopTsConfig.Time_Stamp);

	StopTsConfig.MeterStop_Val = 0;

	for(int i =0;i<4;i++)
		StopTsConfig.MeterStop_Val += (uint32_t)(shmDataAppInfo.eqp_watt[i] << ((3-i)*8));

	memcpy(StopTsConfig.IdTag, shmDataAppInfo.card_no, sizeof(shmDataAppInfo.card_no));
	memset(&(StopTsConfig.IdTag[16]), '\0', 1);

	/*
	ituTextSetString(sEffectiveCurrentText1, "0.0 A");
	ituTextSetString(sUnitPricetxt, buf);
	ituTextSetString(sEffectiveCurrentText, buf);
	ituTextSetString(sEnergyUsedText, "0.00 kWh");
	ituTextSetString(sChargeTimeText, "00:00");
	*/

	// ControlPilotSetListener(bDevChannel, NULL);

	chargecomp_stop = false;
	sDLsChargeMonitoring = false;	
	sDLsChargeFaultMonitoring = false;

	if(!sleepOn1chCheck)
	{		
		sCharging = false;
		TopStopStepAnimation();	 
	
		usleep(100*1000);
		printf("ChargeOnLeave :: sleepOn1chCheck %d \n", sleepOn1chCheck);
		
		WattHourMeterStopMonitoring(activeCh);
		if(sChargeSpritePlaybool)
		{
			sChargeSpritePlaybool = false;
			ituSpriteStop(sChargeSprite);
			ituSpriteGoto(sChargeSprite, 0);
		}
		
		if(sChargeSpritebool)
		{
			sChargeSpritebool = false;
			ituWidgetSetVisible(sChargeSprite, false);			
		}	
	}
	if(EmgControl)
	{
		printf("ChargeOnLeave :: EmgControl %d \n", EmgControl);

		TopStopStepAnimation();	 
		sCharging = false;
		WattHourMeterStopMonitoring(activeCh);
		if(sChargeSpritePlaybool)
		{
			sChargeSpritePlaybool = false;
			ituSpriteStop(sChargeSprite);
			ituSpriteGoto(sChargeSprite, 0);
		}
		if(sChargeSpritebool)
		{
			sChargeSpritebool = false;
			ituWidgetSetVisible(sChargeSprite, false);			
		}

		// CsConfigVal.bReqRmtStopTSFlg = false;
	}

	CsConfigVal.bReqAuthNo = false;
	CardReaderStopMonitoring();
	
	// setTouchKeyListener(NULL);

	usleep(200*1000);
	CtLogRed("Leave charge layer\n");	
	return true;
}

void ChargeReset(void)
{
	sLcgBackground = NULL;
}


