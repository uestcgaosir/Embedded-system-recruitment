#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "angledisplay.h"
#include "hmc.h"
#include "math.h"
#include "PWM.h"
#include "Serial.h"


#define alpha 0.2
#define pi 3.14159
float simpleEMA(float rawValue, float filteredValue) //一阶指数移动平均滤波
{
    return alpha * rawValue + (1 - alpha) * filteredValue;
}


int16_t AX, AY, AZ, GX, GY, GZ;
int16_t Hmcx,Hmcy,Hmcz;
int16_t x,y,z;
char *String="turn ";

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	HMC5883L_Init();
	PWM_Init();
	
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		HMC5883L_GetData(&Hmcx,&Hmcy,&Hmcz);

		x=simpleEMA(Hmcx,x);
		y=simpleEMA(Hmcy,y);
		z=simpleEMA(Hmcz,z);
//		H = sqrt(x^2 + y^2 + z^2);
//		int16_t inclination=atan2(y,x);
//		angle = inclination*(180.0/pi);		
//		if(angle<0)
//			angle+=360.0;
//		angle-=245;
		
		
		double ArcTan;
		ArcTan=(double)z/(double)x;
		int Angle;
		Angle=atan(ArcTan)*57.3+90;
		
		
		if(AX>=1&&AX<8)
		{
			OLED_Clear();
			OLED_ShowString(1,1,String);
			OLED_ShowString(1,6,"right");
		}
		else if(AX<255&&AX>245&&AZ<8)
		{
			OLED_Clear();
			OLED_ShowString(1,1,String);
			OLED_ShowString(1,6,"left");
		}
		else if(AY<255&&AY>240)
		{
			OLED_Clear();
			OLED_ShowString(1,1,"raise");
		}
		else if(AY<8)
		{
			OLED_Clear();
			OLED_ShowString(1,1,"forerake");
		}
		else if(AX==255&&AY==255&&AZ==8)
		{
			OLED_Clear();
			OLED_ShowString(1,1,"Current state");
			OLED_ShowString(2,1,"is horizontal");
		}
		
		int16_t pwmsign=1240-(Angle*1000/360-400);
//		OLED_ShowSignedNum(3,8,pwmsign,5);
		PWM_SetCompare1(pwmsign);
		
	}
}
