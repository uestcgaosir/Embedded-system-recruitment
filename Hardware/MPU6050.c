#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0
#define FILTER_SIZE 5


int16_t MedianFilter(int16_t *data) {
    int16_t sorted[FILTER_SIZE];
    for (int i = 0; i < FILTER_SIZE; i++) {
        sorted[i] = data[i];
    }


    for (int i = 0; i < FILTER_SIZE - 1; i++) {
        for (int j = 0; j < FILTER_SIZE - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int16_t temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    return sorted[FILTER_SIZE / 2];
}


void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ) {
    int16_t AccXData[FILTER_SIZE];
    int16_t AccYData[FILTER_SIZE];
    int16_t AccZData[FILTER_SIZE];
    int16_t GyroXData[FILTER_SIZE];
    int16_t GyroYData[FILTER_SIZE];
    int16_t GyroZData[FILTER_SIZE];

    for (int i = 0; i < FILTER_SIZE; i++) {
        AccXData[i] = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
        AccYData[i] = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
        AccZData[i] = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
        GyroXData[i] = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
        GyroYData[i] = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
        GyroZData[i] = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    }

    *AccX = MedianFilter(AccXData);
    *AccY = MedianFilter(AccYData);
    *AccZ = MedianFilter(AccZData);
    *GyroX = MedianFilter(GyroXData);
    *GyroY = MedianFilter(GyroYData);
    *GyroZ = MedianFilter(GyroZData);
}
										 
