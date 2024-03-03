#include "key.h"


unsigned char Key_Read_BTN(void)
{
	if(P33 == 0)
		return 4;
	else if(P32 == 0)
		return 5;
	else if(P31 == 0)
		return 6;
	else if(P30 == 0)
		return 7;
	else
		return 0;
	
}


unsigned char Key_Read_KBD(void)
{
	unsigned int Key_New = 0;
	unsigned char Key_Val = 0;
	P44 = 0;
	P42 = 1;
	P35 = 1;
	P34 = 1;
	// xxxx xxxx xxxx S4 S5 S6 S7
	Key_New = (P3 & 0x0f);
	
	P44 = 1;
	P42 = 0;
	P35 = 1;
	P34 = 1;
	// xxxx xxxx S4 S5 S6 S7 S8 S9 S10 S11
	Key_New = (Key_New << 4) | (P3 & 0x0f);
	
	P44 = 1;
	P42 = 1;
	P35 = 0;
	P34 = 1;
	// xxxx S4 S5 S6 S7 S8 S9 S10 S11 S12 S13 S14 S15
	Key_New = (Key_New << 4) | (P3 & 0x0f);
	
	P44 = 1;
	P42 = 1;
	P35 = 1;
	P34 = 0;
	// S4 S5 S6 S7 S8 S9 S10 S11 S12 S13 S14 S15 S16 S17 S18 S19
	Key_New = (Key_New << 4) | (P3 & 0x0f);
	
	switch(~Key_New)
	{
		case 0x8000:
			Key_Val = 4;
			break;
		case 0x4000:
			Key_Val = 5;
			break;
		case 0x2000:
			Key_Val = 6;
			break;
		case 0x1000:
			Key_Val = 7;
			break;
		
		case 0x0800:
			Key_Val = 8;
			break;
		case 0x0400:
			Key_Val = 9;
			break;
		case 0x0200:
			Key_Val = 10;
			break;
		case 0x0100:
			Key_Val = 11;
			break;
		
		case 0x0080:
			Key_Val = 12;
			break;
		case 0x0040:
			Key_Val = 13;
			break;
		case 0x0020:
			Key_Val = 14;
			break;
		case 0x0010:
			Key_Val = 15;
			break;
		
		case 0x0008:
			Key_Val = 16;
			break;
		case 0x0004:
			Key_Val = 17;
			break;
		case 0x0002:
			Key_Val = 18;
			break;
		case 0x0001:
			Key_Val = 19;
			break;
		default:
			Key_Val = 0;
			break;
	}
	return Key_Val;
}