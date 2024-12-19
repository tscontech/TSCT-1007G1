#include "tscteeprom.h"

#include "ite/itp.h"
#include "iic/mmp_iic.h"

void EEPROM_i2c_write_page(uint8_t dev_addr, uint8_t addr, uint8_t* data, size_t length)
{
	// uint8_t CmdBuf[2];
	uint8_t* CmdBuf = calloc(length + 2, sizeof(uint8_t));

#if 0
    /* dump write_data for check */
    printf("[it6151_i2c_write] dev_addr = 0x%x, write_data[0x%x] = 0x%x \n", dev_addr, CmdBuf[0], CmdBuf[1]);
#endif

	//Write Address Setting
	CmdBuf[0] = 0x00;
	CmdBuf[1] = addr;

	//Write data Setting
	memcpy(CmdBuf + 2, data, length);
	mmpIicSendDataEx(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, CmdBuf, length + 2);

	usleep(300);
}


//이전 명령의 다음 주소에서 1Byte 읽기(Page 끝 도달 시 Page 처음으로 이동 후 계속)
void EEPROM_i2c_read_next(uint8_t dev_addr, uint8_t* dataBuffer)
{
	uint8_t len;
	uint8_t placeHolder = 0x00;  //Dummy Read Address Setting

	mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, &placeHolder, 0, dataBuffer, 1);

#if 0
    /* dump write_data for check */
    printf("[it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *dataBuffer);
#endif
	usleep(300);
}


//이전 명령과 같은 Page(256B 단위)내 addr부터 length 만큼 연속 읽기(page 끝 도달 시 Page 처음으로 이동 후 계속)
void EEPROM_i2c_read_seq(uint8_t dev_addr, uint8_t addr, uint8_t* dataBuffer, size_t length)
{
	mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, &addr, 1, dataBuffer, length);

#if 0
    /* dump write_data for check */
    printf("[it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *dataBuffer);
#endif
	usleep(300);
}

void EEPROM_i2c_write_byte(uint8_t dev_addr, uint8_t addr, uint8_t data)
{
	// uint8_t CmdBuf[2];
	uint8_t CmdBuf[3];

#if 0
    /* dump write_data for check */
    printf("[it6151_i2c_write] dev_addr = 0x%x, write_data[0x%x] = 0x%x \n", dev_addr, CmdBuf[0], CmdBuf[1]);
#endif

	//Write Address Setting
	CmdBuf[0] = 0x00;
	CmdBuf[1] = addr;

	//Write data Setting
	CmdBuf[2] = data;
	mmpIicSendDataEx(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, CmdBuf, 3);

	usleep(300);
}

void EEPROM_i2c_read_byte(uint8_t dev_addr, uint8_t addr, uint8_t* dataBuffer)
{
	uint8_t len;
	uint8_t addrBuf[2] = {0x00, addr};  //Read Address Setting

	mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, addrBuf, 2, dataBuffer, 1);

#if 0
    /* dump write_data for check */
    printf("[it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *dataBuffer);
#endif
	usleep(300);
}

void EEPROMTest()
{
	const uint8_t test = time(NULL);
	uint8_t EEPROM_temp1;

	EEPROM_i2c_write_byte(EEPROM_I2C_ADDR, 0x02, test);
	usleep(500*1000);
	EEPROM_i2c_read_byte(EEPROM_I2C_ADDR, 0x02, &EEPROM_temp1);
	usleep(500*1000);

	printf("Test Value = %d, EEPROM_Read = %d, Test %s\n", test, EEPROM_temp1, test == EEPROM_temp1 ? "Success" : "Failed");
}