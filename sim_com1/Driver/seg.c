#include "seg.h"


void Seg_Tran(unsigned char *pucSeg_Buf, unsigned char *pucSeg_Code)
{
	unsigned char i ,j;
	for(i = 0, j = 0; i <= 7; i++, j++)
	{
		switch(pucSeg_Buf[j])
		{
			case '0':
				pucSeg_Code[i] = 0xc0;
				break;
			case '1':
				pucSeg_Code[i] = 0xf9;
				break;
			case '2':
				pucSeg_Code[i] = 0xa4;
				break;
			case '3':
				pucSeg_Code[i] = 0xb0;
				break;
			case '4':
				pucSeg_Code[i] = 0x99;
				break;
			case '5':
				pucSeg_Code[i] = 0x92;
				break;
			case '6':
				pucSeg_Code[i] = 0x82;
				break;
			case '7':
				pucSeg_Code[i] = 0xf8;
				break;
			case '8':
				pucSeg_Code[i] = 0x80;
				break;
			case '9':
				pucSeg_Code[i] = 0x90;
				break;
			case 'A':
				pucSeg_Code[i] = 0x88;
				break;
			case 'b':
				pucSeg_Code[i] = 0x83;
				break;
			case 'C':
				pucSeg_Code[i] = 0xc6;
				break;
			case 'd':
				pucSeg_Code[i] = 0xa1;
				break;
			case 'E':
				pucSeg_Code[i] = 0x86;
				break;
			case 'F':
				pucSeg_Code[i] = 0x81;
				break;
			case ' ':
				pucSeg_Code[i] = 0xff;
				break;
			case '\0':
				pucSeg_Code[i] = 0xff;
				break;
		}
		if(pucSeg_Buf[j + 1] == '.')
		{
			pucSeg_Code[i] &= 0x7f;
			j++;
		}
	}
}


void Seg_Display(unsigned char *pucSeg_Code, unsigned char ucSeg_Pos)
{
	P0 = pucSeg_Code[ucSeg_Pos];
	P2 = P2 & 0x1f | 0xe0; // 1110 0000
	P2 = P2 & 0x1f;
	
	P0 = 1 << ucSeg_Pos;
	P2 = P2 & 0x1f | 0xc0;
	P2 = P2 & 0x1f;
}