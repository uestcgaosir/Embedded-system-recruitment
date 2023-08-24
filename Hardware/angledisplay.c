#include "stm32f10x.h"
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "MPU6050.h"
#include <math.h>

#define RAD_TO_DEG 57.2957795131

void MPU6050_GetAngles(float *roll, float *pitch) 
{
    int16_t *AccX, *AccY, *AccZ,*GyroX,*GyroY,*GyroZ;
    float accAngleX, accAngleY;

    MPU6050_GetData(AccX, AccY,  AccZ,  GyroX, GyroY, GyroZ);


    accAngleX = atan2f((float)*AccY, (float)*AccZ) * RAD_TO_DEG;
    accAngleY = atan2f((float)*AccX, (float)*AccZ) * RAD_TO_DEG;

    *roll = accAngleX;
    *pitch = accAngleY;
}


