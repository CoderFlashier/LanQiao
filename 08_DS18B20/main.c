#include "seg.h"
#include "timer.h"
#include "init.h"
#include "onewire.h"


//Seg
unsigned char pucSeg_Buf[10], pucSeg_Code[8], pucSeg_Pos;

//Timer
unsigned int uiSeg_Dly;
unsigned long ulms = 0;

//Temper
unsigned int ui_temp = 0;

void Seg_Proc(void);

void main()
{
	Cls_Peripheral();
	Timer0_Init();
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
	
	ui_temp = rd_temperature();
	sprintf(pucSeg_Buf, "%.2f", ui_temp / 16.0);
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

