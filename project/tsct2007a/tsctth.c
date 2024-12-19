#include "tsctth.h"

#include "ite/itp.h"
#include "iic/mmp_iic.h"

pthread_t sTHTask;

void TH_i2c_write_byte(uint8_t dev_addr, uint8_t addr, uint8_t data)
{
	uint8_t CmdBuf[2];

	#if 0
		/* dump write_data for check */
		printf("[it6151_i2c_write] dev_addr = 0x%x, write_data[0x%x] = 0x%x \n", dev_addr, CmdBuf[0], CmdBuf[1]);
	#endif

	CmdBuf[0] =addr;
	CmdBuf[1] =data;
	// mmpIicSendDataEx(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, CmdBuf, 2);
	mmpIicSendDataEx(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, CmdBuf, 1);
	usleep(300);
}


void TH_i2c_read_byte(uint8_t dev_addr, uint8_t addr, uint8_t* dataBuffer)
{
	uint8_t len;

	// *dataBuffer = addr;

	// mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, &addr, 1, dataBuffer, 1);
	mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, dev_addr, &addr, 0, dataBuffer, 6);

	#if 0
		/* dump write_data for check */
		printf("[it6151_read_byte] dev_addr = 0x%x, read_data[0x%x] = 0x%x \n", dev_addr, addr, *dataBuffer);
	#endif
	usleep(300);
}

void getTH()
{

    unsigned char I2C_temp1[6]={0, };

	int Temp_degC=0, Humi_RH=0;

	TH_i2c_write_byte(Temp_I2C_ADDR, 0xFD, 0xFD);
	usleep(10000);
	TH_i2c_read_byte(Temp_I2C_ADDR, 0xFD, &I2C_temp1);
	printf("MainStartOnPress <%d> <%d> <%d> <%d> <%d> <%d> \n", I2C_temp1[0], I2C_temp1[1], I2C_temp1[2], I2C_temp1[3], I2C_temp1[4], I2C_temp1[5]);

	//섭씨
	Temp_degC= (175* (I2C_temp1[0] *256+I2C_temp1[1]) /65535) -45;
	Humi_RH= (125* (I2C_temp1[3] *256+I2C_temp1[4]) /65535) -6;

    printf("MainStartOnPress <Temp_degC : %d> <Humi_RH : %d> \n", Temp_degC, Humi_RH);

}

void THTask(void)
{
	while(sTHTask)
	{
		getTH();  //TODO: Temp와 Humid의 출력을 밖에서 처리할 것.(되도록이면 GUI 상단에 출력되도록)
		sleep(5);
	}
}

void THInit()
{
	if (sTHTask==0)
	{
		if(pthread_create(&sTHTask, NULL, THTask, NULL) != 0)
		{
			printf("[TH] init failed!!!\n");  //TODO: 빨갛게 만들 것
		}
		pthread_detach(sTHTask);
	}
}