#include "init.h"
#include "led.h"
#include "timer.h"


//Timer
unsigned long ulms = 0;
unsigned int uiLED_Delay = 0;
unsigned char ucLed = 0x00;

void LED_Proc(void);


void main()
{
	Cls_Peripheral();
	Timer0_Init();
	EA = 1;
	while(1)
	{
		LED_Proc();
	}
}

void LED_Proc(void)
{
	if(uiLED_Delay < 200)
			return;
	ucLed ^= 0x01;
	LED_Display(ucLed);
	uiLED_Delay = 0;
}


void Timer0(void) interrupt 1
{
	ulms++;
	uiLED_Delay++;
}