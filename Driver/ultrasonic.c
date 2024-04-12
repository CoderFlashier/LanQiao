#include "ultrasonic.h"

unsigned char Wave_Recv(void)
{
	unsigned char ucNum = 10;
	
	Tx = 0;//发射端P10
	TL0 = 0xF4;
	TH0 = 0xFF;
	TR0 = 1;
	
	while(ucNum--)
	{
		while(TF0 == 0)
		Tx ^= 1;
		TF0 = 0;
	}
	
	//作为正计时
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
}