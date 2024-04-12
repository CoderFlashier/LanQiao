#include "ultrasonic.h"

unsigned char Ultrasonic_Test(void)
{
	unsigned char n = 10;
	
	//定时器复位
	Tx = 0;
	TL0 = 0xF4;
	TH0 = 0xFF;
	TR0 = 1;//开始计时
	
	while(n--)
	{
		while(!TF0);
		Tx ^= 1;
		TF0 = 0;
	}
	
	//开始正计时
	TR0 = 0;
	TL0 = 0;
	TH0 = 0;
	TR0 = 1;
	
	while(Rx && !TF0);
	TR0 = 0;
	
	if(TF0)
		return 255;
	else
		return ((TH0 << 8) | TL0) * 0.017;
	//定时器为12T模式，每1us记一次
}