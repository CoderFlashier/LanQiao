#incldue "buzz_relay.h"

void Buzzle_Relay(unsigned char dat)
{
	unsigned char temp = 0;
	if(dat == 1 || dat == 3)
		temp |= 0x40;
	if(dat == 2 || dat == 3)
		temp |= 0x10;
	
	P0 = temp;
	P2 = (P2 & 0x1f) | 0xa0;
	P2 &= 0x1f;
}