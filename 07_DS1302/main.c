#include "seg.h"
#include "timer.h"
#include "init.h"
#include "led.h"
#include "ds1302.h"

//Seg
unsigned char pucSeg_Buf[10], pucSeg_Code[8], pucSeg_Pos;

//RTC
unsigned char pucRTC[3] = {23, 59, 50};

//Timer
unsigned int uiSeg_Dly;
unsigned int uiRTC_Dly;
unsigned long ulms = 0;

void Seg_Proc(void);
void RTC_Proc(void);


void main()
{
	Cls_Peripheral();
	Timer0_Init();
	EA = 1;
	Set_RTC(pucRTC);
	
	while(1)
	{
		Seg_Proc();
		RTC_Proc();
	}
}


void Seg_Proc(void)
{
	if(uiSeg_Dly < 200)
		return;
	
	uiSeg_Dly = 0;
	
	sprintf(pucSeg_Buf, "%02d %02d %02d", (unsigned int)pucRTC[0], (unsigned int)pucRTC[1], (unsigned int)pucRTC[2]);
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}


void RTC_Proc(void)
{
	if(uiSeg_Dly < 200)
		return;
	
	uiRTC_Dly = 0;
	
	Get_RTC(pucRTC);
}


void Timer_0(void) interrupt 1
{
	uiSeg_Dly++;
	uiRTC_Dly++;
	ulms++;
	if(ulms % 2 == 0)
	{
		pucSeg_Pos = (pucSeg_Pos + 1) % 8;
		Seg_Display(pucSeg_Code, pucSeg_Pos);
	}
}

