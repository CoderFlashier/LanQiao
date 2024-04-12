#include "led.h"


void LED_Display(unsigned char ucLED)
{
	P0 = ~ucLED;

	P2 = P2 & 0x1f | 0x80;
	//0x1F -- 0001 1111
	//0x80 -- 1000 0000
	
	P2 = P2 & 0x1f;
}