#include "init.h"


void Cls_Peripheral(void)
{
	//LED_init
	P0 = 0xff;
	P2 = P2 & 0x1f | 0x80;	
	P2 = P2 & 0x1f;
	
	//BUZZER_init
	P0 = 0x00;
	P2 = P2 & 0x1f | 0xa0;
	P2 = P2 & 0x1f;
}