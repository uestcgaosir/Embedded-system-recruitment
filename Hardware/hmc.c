#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "hmc_reg.h"

#define HMC5883L_ADDRESS 0x1E
#define HMC5883L_READ_ADDRESS 0x3D
#define HMC5883L_WRITE_ADDRESS 0x3C


//读寄存器(地址）
uint8_t HMC5883L_ReadReg(uint8_t RegAddress)
{
		uint8_t Data;
		MyI2C_Start();
		MyI2C_SendByte(HMC5883L_WRITE_ADDRESS);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress);
		MyI2C_ReceiveAck();
		
		MyI2C_Start();
		MyI2C_SendByte(HMC5883L_READ_ADDRESS | 0x01);
		MyI2C_ReceiveAck();
		Data = MyI2C_ReceiveByte();
		MyI2C_SendAck(1);
		MyI2C_Stop();			
		return Data;
}


//写寄存器（地址，写入的数据）
void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data)
{
		MyI2C_Start();
		MyI2C_SendByte(HMC5883L_WRITE_ADDRESS);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(Data);
		MyI2C_ReceiveAck();
		MyI2C_Stop();
}



//初始化
void HMC5883L_Init(void)
{
	MyI2C_Init();
	
	HMC5883L_WriteReg(HMC5883L_CON_REG_A,0x70);
	HMC5883L_WriteReg(HMC5883L_CON_REG_B,0x40);
	HMC5883L_WriteReg(HMC5883L_MODE_REG	,0x00);
}




//一次获取hmc多个寄存器（x,y,z轴磁场）
void HMC5883L_GetData(int16_t*AccX,int16_t*AccY,int16_t*AccZ)
{
	uint8_t Data1,Data2;
	//x
	Data1 = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_X_MSB_REG);
	Data2 = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_X_LSB_REG);
	*AccX = (Data1 << 8 | Data2);
	//y
	Data1 = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Y_MSB_REG);
	Data2 = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Y_LSB_REG);
	*AccY = (Data1 << 8 | Data2);
	//z
	Data1 = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Z_MSB_REG);
	Data2 = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Z_LSB_REG);
	*AccZ = (Data1 << 8 | Data2);
}




