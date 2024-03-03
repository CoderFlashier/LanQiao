#include "STC15F2K60S2.H"
#include "init.h"
#include "seg.h"
#include "key.h"
#include "timer.h"

//Seg
unsigned char pucSeg_Buf[10], pucSeg_Code[8], pucSeg_Pos;

//Key
unsigned char ucKey_Val = 0;

//Timer
unsigned int uiSeg_Dly = 0;
unsigned int uiKey_Dly = 0;
unsigned long ulms = 0;

void Seg_Proc(void);
void Key_Proc(void);

void main()
{
	Cls_Peripheral();
	Timer0_Init();
	EA = 1;
	
	while(1)
	{
		Key_Proc();
		Seg_Proc();
	}
}


void Seg_Proc(void)
{
	if(uiSeg_Dly < 20)
		return;
	
	uiSeg_Dly = 0;
	
	sprintf(pucSeg_Buf, "%d", (int)ucKey_Val);
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}


void Key_Proc(void)
{
	if(uiKey_Dly < 20)
		return;
	
	uiKey_Dly = 0;
	
	ucKey_Val = Key_Read_KBD();
}


void Timer_0(void) interrupt 1
{
	uiSeg_Dly++;
	uiKey_Dly++;
	ulms++;
	if(ulms % 2 == 0)
	{
		pucSeg_Pos = (pucSeg_Pos + 1) % 8;
		Seg_Display(pucSeg_Code, pucSeg_Pos);
	}
}
