#include "timer.h"
#include "led.h"
#include "init.h"
#include "seg.h"
#include "stdio.h"
#include "key.h"
#include "buzz_relay.h"
#include "ds1302.h"
#include "iic.h"

//Timer
unsigned long ulms = 0;
unsigned int uiLED_Dly = 0;
unsigned int uiSeg_Dly = 0;
unsigned int uiKey_Dly = 0;
unsigned int uiPeripheral_Dly = 0;
unsigned int uiRTC_Dly = 0;

//LED
unsigned char ucLED = 0x25;

//Seg
unsigned char ucSeg_Pos = 0;
unsigned char pucSeg_Buf[12], pucSeg_Code[8];

//Key
unsigned char ucKey_Val = 0;

//RTC
unsigned char pucRTC[3] = {23, 59, 55};

//Function
void LED_Proc(void);
void Seg_Proc(void);
void Key_Proc(void);
void Peripheral_Proc(void);
void RTC_Proc(void);


void main(void)
{
	init();
	Timer0_Init();
	EA = 1;
	
	Set_RTC(pucRTC);
	
	while(1)
	{
		LED_Proc();
		Seg_Proc();
		Key_Proc();
		RTC_Proc();
		
		Peripheral_Proc();
	}
}


void LED_Proc(void)
{
	if(uiLED_Dly < 200)
		return;
	
	uiLED_Dly = 0;
	
	LED_Display(ucLED);
}

void Seg_Proc(void)
{
	if(uiSeg_Dly < 200)
		return;
	
	uiSeg_Dly = 0;
	
	sprintf(pucSeg_Buf, "%02u-%02u-%02u", (unsigned int)pucRTC[0], (unsigned int)pucRTC[1], (unsigned int)pucRTC[2]);
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}


void Key_Proc(void)
{
	if(uiKey_Dly > 20)
		return;
	
	uiKey_Dly = 0;
	
	ucKey_Val = Key_Read_KBD();
}


void Peripheral_Proc(void)
{
	if(uiPeripheral_Dly < 500)
		return;
	
	uiPeripheral_Dly = 0;
	
	Set_Peripheral(0, 1);
}
	

void RTC_Proc(void)
{
	if(uiRTC_Dly < 500)
		return;
	
	uiRTC_Dly = 0;
	
	Get_RTC(pucRTC);
}
	

void Timer_0(void) interrupt 1
{
	ulms++;
	uiLED_Dly++;
	uiSeg_Dly++;
	uiKey_Dly++;
	uiPeripheral_Dly++;
	uiRTC_Dly++;
	
	if(ulms % 2 ==0)
	{
		ucSeg_Pos = (ucSeg_Pos + 1) % 8;
		Seg_Display(pucSeg_Code, ucSeg_Pos);
	}
}