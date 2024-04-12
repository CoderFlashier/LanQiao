#include "sonic.h"

//
void InitSonicTimer(void)	
{
	AUXR &= 0xBF;
	TMOD &= 0x0F;
	TL1 = 0x00;
	TH1 = 0x00;
	TF1 = 0;
	TR1 = 0;	//定时器暂停
}

//
void DelaySonic(void)		//@12MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 36;
	while (--i);
}

//
void SendDriverSignal(void)
{
	EA = 0; //
	
	SONIC_TX = 1;
	DelaySonic();
	SONIC_TX = 0;
	DelaySonic();
	SONIC_TX = 1;
	DelaySonic();
	SONIC_TX = 0;
	DelaySonic();
	SONIC_TX = 1;
	DelaySonic();
	SONIC_TX = 0;
	DelaySonic();
	SONIC_TX = 1;
	DelaySonic();
	SONIC_TX = 0;
	DelaySonic();
	SONIC_TX = 1;
	DelaySonic();	
	SONIC_TX = 0;
	DelaySonic();
	SONIC_TX = 1;
	DelaySonic();
	SONIC_TX = 0;
	DelaySonic();
	SONIC_TX = 1;
	DelaySonic();
	SONIC_TX = 0;
	DelaySonic();

	EA = 1;
}

//
unsigned int CalDistance(void)
{

	unsigned int t = 0;
	
	SendDriverSignal();
	TR1 = 1;
	while((SONIC_RX == 1) && (TF1 == 0));
	TR1 = 0;

	//测距超时
	if(TF1 == 1){
		TF1 = 0;
		TH1 = 0;
		TL1 = 0;
		return 255;
	}else{
		t = ((TH1 << 8) | TL1);
		TF1 = 0;
		TH1 = 0;
		TL1 = 0;
		return (unsigned char )(t * 0.017);
	}
}
