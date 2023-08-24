#ifndef __HMC_H
#define __HMC_H

void HMC5883L_GetData(int16_t*AccX,int16_t*AccY,int16_t*AccZ);
void HMC5883L_Init(void);
void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t HMC5883L_ReadReg(uint8_t RegAddress);

#endif
