#include "timer.h"


//void Timer0_Init(void)		//12微秒@12.000MHz
//{
//	AUXR &= 0x7F;			//定时器时钟12T模式
//	TMOD &= 0xF0;			//设置定时器模式
//	TL0 = 0xF4;				//设置定时初始值
//	TH0 = 0xFF;				//设置定时初始值
//	TF0 = 0;				//清除TF0标志
//	TR0 = 0;				//定时器0不开始计时
//}


void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x20;				//设置定时初始值
	TH1 = 0xD1;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;
}


//NE555
void Timer0_Init(void)
{
	TMOD &= 0xf0;
	TMOD |= 0x05;
	TL0 = 0;
	TH0 = 0;
	TR0 = 1;
}