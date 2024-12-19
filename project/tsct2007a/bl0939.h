#ifndef __BL0939_H__
#define __BL0939_H__

#define BL0939_IA_FAST_RMS_ADDR 			0x00		//Unsigned Current RMS Register. Faster but approximate vaule. F_RMS = |WAVE|
#define BL0939_IA_WAVE_ADDR 				0x01		//Signed Current Waveform Register. (IP,IN) -> PGA -> ADC -> SINC3 -> HPF -> WAVE
#define BL0939_IB_WAVE_ADDR	 				0x02		//Signed 24-bit, 7.8k update rate.
#define BL0939_V_WAVE_ADDR	 				0x03		//Signed Voltage Waveform Register. (VP) -> PGA -> ADC -> SINC3 -> HPF -> WAVE
#define BL0939_IA_RMS_ADDR 					0x04		//Unsigned Current RMS Register. A_RMS = 324004 * I(A) / Vref. 
#define BL0939_IB_RMS_ADDR 					0x05		//Vref => Internal Voltage Ref = 1.218v(Typical Value) I(A) => Input Voltage(mV) Between (IP1, IN1)
#define BL0939_V_RMS_ADDR	 				0x06		//Unsigned Voltage RMS Register V_RMS = 79931 * V(V) / Vref. V(V) => Input Voltage(mV) of (VP)
#define BL0939_IB_FAST_RMS_ADDR 			0x07		//Unsigned Current RMS Register. Faster but approximate vaule. F_RMS = |WATT|
#define BL0939_A_WATT_ADDR 					0x08		//Active Power Register. 4046*I(A) * V(V) * Cos(pi) / Vref^2
#define BL0939_B_WATT_ADDR 					0x08		//I(A) = Channel Input Signal(mV), V(V) = Vp Input Signal(mV), pi = phase angle of I(A) & V(V).
#define BL0939_CFA_CNT_ADDR 				0x0A		//Energy Pulse Count.
#define BL0939_CFB_CNT_ADDR 				0x0B		//Energy Pulse Count.
#define BL0939_A_CORNER_ADDR 				0x0C		//Current Voltage Waveform Phase Angle Register. 2*pi*CORNER*(f_C/f_0)
#define BL0939_B_CORNER_ADDR 				0x0D		//f_C = AC Freq(MODE.AC_FREQ_SEL), f_0 : Sampling Freq(Typical value = 1MHz)
#define BL0939_TPS1_ADDR 					0x0E		//Unsigned Internal Temp Value Register. T = (170/448)*(TPS1/2 - 32)-45
#define BL0939_TPS2_ADDR 					0x0F		//Unsigned External Temp Value Register.

#define BL0939_IA_FAST_RMS_CTRL_ADDR 		0x10		//Fast RMS Control Register (A Ch.)
#define BL0939_IA_RMSOS_ADDR 				0x13		//Current Voltage RMS Offset Correction Register. RMSOS = (RMS^2 - RMS0^2) / (9.3132 * 2^15)
#define BL0939_IB_RMSOS_ADDR	 			0x14		//RMS => Effective Value After Correction, RMS0 => Effective Value Before Correction
#define BL0939_A_WATTOS_ADDR	 			0x15		//Active Power Offset Correction Register. (A Ch.) WATTOS = (WATT - WATT0) / (8*3.05172)
#define BL0939_B_WATTOS_ADDR 				0x16		//Active Power Offset Correction Register. (B Ch.) 
#define BL0939_WA_CREEP_ADDR 				0x17		//Active Power Anti-Creep Register, WA_CREEP = WATT / (3.0517578125*8)
#define BL0939_MODE_ADDR	 				0x18		//User Mode Select Register
#define BL0939_SOFT_RESET_ADDR 				0x19		//Write 0x5A5A5A to Reset User Area
#define BL0939_USR_WRPROT_ADDR 				0x1A		//Write 0x55 for enable write to 0x18
#define BL0939_TPS_CTRL_ADDR 				0x1B		//Temperature Mode Control
#define BL0939_TPS2_A_ADDR 					0x1C		//External Termp Sensor Gain Coefficient A Correction Register
#define BL0939_TPS2_B_ADDR 					0x1D		//External Termp Sensor Gain Coefficient B Correction Register
#define BL0939_IB_FAST_RMS_CTRL_ADDR 		0x1E		//Fast RMS Control Register (B Ch.)

#define RAWDATA_TO_INT(RAW_DATA_ARRAY) ((*(uint32_t*)(RAW_DATA_ARRAY)) & 0xFFFFFF)

#define ENABLE_WRITE 0x3B000055U

#define SOFT_RESET 0x5A5A5A

#define SET_MODE(RMS_UPDATE_RATE, AC_FREQ, CF_SEL, CF_UNABLE)   \
( (((RMS_UPDATE_RATE) && 1) << 8) | (((AC_FREQ) && 1) << 9) | (((CF_SEL) && 1) << 11) | (((CF_UNABLE) && 1) << 12) )

#define SET_TPS_CTRL(EXTERNAL_THRESHOLD, MEASURE_INTERVAL, MEASURE_SEL, ALERT_CTRL, MEASURE_SWITCH)   \
( ((EXTERNAL_THRESHOLD) & 0x3FF) | (((MEASURE_INTERVAL) & 0b11) << 10) | (((MEASURE_SEL) & 0b11) << 12) | (((ALERT_CTRL) && 1) << 14) | (((MEASURE_SWITCH) && 1) << 12) )

#define SET_FAST_RMS_CTRL(IS_FULL_CYCLE, THRESHOLD) \
( ( ((IS_FULL_CYCLE) && 1) << 15 ) | ( (THRESHOLD) & 0x7FFF )  )

#define VALUE_WITH_CHECKSUM(ADDR, VALUE) \
( (((~(0x55 + (ADDR) + (((VALUE) & 0XFF0000) >> 16) + (((VALUE) & 0XFF00) >> 8) + ((VALUE) & 0XFF)) ) & 0xFF) << 24) | ((VALUE) & 0XFFFFFF) )

typedef union
{
	struct
	{
		uint16_t reserved_1 			: 8;
		uint16_t RMS_UPDATE_SEL 		: 1;		//RMS Register Refresh Time   	0 : 400ms (default), 1 : 800ms
		uint16_t AC_FREQ_SEL 			: 1;		//AC Frequency 					0 : 50Hz -> 40ms (default), 1 : 60Hz -> 33ms
		uint16_t reserved_2 			: 1;
		uint16_t CF_SEL 				: 1;		//CF Pin Output Pulse      		0 :  A Ch. Energy Pulse (default), 1 : B Ch. Energy Pulse
		uint16_t CF_UNABLE 				: 1;		//CF Pin Output Function   		0 : Use Mode[11] - Energy Pulse (default), 1 : Use TPS[14] - A Ch. Temp/Leak Alarm
		uint16_t reserved_3 			: 3;
	};
	uint16_t value; 								//Default = 0x0000
}BL0939_MODE_REGISTER;  //0x18

typedef union
{
	struct
	{
		uint16_t EXTERNAL_TEMP_ALARM_THRES 	: 10;	//External Temp Measure Alarm Threshold
		uint16_t TEMP_MEASURE_INTERVAL		: 2;	//Temp Measure Interval					00 : 50ms (default), 01 : 100ms, 10 : 200ms, 11 : 400ms
		uint16_t TEMP_MEASURE_SEL 			: 2;	//Temp Measurement Selection   			00 : Auto (default), 01 : Same as 01, 10 : Internal, 11 : External
		uint16_t ALERT_CTRL 				: 1;	//CF Function Selection   				0 : Temp Alarm On (default), 1 : Current A Ch. Overcurrent/Leak Alarm On
		uint16_t TEMP_MEASURE_SWITCH 		: 1;	//Temp Measure Switch					0 : On (default), 1 : Off
	};
	uint16_t value; 								//Default = 0x07FF
} BL0939_TPS_CTRL_REGISTER;  //0x1B

typedef union
{
	struct
	{
		uint16_t FAST_RMS_THRESHOLD 		: 15;		//RMS Threshold for Overcurrent/Leak Alarm
		uint16_t FAST_RMS_REFRESH 			: 1;		//0 : Half Cycle, 1 : Cycle(default)
	};
	uint16_t value; 									//default = 0xFFFF
} BL0939_FAST_RMS_CTRL_REGISTER;  //0x10 0x1E

typedef union 
{
	uint8_t data[3];
	struct{
		uint8_t l;
		uint8_t m;
		uint8_t h;
	};
} BL0939_RAW_DATA;

typedef union
{
	uint8_t data[4];
	uint32_t value;
} BL0939_DATA;



typedef union {
	uint8_t fullPacket[35];
	struct{
		uint8_t header;
		BL0939_RAW_DATA IA_FAST_RMS;
		BL0939_RAW_DATA IA_RMS;
		BL0939_RAW_DATA IB_RMS;
		BL0939_RAW_DATA V_RMS;
		BL0939_RAW_DATA IB_FAST_RMS;
		BL0939_RAW_DATA A_WATT;
		BL0939_RAW_DATA B_WATT;
		BL0939_RAW_DATA CFA_CNT;
		BL0939_RAW_DATA CFB_CNT;
		BL0939_RAW_DATA TPS1;
		BL0939_RAW_DATA TPS2;
		uint8_t checksum;
	};
} BL0939_RAW_FULL_PACKET;

typedef void (*BL0939Listener)(char*, int size);

bool BL0939Check(void);
void BL0939Init(void);

#endif


