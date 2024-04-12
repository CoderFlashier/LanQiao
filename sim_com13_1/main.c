#include "timer.h"
#include "seg.h"
#include "key.h"
#include "stdio.h"
#include "init.h"
#include "onewire.h"
#include "ds1302.h"
#include "buzz_relay.h"
#include "led.h"

//Key
unsigned char Key_Val = 0;
unsigned char Key_Val_Last = 0;

//Seg
unsigned char Seg_Num[12] = {0};
unsigned char Seg_Val[12] = {0};
unsigned char Seg_Pos = 0;

//Timer
unsigned char Seg_Dly = 0;

//DS18B20
float Temperature_Val = 0;
unsigned int Temperature_Dly = 0;
float Temperature_Up = 23.0;

//RCT
unsigned char Time_Val[3] = {12, 59, 50};
unsigned char RTC_Dly = 0;

//Relay
unsigned int Relay_Dly = 0;
unsigned char Relay_Mode = 0;
unsigned char Relay_Mark = 0;
unsigned char Relay_Mode_1_Mark = 0;
unsigned char Relay_Is_Open = 0;

//LED
unsigned char LED_Num = 0x02;
unsigned int LED_Dly = 0;

//Mode
unsigned char Mode_Mark = 0;
unsigned int Mode_Val = 0;
void Mode_1(void);
void Mode_2(void);
void Mode_3(void);

//Func
void Seg_Proc(void);
void Temperature_Read(void);
void RTC_Time_Read(void);
void Is_Realy(void);


void main()
{
	init();
	Timer1_Init();
	Set_RTC(Time_Val);
	EA = 1;
	
	while(1)
	{
		Key_Val = Key_Read_KBD();
		
		if(Key_Val == 12 && Key_Val_Last == 0)			
			Mode_Mark = (Mode_Mark + 1) % 3;
		if(Key_Val == 13 && Key_Val_Last == 0)
			Relay_Mode = (Relay_Mode + 1) % 2;
		Key_Val_Last = Key_Val;
				
		switch (Mode_Mark)
		{
			case 0:
				Mode_1();
				break;
			case 1:
				Mode_2();
				break;
			case 2:
				Mode_3();
				break;
		}
		Seg_Proc();
	}
}

void Mode_1(void)
{
	Temperature_Read();
	sprintf(Seg_Num, "u1   %02.1f", (Temperature_Val / 16.0) + Temperature_Up);
	Is_Realy();
}

void Mode_2(void)
{
	unsigned char Mode_2_Key_Val = 0;
	unsigned char Mode_2_Key_Val_Last = 1;
	
	while(1)
	{
		RTC_Time_Read();
		Mode_2_Key_Val = Key_Read_KBD();
		if(Mode_2_Key_Val == 0 && Mode_2_Key_Val_Last == 0)
			sprintf(Seg_Num, "u2 %02d-%02d", (unsigned int)Time_Val[0], (unsigned int)Time_Val[1]);
		if(Mode_2_Key_Val == 17 && Mode_2_Key_Val_Last == 17)
			sprintf(Seg_Num, "u2 %02d-%02d", (unsigned int)Time_Val[1], (unsigned int)Time_Val[2]);
		
		if(Mode_2_Key_Val == 12 && Mode_2_Key_Val_Last == 0)
			break;
		Seg_Proc();
		
		if(Mode_2_Key_Val == 13 && Mode_2_Key_Val_Last == 0)
			Relay_Mode = (Relay_Mode + 1) % 2;
		
		Mode_2_Key_Val_Last = Mode_2_Key_Val;
		Is_Realy();
	}
	
	Mode_Mark = (Mode_Mark + 1) % 3;
}	

void Mode_3(void)
{
	unsigned char Mode_3_Key_Val = 0;
	unsigned char Mode_3_Key_Val_Last = 12;
	while(1)
	{
		Temperature_Read();
		sprintf(Seg_Num, "u3    %02.0f", (Temperature_Val / 16.0) + Temperature_Up);
		
		Mode_3_Key_Val = Key_Read_KBD();
		if(Mode_3_Key_Val == 16 && Mode_3_Key_Val_Last == 0)
			Temperature_Up += 1.0;
		if(Mode_3_Key_Val == 17 && Mode_3_Key_Val_Last == 0)
			Temperature_Up -= 1.0;
		
		if(Temperature_Up < 10.0)
			Temperature_Up = 99.0;
		if(Temperature_Up > 99.0)
			Temperature_Up = 10.0;
		
		if(Mode_3_Key_Val == 12 && Mode_3_Key_Val_Last == 0)
			break;
		Seg_Proc();
		
		if(Mode_3_Key_Val == 13 && Mode_3_Key_Val_Last == 0)
			Relay_Mode = (Relay_Mode + 1) % 2;
		
		Mode_3_Key_Val_Last = Mode_3_Key_Val;
		Is_Realy();
	}	
	
	Mode_Mark = (Mode_Mark + 1) % 3;
}

void Seg_Proc(void)
{
	if(Seg_Dly < 200)
		return;
	
	Seg_Dly = 0;
	
	Seg_Tran(Seg_Num, Seg_Val);
}


void Temperature_Read(void)
{
	if(Temperature_Dly < 500)
		return;
	
	Temperature_Val = rd_temperature();
}

void RTC_Time_Read(void)
{
	if(RTC_Dly < 200)
		return;
	
	RTC_Dly = 0;
	
	Get_RTC(Time_Val);
}

void Is_Realy(void)
{
	switch(Relay_Mode)
	{
		case 0:
			if(Relay_Mark != 0)
			{
				Set_Peripheral(0, 0);
				LED_Num = 0x02;
			}
			
			if((((Temperature_Val / 16.0) + Temperature_Up) < 10.0) || (((Temperature_Val / 16.0) + Temperature_Up) > 99.0))
			{	
				Set_Peripheral(0, 1);
				Relay_Is_Open = 1;
			}
			else
			{	
				Set_Peripheral(0, 0);
				Relay_Is_Open = 0;
			}
			
			if(Time_Val[1] == 59 && Time_Val[2] == 59)
			{	
				Relay_Mode_1_Mark = 1;
				Relay_Dly = 0;
			}
			
			if(Relay_Dly == 800)
				LED_Num = 0x03;
							
			if(Relay_Dly == 5800)
			{	
				Relay_Mode_1_Mark = 0;
				Relay_Dly = 0;
				LED_Num = 0x02;
			}
			
			Relay_Mark = 0;
			
			break;
			
			
		case 1:
			if(Relay_Mark != 1)
			{
				Set_Peripheral(0, 0);
				LED_Num = 0x00;
			}
		
			if(Time_Val[1] == 59 && Time_Val[2] == 59)
			{
				Relay_Mode_1_Mark = 1;
				Relay_Dly = 0;
			}
			
			if(Relay_Dly == 800)
			{
				LED_Num = 0x01;
				Set_Peripheral(0, 1);
				Relay_Is_Open = 1;
			}
							
			if(Relay_Dly == 5800)
			{
				Set_Peripheral(0, 0);
				Relay_Is_Open = 0;
				LED_Display(0x00);
				Relay_Mode_1_Mark = 0;
				Relay_Dly = 0;
				LED_Num = 0x00;
			}
			
			Relay_Mark = 1;
			break;
	}
	
	if(Relay_Is_Open)
	{
		if(LED_Dly % 100 == 0)
					LED_Num ^= 0x04;
	}
	LED_Display(LED_Num);
}

void Timer1(void) interrupt 3
{
	Seg_Dly++;
	Temperature_Dly++;
	RTC_Dly++;
	LED_Dly++;
	if(Relay_Mode_1_Mark == 1)
		Relay_Dly++;
	
	if(Seg_Dly % 2 == 0)
	{
		Seg_Display(Seg_Val, Seg_Pos);
		Seg_Pos = (Seg_Pos + 1) % 8;
	}
}
