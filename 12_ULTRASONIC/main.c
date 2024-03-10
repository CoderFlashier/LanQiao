#include "seg.h"
#include "timer.h"
#include "init.h"
#include "led.h"
#include "ultrasonic.h"

//Seg
unsigned char pucSeg_Buf[10], pucSeg_Code[8], pucSeg_Pos;

//Ultrasonic
unsigned char ucDist = 0;

//Timer
unsigned int uiSeg_Dly;
unsigned long ulms = 0;
unsigned int uiUltrasonic_Dly = 0;

void Seg_Proc(void);
void Ultrasonic_Proc(void);


void main()
{
	Cls_Peripheral();
	Timer0_Init();
	Timer1_Init();
	EA = 1;
	
	while(1)
	{
		Seg_Proc();
		Ultrasonic_Proc();
	}
}


void Seg_Proc(void)
{
	if(uiSeg_Dly < 200)
		return;
	
	uiSeg_Dly = 0;
	
	sprintf(pucSeg_Buf, "%-3u", (unsigned int)ucDist);
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}


void Ultrasonic_Proc(void)
{
	if(uiUltrasonic_Dly < 500)
		return;
	
	uiUltrasonic_Dly = 0;
	ucDist = Wave_Recv();
}

void Timer_1(void) interrupt 3
{
	uiSeg_Dly++;
	ulms++;
	uiUltrasonic_Dly++;
	if(ulms % 2 == 0)
	{
		pucSeg_Pos = (pucSeg_Pos + 1) % 8;
		Seg_Display(pucSeg_Code, pucSeg_Pos);
	}
}

