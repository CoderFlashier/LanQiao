#include "buzz_relay.h"


void Set_Peripheral(unsigned char Buzzer_Status, unsigned char Realy_Status)
{
	unsigned char Peripheral_Val = 0x00;
	
	if(Buzzer_Status == 1)
	{
		Peripheral_Val |= 0x40; // 0100 0000
	}
	
	if(Realy_Status == 1)
	{
		Peripheral_Val |= 0x10; // 0001 0000
	}
	P0 = Peripheral_Val;
	P2 = P2 & 0x1f | 0xa0;// 0001 1111 | 1010 0000
	P2 = P2 & 0x1f; // 1011 1111 & 0001 1111
}