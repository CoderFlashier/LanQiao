#include "init.h"
#include "seg.h"
#include "timer.h"
#include "key.h"
#include "stdio.h"
#include "ultrasonic.h"
#include "ds1302.h"
#include "iic.h"
#include "onewire.h"


//Key
unsigned char key_val = 0;

//Seg
unsigned char seg_val[9] = {0};
unsigned char seg_code[8] = {0};
void Seg_Set(void);
unsigned char seg_dly = 0;
unsigned char seg_place = 0;

//Timer
unsigned long time_st = 0;

//Ultrasonic
unsigned char dist = 0;
void Dist_Test(void);
unsigned int dist_dly = 0;

//DS1302
unsigned char ds1302_val[3] = {22, 59, 55};

//PCF8591
unsigned char PCF8591_dly = 0;
void PCF5891_Test(void);

//NE555
unsigned int ne555 = 0;
void NE555_Test(void);
unsigned int ne555_dly = 0;

//DS18B20
float temperature = 0;
unsigned int temperature_dly = 0;
void Temp_Proc(void);

void main(void)
{
	Init();
	Timer0_Init();
	Timer1_Init();
	EA = 1;
	Set_DS1302(ds1302_val);
	
	while(1)
	{
		Temp_Proc();
	}
}

void PCF5891_Test(void)
{
	unsigned char temp1 = 0;
	unsigned char temp2 = 0;
	
	if(PCF8591_dly < 200)
		return;
	
	PCF8591_dly = 0;
	
	temp1 = PCF5891_ADC(0x41);
	temp2 = PCF5891_ADC(0x43);
	
	sprintf(seg_val, "%03u %03u", (unsigned int)temp2, (unsigned int)temp1);
	Seg_Set();
}

void NE555_Test(void)
{
	if(ne555_dly < 500)
		return;
	ne555_dly = 0;
	
	sprintf(seg_val, "%u", ne555);
	Seg_Set();
}

void Temp_Proc(void)
{
	if(temperature_dly < 500)
		return;
	
	temperature_dly = 0;
	
	temperature = Read_Temperature() / 16.0;
	sprintf(seg_val, "%.2f", temperature);
	Seg_Set();
}

void Seg_Set(void)
{
	if(seg_dly < 200)
		return;
	
	seg_dly = 0;
	Seg_Tran(seg_val, seg_code);
}

void Dist_Test(void)
{
	if(dist_dly < 500)
			return;
	
	dist_dly = 0;
	dist = Ultrasonic_Test();
	sprintf(seg_val, "%u", (unsigned int)dist);
}



void Timer_1(void) interrupt 3
{
	time_st++;
	seg_dly++;
	dist_dly++;
	PCF8591_dly++;
	ne555_dly++;
	temperature_dly++;
	
	if(time_st % 2 == 0)
	{
		seg_place = (seg_place + 1) % 8;
		Seg_Display(seg_code, seg_place);
	}
	
	if(time_st % 1000 == 0)
	{
		ne555 = ((TH0 << 8) | TL0);
		TH0 = 0;
		TL0 = 0;
	}
}