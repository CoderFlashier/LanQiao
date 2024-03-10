#include "seg.h"
#include "timer.h"
#include "init.h"
#include "led.h"
#include "iic.h"

//Seg
unsigned char pucSeg_Buf[10], pucSeg_Code[8], pucSeg_Pos;

//Timer
unsigned int uiSeg_Dly;
unsigned long ulms = 0;

//EEPROM
unsigned char pucBufWrite[3] = {13, 20};
unsigned char pucBufRead[3] = {0};

void Seg_Proc(void);

void main()
{
	Cls_Peripheral();
	Timer0_Init();
	EA = 1;
	
	//EEPROM_Write(pucBufWrite, 0x00, 2);
	EEPROM_Read(pucBufRead, 0x00, 2);
	
	while(1)
	{
		Seg_Proc();
	}
}


void Seg_Proc(void)
{
	if(uiSeg_Dly < 200)
		return;
	
	uiSeg_Dly = 0;
	
	sprintf(pucSeg_Buf, "%d %d", (unsigned int)pucBufRead[0], (unsigned int)pucBufRead[1]);
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}


void Timer_0(void) interrupt 1
{
	uiSeg_Dly++;
	ulms++;
	if(ulms % 2 == 0)
	{
		pucSeg_Pos = (pucSeg_Pos + 1) % 8;
		Seg_Display(pucSeg_Code, pucSeg_Pos);
	}
}

