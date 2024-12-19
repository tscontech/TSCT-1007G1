#include <string.h>
#include <unistd.h>
#include "ite/audio.h"
#include "audio_mgr.h"
#include "ctrlboard.h"
#include "tsctcommon.h"
#include "bl0939.h"
#include "tsctcfg.h"
#include "cstordersequence.h"

#include "ite/ith.h"
#include "ite/itp.h"
#include "uart/uart.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>




//-----------------------------------------------------------------------
// MACRO
//-----------------------------------------------------------------------
#define READ_232_TASK_DELAY		100 // ms
#define READ_ID_TASK_DELAY		100 // ms
#define READ_DEV_ADDR           0x55
#define WRITE_DEV_ADDR          0xA5

#define SINGLE_PACKET_SIZE      6
#define SINGLE_WRITE_SIZE       6
#define FULL_PACKET_SIZE        35
#define SINGLE_READ_SIZE        4
#define REQ_SIZE                2
#define DATA_SIZE               sizeof(BL0939_RAW_DATA)

#define DUMP_FULL_PACKET()      printf("[BL0939] FULL PACKET DUMP\n\
\tDUMP:\t%02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n\
\t\t%02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n\
\t\t%02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n\
\t\t%02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n",         \
writeData[0],writeData[1],writeData[2],writeData[3],writeData[4],writeData[5],writeData[6],writeData[7],            \
writeData[8],writeData[9],writeData[10],writeData[11],writeData[12],writeData[13],writeData[14],writeData[15],      \
writeData[16],writeData[17],writeData[18],writeData[19],writeData[20],writeData[21],writeData[22],writeData[23],    \
writeData[24],writeData[25],writeData[26],writeData[27],writeData[28],writeData[29],writeData[30],writeData[31],    \
writeData[32],writeData[33],writeData[34],writeData[35],writeData[36],writeData[37],writeData[38],writeData[39],    \
writeData[40],writeData[41],writeData[42],writeData[43],writeData[44],writeData[45],writeData[46],writeData[47],    \
writeData[48],writeData[49],writeData[50],writeData[51],writeData[52],writeData[53],writeData[54],writeData[55],    \
writeData[56],writeData[57],writeData[58],writeData[59],writeData[60],writeData[61],writeData[62],writeData[63])

#define DUMP_SINGLE_PACKET()      printf("[BL0939] SINGLE PACKET DUMP\n\
\tDUMP:\t%02x  %02x  %02x  %02x  %02x  %02x\n",         \
writeData[0],writeData[1],writeData[2],writeData[3],writeData[4],writeData[5])

//-----------------------------------------------------------------------
// Local Variable
//-----------------------------------------------------------------------

static pthread_t sBLTask, sBLReadTask;
static pthread_attr_t sBLTaskAttr;
struct sched_param sBLSchedParam;
static bool sReadRunning = true;
static ReadQueue sBLReadQueue;

static pthread_t sMonitoringTask1;
static bool sMonitoringTaskExit = false;
static BL0939Listener sListener = NULL;

static uint_fast32_t errorCount = 0;

static pthread_t sBLLeak;
static bool isLeaked = false;

enum{
	UID_CARD=1,
	TMONEY_CARD,
	OTHER_CARD
};

//-----------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------

static inline void busyWait(int cycle)
{
    while(--cycle > 0)
    {
        cycle = cycle;
    }
    return ;
}

static void* BLReadTask(void* arg)
{
	ReadQueueInit(&sBLReadQueue);

    while(1)
    {
		int i;
		char buffer[64];

		if(sReadRunning){
			memset(buffer, 0, 64);
			int bufsize = read(BL0939_DEV, buffer, 64);
			for (i = 0; i < bufsize; i++)
				ReadQueueEnqueue(&sBLReadQueue, buffer[i]);	
		}
        usleep(100*1000);
	}
		
	sBLReadTask = 0;
	CtLogYellow("[BL0939] exit read thread\n");
}

/**
 * @brief Calculate Checksum
 * 
 * @param data Full data with dev address and checksum
 * @param length Full data length with dev address and checksum
 * @return uint8_t calculated checksum
 */
static uint8_t calcCheckSum(uint8_t* data, size_t length)
{
    //Checksum = ~( (DevAddr + RegAddr + Data_L + Data_M + Data_H ) & 0xFF)
    uint8_t sum = 0;

    for(size_t i = 0; i < length-1; ++i)
    {
        sum += data[i];
    }

    return ~sum;
}

/**
 * @brief Test Checksum
 * 
 * @param data Full data with dev address and checksum
 * @param length Full data length with dev address and checksum
 * @return bool Is checksum correct?
 */
static bool testCheckSum(uint8_t* data, size_t length)
{
    //Checksum = ~( (DevAddr + RegAddr + Data_L + Data_M + Data_H ) & 0xFF)

    return (calcCheckSum(data, length) ^ data[length-1] == 0) ? true : false;
}

/**
 * @brief Write Single Register BL0939
 * 
 * @param addr Target Register Address
 * @param data 3 Byte Write Data (without checksum)
 * @return int Write Data Length
 */
int BL0939Write(uint8_t addr, uint8_t *data)
{
    //SEND 0xA?(Dev Addr), Addr[0..7](Reg), DATA_L[0..7], DATA_M[0..7], DATA_H[0..7], CHECKSUM[0..7]
    //? will be 4 bit value [A4 A3 A2 A1] for SOP20L (High to 1, Low to 0), [0 1 0 1] for SOP16L
    //Data length must 3 byte. invaild data byte will be filled with 0.

	//DumpBuffer("232 Reqeust", data, size);
    uint8_t* writeData = calloc(SINGLE_PACKET_SIZE, sizeof(uint8_t));
    int ret;

    writeData[0] = WRITE_DEV_ADDR;
    writeData[1] = addr;

    for(size_t i = 0; i < DATA_SIZE ; ++i)
        writeData[i + REQ_SIZE] = data[i];

    writeData[SINGLE_PACKET_SIZE - 1] = calcCheckSum(writeData, SINGLE_PACKET_SIZE);
    
    ret = write(BL0939_DEV, writeData, SINGLE_PACKET_SIZE);
    usleep(500*1000);

    free(writeData);

	return ret;
}

/**
 * @brief Read Single Register BL0939
 * 
 * @param addr Target Register Address
 * @param buffer 4 Byte Read Data Buffer (with checksum)
 * @param bufferSize Read Data Buffer's Size with checksum
 * @return int Read Data Length
 */
//int BL0939Read(uint8_t addr, uint8_t* buffer, size_t bufferSize)
int BL0939Read(uint8_t addr, uint8_t* buffer)
{
    //SEND 0x5?(Dev Addr), Addr[0..7](Reg)
    //? will be 4 bit value [A4 A3 A2 A1] for SOP20L, [0 1 0 1] for SOP16L

    //GET  DATA_L[0..7], DATA_M[0..7], DATA_H[0..7], CHECKSUM[0..7]
    //Data length must 3 byte. invaild data byte will be filled with 0.

    //Checksum = ~(DevAddr + Addr + DATA...)

    //SEND  FRAME HEADER[]
    //Don't know about frame header. Is it next connection data???
    //After 18.5ms, UART Reset Automatically, so do nothing to end connection.
    //After 6.65ms from RX Pull up, UART will be Reset.

    uint8_t* writeData = calloc(64, sizeof(uint8_t));
    int ret;

    writeData[0] = READ_DEV_ADDR;
    writeData[1] = addr;

    // pthread_mutex_lock(&blLock);
    ret = write(BL0939_DEV, writeData, 1);
	ret += write(BL0939_DEV, writeData+1, 1);  //Simulate stopbit 1.5

    if(ret != 2)
    {
        CtLogRed("[BL0939] Read Failed!!! (Send data broken)\n");
        ++errorCount;
        return -1;
    }


    usleep(500*1000);

    for(int i = REQ_SIZE; i < SINGLE_PACKET_SIZE; i += ret)
    {
        ret = read(BL0939_DEV, writeData + i, SINGLE_READ_SIZE);
        // busyWait(116);
    }

    DUMP_SINGLE_PACKET();

    // pthread_mutex_unlock(&blLock);
    

    

    //TODO: usleep을 줘야할 수도 있음! 테스트 할 것!
    // if(ret == bufferSize + 3)
    // {
    //     CtLogRed("[BL0939] Read Failed!!! (Get data failed)\n");
    //     return -1;
    // }

    if(testCheckSum(writeData, SINGLE_PACKET_SIZE))
    {
        // CtLogGreen("[BL0939] Checksum PASS!\n");
        errorCount = 0;
    }
    else
    {
        CtLogRed("[BL0939] Checksum FAIL!\n");
        ++errorCount;
    }

    memcpy(buffer, writeData+2, SINGLE_READ_SIZE);

    free(writeData);
	
	return ret;
}

int BL0939DummyRead()
{
    uint8_t* writeData = calloc(64, sizeof(uint8_t));
    size_t ret;

    for(int i = REQ_SIZE; i < SINGLE_PACKET_SIZE; i += ret)
    {
        ret = read(BL0939_DEV, writeData + i, 64);
    }
	
    free(writeData);
	
	return ret;
}

/**
 * @brief Read BL0939 (Op 0xAA)
 * 
 * @param buffer Read Data Buffer with checksum
 * @return int Read Data Length
 */
int BL0939FullRead(uint8_t* buffer)
{
    //SEND 0x5?(Dev Addr), Addr[0..7](Reg)
    //? will be 4 bit value [A4 A3 A2 A1] for SOP20L, [0 1 0 1] for SOP16L

    //GET  HEADER(0x55) DATA0[0..23], ... ,DATA11[0..23], CHECKSUM[0..7]
    //Data length must 3 byte. invaild data byte will be filled with 0.

    //Checksum = ~(DevAddr + HEADER + DATA...)

    uint8_t* writeData = calloc(64, sizeof(uint8_t));
    ssize_t ret;

    writeData[0] = READ_DEV_ADDR;
    writeData[1] = 0xAA;  //Operation 0xAA

    // pthread_mutex_lock(&blLock);
    ret = write(BL0939_DEV, writeData, 1);
	ret += write(BL0939_DEV, writeData+1, 1);  //Simulate stop bit 1.5

    if(ret != 2)
    {
        CtLogRed("[BL0939] Read Failed!!! (Send data broken)\n");
        ++errorCount;
        return -1;
    }
    
    ret = 0;

    usleep(500*1000);  //Wait 76 micro sec for first receive.

    for(int i = REQ_SIZE; i < sizeof(BL0939_RAW_FULL_PACKET) + REQ_SIZE;)
    {
        ret += read(BL0939_DEV, writeData + i, sizeof(BL0939_RAW_FULL_PACKET));
        i += ret;
        // busyWait(116);  //Wait 116 micro sec for next byte. Not needed here.
    }
    // pthread_mutex_unlock(&blLock);
    DUMP_FULL_PACKET();

    writeData[1] = 0x55;  //For test checksum.
    if(testCheckSum(writeData+1, sizeof(BL0939_RAW_FULL_PACKET) + 1))
    {
        // CtLogGreen("[BL0939] Checksum PASS!\n");
        errorCount = 0;
    }
    else
    {
        CtLogRed("[BL0939] Checksum FAIL!\n");
        memcpy(buffer, writeData+2, 35);
        ++errorCount;

        return -ret;
    }

    memcpy(buffer, writeData+2, 35);

    free(writeData);
	
	return ret;
}

//********************* HELPER Function*********************************** */



//********************* Test Function*********************************** */

static bool RequestTest(void)
{
    //0xA5(Dev Addr), 0xAA is Get Full Power Parameter Packet.
    //Output will be 35 byte length with checksum.
	BL0939_RAW_FULL_PACKET buffer;
    int ret = BL0939FullRead(&buffer);
	// int ret = BL0939FullRead(&buffer, sizeof(BL0939_FULL_PACKET));

    // if(ret == 35)
//     printf("[BL0939] DUMP : \n\
// \theader = %x\n\
// \tIA_FAST_RMS = %x %x %x\n\
// \tIA_RMS = %x %x %x\n\
// \tIB_RMS = %x %x %x\n\
// \tV_RMS = %x %x %x\n\
// \tIB_FAST_RMS = %x %x %x\n\
// \tA_WATT = %x %x %x\n\
// \tB_WATT = %x %x %x\n\
// \tCFA_CNT = %x %x %x\n\
// \tCFB_CNT = %x %x %x\n\
// \tTPS1 = %x %x %x\n\
// \tTPS2 = %x %x %x\n\
// \tchecksum = %x\n\
// \tlength = %d\n",
// buffer.header,
// buffer.IA_FAST_RMS.l, buffer.IA_FAST_RMS.m, buffer.IA_FAST_RMS.h, 
// buffer.IA_RMS.l, buffer.IA_RMS.m, buffer.IA_RMS.h, 
// buffer.IB_RMS.l, buffer.IB_RMS.m, buffer.IB_RMS.h, 
// buffer.V_RMS.l, buffer.V_RMS.m, buffer.V_RMS.h, 
// buffer.IB_FAST_RMS.l, buffer.IB_FAST_RMS.m, buffer.IB_FAST_RMS.h, 
// buffer.A_WATT.l, buffer.A_WATT.m, buffer.A_WATT.h, 
// buffer.B_WATT.l, buffer.B_WATT.m, buffer.B_WATT.h, 
// buffer.CFA_CNT.l, buffer.CFA_CNT.m, buffer.CFA_CNT.h, 
// buffer.CFB_CNT.l, buffer.CFB_CNT.m, buffer.CFB_CNT.h,
// buffer.TPS1.l, buffer.TPS1.m, buffer.TPS1.h,
// buffer.TPS2.l, buffer.TPS2.m, buffer.TPS2.h, 
// buffer.checksum, ret);
    
    return ret == 35 && !errorCount;
}

static bool ReadFullPacket(void)
{
    //0xA5(Dev Addr), 0xAA is Get Full Power Parameter Packet.
    //Output will be 35 byte length with checksum.
	BL0939_RAW_FULL_PACKET buffer;
    int ret = BL0939FullRead(&buffer);
	// int ret = BL0939FullRead(&buffer, sizeof(BL0939_FULL_PACKET));

    if(ret == 35)
    printf("[BL0939] DUMP : \n\
\theader = %x\n\
\tIA_FAST_RMS = %d\n\
\tIA_RMS = %d\n\
\tIB_RMS = %d\n\
\tV_RMS = %d\n\
\tIB_FAST_RMS = %d\n\
\tA_WATT = %d\n\
\tB_WATT = %d\n\
\tCFA_CNT = %d\n\
\tCFB_CNT = %d\n\
\tTPS1 = %d\n\
\tTPS2 = %d\n\
\tchecksum = %x\n\
\tlength = %d\n\
\tINTR = %d\n",
buffer.header,
RAWDATA_TO_INT(buffer.IA_FAST_RMS.data),
RAWDATA_TO_INT(buffer.IA_RMS.data),
RAWDATA_TO_INT(buffer.IB_RMS.data),
RAWDATA_TO_INT(buffer.V_RMS.data),
RAWDATA_TO_INT(buffer.IB_FAST_RMS.data),
RAWDATA_TO_INT(buffer.A_WATT.data),
RAWDATA_TO_INT(buffer.B_WATT.data),
RAWDATA_TO_INT(buffer.CFA_CNT.data),
RAWDATA_TO_INT(buffer.CFB_CNT.data),
RAWDATA_TO_INT(buffer.TPS1.data),
RAWDATA_TO_INT(buffer.TPS2.data),
buffer.checksum, ret, ithGpioGet(GPIO_BL0939_LEAK));
    
    return ret == 35 && !errorCount;
}

bool BL0939Check(void)
{	
	bool testResult = false;

	// sReadRunning = true;

	testResult = RequestTest();

	if (testResult)
	{
		blcheck = false;
		CtLogGreen("[BL0939] Check Test PASS!\n");
	}
	else
	{
		blcheck = true;	
		CtLogRed("[BL0939] Check Test FAILED!\n");
	}
		
	// sReadRunning = false;


	return blcheck;
}

bool testData(uint32_t expected, uint8_t* actual)
{
    BL0939_DATA translated;

    translated.value = expected;

    printf("[BL0939] expected : %X %X %X %X , actual : %X %X %X %X\n", 
            translated.data[0], translated.data[1], translated.data[2], translated.data[3], 
            actual[0], actual[1], actual[2], actual[3]);

    return translated.data[0] == actual[0] 
            && translated.data[1] == actual[1] 
            && translated.data[2] == actual[2]
            && translated.data[3] == actual[3];
}

bool easyWriteData(uint8_t regAddr, uint32_t writeData)
{
    uint8_t data[4] = {0, };

    BL0939Write(regAddr, (uint8_t*)&writeData);

    usleep(200*1000);

    BL0939Read(regAddr, data);

    usleep(200*1000);

    printf("[BL0939] %2X => %2X %2X %2X %2X\n", regAddr, data[0], data[1], data[2], data[3]);

    return testData(writeData, data);
}

bool easyReadData(uint8_t regAddr, uint32_t* readData, uint32_t expected)
{
    uint8_t data[4] = {0, };

    BL0939Read(regAddr, data);

    usleep(200*1000);

    printf("[BL0939] %2X => %2X %2X %2X %2X, %d\n", regAddr, data[0], data[1], data[2], data[3], *(uint32_t*)data & 0x00FFFFFF);

    if(readData)
        readData = *(uint32_t*)data;

    if(expected)
        return testData(expected, data);
    else
        return VALUE_WITH_CHECKSUM(regAddr, *(uint32_t*)data) == *(uint32_t*)data;
}

//********************* RX Function*********************************** */

static void* BLTask(void* arg)
{

    bool notConnected = true;
    uint32_t parameterUpdated = 0;

    uint8_t data[6];
    usleep(3*1000*1000);
    CtLogYellow("[BL0939] run thread..\n");
    while(1)
    {
        if(notConnected)
        {
            notConnected = BL0939Check();

            if(!notConnected)
                printf("[BL0939] First Connect - Pass\n");
            else
                printf("[BL0939] First Connect - Fail\n");

            sleep(5);
            continue;
        }
        // else BL0939DummyRead();
        if(parameterUpdated != 0b10000)
        {
            switch(parameterUpdated)
            {
                case 0 : 
                    BL0939Read(BL0939_TPS_CTRL_ADDR, data);
                    printf("[BL0939] 0x1B => %2x %2x %2x %2x\n", data[0], data[1], data[2], data[3]);
                    if(testData(0x890007ff, data))
                    {
                        parameterUpdated = 1;
                        printf("[BL0939] Read Test - Pass\n");
                    }
                    else printf("[BL0939] Read Test - Fail\n");
                    break;
                case 1 : 
                    if(easyWriteData(BL0939_USR_WRPROT_ADDR, ENABLE_WRITE))
                    {
                        parameterUpdated <<= 1;
                        printf("[BL0939] Enable Write Register - Pass\n");
                    }
                    else printf("[BL0939] Enable Write Register - Fail\n");
                    break;
                case 2 : 
                    if(easyWriteData(BL0939_MODE_ADDR, VALUE_WITH_CHECKSUM(BL0939_MODE_ADDR, SET_MODE(0, 1, 0, 1))))
                    {
                        parameterUpdated <<= 1;
                        printf("[BL0939] Change Freq - Pass\n");
                    }
                    else printf("[BL0939] Change Freq - Fail\n");
                    break;
                case 4 : 
                    // if(easyWriteData(BL0939_IB_FAST_RMS_CTRL_ADDR, 0x0D0080FFU))
                    //Limit to 2048
                    if(easyWriteData(BL0939_IB_FAST_RMS_CTRL_ADDR, VALUE_WITH_CHECKSUM(BL0939_IB_FAST_RMS_CTRL_ADDR, SET_FAST_RMS_CTRL(1, 30) )))
                    {  //Full Cycle, Threshold = (0x7f << 8)
                        parameterUpdated <<= 1;
                        printf("[BL0939] Set RMS Threshold - Pass\n");
                    }
                    else printf("[BL0939] Set RMS Threshold - Fail\n");
                    break;
                case 8 : 
                    printf("[BL0939] Setting Finished!\n");
                    parameterUpdated <<= 1;
                    break;
            }
            usleep(500*1000);
            continue;
        }
        // else
        // {
            // ReadFullPacket();
            // usleep(500*1000);
            //easyReadData(BL0939_IB_FAST_RMS_CTRL_ADDR, NULL, VALUE_WITH_CHECKSUM(BL0939_IB_FAST_RMS_CTRL_ADDR, SET_FAST_RMS_CTRL(1, 0x3) ));
            // easyReadData(BL0939_IB_WAVE_ADDR, NULL, 0);
            // easyReadData(BL0939_IB_FAST_RMS_ADDR, NULL, 0);
            // data[0] = 0x00;
            // data[1] = 0x00;
            // data[2] = 0x00;
            // BL0939Check();
            // usleep(200*1000);
            // BL0939Read(0x1B, data);
            // usleep(200*1000);
            // BL0939Read(0x1A, data);
            // usleep(200*1000);
            // BL0939Read(0x18, data);
            // usleep(200*1000);
            // BL0939Read(0x1E, data);
        // }
        if(errorCount)
            CtLogYellow("[BL0939] Communication Error (Count %d)\n", errorCount);
        sleep(5);
	}
		
	sBLTask = 0;
	CtLogYellow("[BL0939] exit thread\n");
}

void touchToRecoverEMB(bool longpress)
{
    EMBListenerOnCharge12(false);
    setTouchKeyListener(NULL);
}

void BL0939LeakInterrupt(unsigned int pin, void *arg);

void BLLeakTask()
{
    bool isStoped = false;
    bool isLeakFirst = true;
    while (sBLLeak)
    {
        if(isLeaked)
        {
            MagneticContactorOff();
            // ithGpioDisableIntr(GPIO_BL0939_LEAK);
            if(isLeaked = ithGpioGet(GPIO_BL0939_LEAK))
            {
                printf("\x1b[0;31m[BL0939] LEAK DETECTED!!! %d\x1b[0m\n", isLeaked);
            }
            else
            {
                printf("\x1b[0;32m[BL0939] LEAK RECOVERED!!!\x1b[0m\n");
            }
            //TODO: 충전 종료 처리
            if(isLeakFirst)
            {
                isLeakFirst = false;
                if(shmDataAppInfo.app_order == APP_ORDER_CHARGING)
    			    shmDataAppInfo.charge_comp_status = END_ERR;
			    CstSetEpqStatus(TSCT_IN_UNDER_VOLTAGE, false);
			    ShowFatalErrorDialogBox(TSCT_GROUND_FAULT);
            }
            isStoped = false;

        }
        else
        {
            if(!isStoped)
            {
                isStoped = true;
                isLeakFirst = true;
                ithGpioRegisterIntrHandler(GPIO_BL0939_LEAK, BL0939LeakInterrupt, NULL);
                ithIntrEnableIrq(ITH_INTR_GPIO);
                ithGpioEnableIntr(GPIO_BL0939_LEAK);
            }
        }

        usleep(1000);
    }

}


void BL0939LeakInterrupt(unsigned int pin, void *arg)
{
    ithEnterCritical();  // to prevent from interrupt
    ithIntrDisableIrq(ITH_INTR_GPIO);
    ithGpioDisableIntr(GPIO_BL0939_LEAK);
    MagneticContactorOff();
    ithGpioClearIntr(GPIO_BL0939_LEAK);
    isLeaked = true;
    // printf("\x1b[0;31m[BL0939] LEAK DETECTED!!! %d\x1b[0m\n", ithGpioGet(pin));
    ithExitCritical();  //Unlock spinlock
}


void BL0939Init(void)
{
    ithGpioSetMode(GPIO_BL0939_LEAK, ITH_GPIO_MODE0);
    ithGpioSetIn(GPIO_BL0939_LEAK);
    ithGpioCtrlEnable(GPIO_BL0939_LEAK, ITH_GPIO_PULL_ENABLE);  //Enable Pull Up/Down
    ithGpioCtrlDisable(GPIO_BL0939_LEAK, ITH_GPIO_PULL_UP);  // Disable Pull Up to Enable Pull Down
    ithGpioEnable(GPIO_BL0939_LEAK);

    usleep(1000);

    ithGpioClearIntr(GPIO_BL0939_LEAK);
    ithGpioRegisterIntrHandler(GPIO_BL0939_LEAK, BL0939LeakInterrupt, NULL);
    ithGpioCtrlDisable(GPIO_BL0939_LEAK, ITH_GPIO_INTR_LEVELTRIGGER);  //Disable Level Trigger to enable Edge Trigger
    ithGpioCtrlEnable(GPIO_BL0939_LEAK, ITH_GPIO_INTR_BOTHEDGE);  //Enable Both Edge Trigger
    // ithGpioCtrlDisable(GPIO_BL0939_LEAK, ITH_GPIO_INTR_TRIGGERFALLING);  //Disable Falling Edge Trigger to enable Rising Edge Trigger
    ithIntrEnableIrq(ITH_INTR_GPIO);
    ithGpioEnableIntr(GPIO_BL0939_LEAK);

    pthread_attr_init(&sBLTaskAttr);
    sBLSchedParam.sched_priority = 5;
    pthread_attr_setschedparam(&sBLTaskAttr, &sBLSchedParam);

	// if (sBLReadTask==0)
	// {
	// 	CtLogYellow("[RFID] create rs232 read thread..\n");
	// 	pthread_create(&sBLReadTask, NULL, BLReadTask, NULL);
	// 	pthread_detach(sBLReadTask);
	// }		

	if (sBLTask==0)
	{
		CtLogYellow("[BL0939] create thread..\n");
		pthread_create(&sBLTask, &sBLTaskAttr, BLTask, NULL);
		pthread_detach(sBLTask);
	}

    if (sBLLeak == 0)
	{
		CtLogYellow("[BL0939] create Leak thread..\n");
		pthread_create(&sBLLeak, NULL, BLLeakTask, NULL);
		pthread_detach(sBLTask);
	}
	
}


