#include "seg.h"


void Seg_Tran(unsigned char Seg_Val[], unsigned char Seg_Code[])
{
	unsigned char i = 0;
	unsigned char j = 0;
	for(i = 0, j = 0; i <= 7; i++, j++)
	{
		switch(Seg_Val[j])
		{
			case '0':
				Seg_Code[i] = 0xc0;
				break;
			case '1':
				Seg_Code[i] = 0xf9;
				break;
			case '2':
				Seg_Code[i] = 0xa4;
				break;
			case '3':
				Seg_Code[i] = 0xb0;
				break;
			case '4':
				Seg_Code[i] = 0x99;
				break;
			case '5':
				Seg_Code[i] = 0x92;
				break;
			case '6':
				Seg_Code[i] = 0x82;
				break;
			case '7':
				Seg_Code[i] = 0xf8;
				break;
			case '8':
				Seg_Code[i] = 0x80;
				break;
			case '9':
				Seg_Code[i] = 0x90;
				break;
			case 'A':
				Seg_Code[i] = 0x88;
				break;
			case 'b':
				Seg_Code[i] = 0x83;
				break;
			case 'C':
				Seg_Code[i] = 0xc6;
				break;
			case 'd':
				Seg_Code[i] = 0xa1;
				break;
			case 'E':
				Seg_Code[i] = 0x86;
				break;
			case 'F':
				Seg_Code[i] = 0x8e;
				break;
			case ' ':
				Seg_Code[i] = 0xff;
				break;
			case 0:
				Seg_Code[i] = 0xff;
				break;
			
		}
		
		if(Seg_Val[i + 1] == '.')
		{
			Seg_Code[i] &= 0x7f;
			j += 1;
		}
	}
}



void Seg_Display(unsigned char Seg_Code[], unsigned char Seg_Place)
{
	P0 = 0x00;
	P2 = P2 & 0x1f | 0xe0;
	P2 &= 0x1f;
	
	P0 = 1 << Seg_Place;
	P2 = P2 & 0x1f | 0xc0;
	P2 &= 0x1f;
	
	P0 = Seg_Code[Seg_Place];
	P2 = P2 & 0x1f | 0xe0;
	P2 &= 0x1f;
	
//	P0 = Seg_Code[Seg_Place];
//	P2 = P2 & 0x1f | 0xe0; // 1110 0000
//	P2 = P2 & 0x1f;
//	
//	P0 = 1 << Seg_Place;
//	P2 = P2 & 0x1f | 0xc0;
//	P2 = P2 & 0x1f;
}