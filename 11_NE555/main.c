#include "seg.h"
#include "timer.h"
#include "init.h"

//Seg
unsigned char pucSeg_Buf[10], pucSeg_Code[8], pucSeg_Pos;

//Timer
unsigned int uiSeg_Dly;
unsigned long ulms = 0;

//NE555
unsigned int freq = 0;

void Seg_Proc(void);

void main()
{
	Cls_Peripheral();
	Timer0_Init();
	Timer1_Init();
	EA = 1;
	
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
	
	sprintf(pucSeg_Buf, "%5u", freq);
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}


void Timer_1(void) interrupt 3
{
	uiSeg_Dly++;
	ulms++;
	if(ulms % 2 == 0)
	{
		pucSeg_Pos = (pucSeg_Pos + 1) % 8;
		Seg_Display(pucSeg_Code, pucSeg_Pos);
	}
	
	if(ulms % 1000 == 0)
	{
		freq = ((TH0 << 8) | TL0);
		TL0 = 0;
		TH0 = 0;
	}
}