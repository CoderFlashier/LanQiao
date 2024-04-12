#include "init.h"
#include "seg.h"
#include "ultrasonic.h"
#include "timer.h"
#include "key.h"
#include "led.h"
#include "stdio.h"
#include "iic.h"

//Seg
unsigned char Seg_Num[10] = {0};
unsigned char Seg_Code[10] = {0};
//unsigned char Seg_Proc = 0;
unsigned char Seg_Dly = 0;
unsigned char Seg_Site = 0;
void Seg_Proc(void);

//Ultrasonic
unsigned char Far_Val = 0;
unsigned char Far_Val_Last = 0;
unsigned int Ultrasonic_Dly = 0;
unsigned char Ultrasonic_Max = 60;
unsigned char Ultrasonic_Min = 10;
unsigned char Ultrasonic_Mode = 0;
unsigned char Alarm = 0;
unsigned char Ultrasonic_Set_Mode = 3;
unsigned char Ultrasonic_Out = 0;
unsigned char Ultrasonic_Out_Mark = 0;
void Is_Out(void);

//Mode
unsigned char Mode_Mark = 0;
unsigned char Mode_Mark_Last = 3;
void Mode_0(void);
void Mode_1(void);
void Mode_2(void);

//Key
unsigned char Key_Val = 0;
unsigned char Key_Val_Last = 20;

//PCF8591
float V_Val = 0;

//LED
unsigned char LED_Val = 0;
unsigned int LED_Dly = 0;

void main()
{
	init();
	Timer0_Init();
	Timer1_Init();
	EA = 1;
	
	while(1)
	{
		Key_Val = Key_Read_KBD();
		if(Key_Val == 4 && Key_Val_Last == 0)
			Mode_Mark = (Mode_Mark + 1) % 3;
		switch(Mode_Mark)
		{
			case 0:
				Mode_0();
				break;
			case 1:
				Mode_1();
				break;
			case 2:
				Mode_2();
				break;
		}
		Key_Val_Last = Key_Val;
		Seg_Proc();
	}
}


void Mode_0(void)
{
	if(Mode_Mark == 0 &&  Mode_Mark_Last != 0)
	{
		LED_Val &= 0xf8;
		LED_Val |= 0x01;
	}
	
	Mode_Mark_Last = Mode_Mark;
	if(Ultrasonic_Dly < 400)
		return;
	
	Ultrasonic_Dly = 0;
	
	Far_Val = Wave_Recv();
	
	Is_Out();
	
	sprintf(Seg_Num, "A    %3u", (unsigned int)Far_Val);
	//LED_Display(LED_Val);
}


void Mode_1(void)
{
	unsigned char Mode_1_Key_Val = 0;
	unsigned char Mode_1_Key_Val_Last = 20;
	
	if(Mode_Mark == 1 &&  Mode_Mark_Last != 1)
	{
		LED_Val &= 0xf8;
		//1111 1000
		LED_Val |= 0x02;
	}
	
	//sprintf(Seg_Num, "P%u %u-%u", (unsigned int)(Ultrasonic_Mode + 1), (unsigned int)Ultrasonic_Min, (unsigned int)Ultrasonic_Max);
	while(1)
	{
		if(Ultrasonic_Dly > 400)
		{
			Ultrasonic_Dly = 0;
			Far_Val = Wave_Recv();
			Is_Out();
		}
	
		Mode_1_Key_Val = Key_Read_KBD();
		if(Mode_1_Key_Val == 5 && Mode_1_Key_Val_Last == 0)
			Ultrasonic_Mode = (Ultrasonic_Mode + 1) % 2;
		
		if(Ultrasonic_Mode == 0)
		{
			if(Mode_1_Key_Val == 9 && Mode_1_Key_Val_Last == 0)
			{
				Ultrasonic_Max += 10;
				if(Ultrasonic_Max > 90)
					Ultrasonic_Max = 50;
			}
			
			if(Mode_1_Key_Val == 8 && Mode_1_Key_Val_Last == 0)
			{
				Ultrasonic_Min += 10;
				if(Ultrasonic_Min > 40)
					Ultrasonic_Min = 0;
			}
				
		}
		
		if(Ultrasonic_Mode == 1)
		{
			V_Val = (PCF8591_ADC() / 256.0) * 5.0;
			if(Mode_1_Key_Val == 9 && Mode_1_Key_Val_Last == 0)
				Ultrasonic_Set_Mode = 0;
			if(Mode_1_Key_Val == 8 && Mode_1_Key_Val_Last == 0)
				Ultrasonic_Set_Mode = 1;
			
			switch(Ultrasonic_Set_Mode)
			{
				case 0:
					if(V_Val <= 1)
						Ultrasonic_Max = 50;
					else if(V_Val > 1 && V_Val <= 2)
						Ultrasonic_Max = 60;
					else if(V_Val > 2 && V_Val <= 3)
						Ultrasonic_Max = 70;
					else if(V_Val > 3 && V_Val <= 4)
						Ultrasonic_Max = 80;
					else
						Ultrasonic_Max = 90;
					break;
					
				case 1:
					if(V_Val <= 1)
						Ultrasonic_Min = 0;
					else if(V_Val > 1 && V_Val <= 2)
						Ultrasonic_Min = 10;
					else if(V_Val > 2 && V_Val <= 3)
						Ultrasonic_Min = 20;
					else if(V_Val > 3 && V_Val <= 4)
						Ultrasonic_Min = 30;
					else
						Ultrasonic_Min = 40;
					break;
			}
		}
		sprintf(Seg_Num, "P%u %02u-%02u", (unsigned int)(Ultrasonic_Mode + 1), (unsigned int)Ultrasonic_Min, (unsigned int)Ultrasonic_Max);
		//sprintf(Seg_Num, "P%u %f", (unsigned int)(Ultrasonic_Mode + 1), V_Val);
		Seg_Proc();
		
		Mode_Mark_Last = Mode_Mark;
		
		if(Mode_1_Key_Val == 4 && Mode_1_Key_Val_Last == 0)
		{
			Mode_Mark = (Mode_Mark + 1) % 3;
			break;
		}
		Mode_1_Key_Val_Last = Mode_1_Key_Val;
		//LED_Display(LED_Val);
	}
}


void Mode_2(void)
{
	unsigned char Mode_2_Key_Val = 0;
	unsigned char Mode_2_Key_Val_Last = 20;
	
	if(Mode_Mark == 2 &&  Mode_Mark_Last != 2)
	{
		LED_Val &= 0xf8;
		LED_Val |= 0x04;
	}
	
	while(1)
	{
		Mode_2_Key_Val = Key_Read_KBD();
		
		if(Ultrasonic_Dly > 400)
		{
			Ultrasonic_Dly = 0;
			Far_Val = Wave_Recv();
			Is_Out();
		}
		
		if(Ultrasonic_Out < 10)
			sprintf(Seg_Num, "E      %u", (unsigned int)Ultrasonic_Out);
		else
			sprintf(Seg_Num, "E      -");
		
		Mode_Mark_Last = Mode_Mark;
		//LED_Display(LED_Val);
		
		if(Mode_2_Key_Val == 5 && Mode_2_Key_Val_Last == 0)
		{
			Ultrasonic_Out = 0;
		}
		
		if(Mode_2_Key_Val == 4 && Mode_2_Key_Val_Last == 0)
		{
			Mode_Mark = (Mode_Mark + 1) % 3;
			break;
		}
		
		Mode_2_Key_Val_Last = Mode_2_Key_Val;
		Seg_Proc();
	}
}


void Seg_Proc(void)
{
	if(Seg_Dly < 200)
		return;
	
	Seg_Dly = 0;
	
	Seg_Tran(Seg_Num, Seg_Code);
}

void Is_Out(void)
{
	if((Far_Val > Ultrasonic_Max && Far_Val_Last < Ultrasonic_Max) || (Far_Val < Ultrasonic_Min && Far_Val_Last > Ultrasonic_Min))
		Ultrasonic_Out++;
	
	if(Far_Val > Ultrasonic_Max || Far_Val < Ultrasonic_Min)
		Ultrasonic_Out_Mark = 1;
	else
	{
		LED_Val |= 0x80;
		Ultrasonic_Out_Mark = 0;
	}
	
	Far_Val_Last = Far_Val;
}

void Timer_1(void) interrupt 3
{
	Seg_Dly++;
	Ultrasonic_Dly++;
	
	if(Seg_Dly % 2 == 0)
	{
		Seg_Site = (Seg_Site + 1) % 8;
		Seg_Display(Seg_Code, Seg_Site);
	}
	
	if(Ultrasonic_Out_Mark == 1)
		if(LED_Dly % 1000 == 0)
		{
			LED_Dly = 0;
			LED_Val ^= 0x80;
		}
		
	LED_Display(LED_Val);
}