#include "timer.h"
#include "iic.h"
#include "init.h"
#include "led.h"
#include "key.h"
#include "seg.h"
#include "ultrasonic.h"
#include "string.h"


//ultrasoinc
unsigned int ultrasonic_dly = 0;
unsigned char dist = 0;
unsigned char ultrasonic_disp[8] = {0};

//seg
unsigned char seg_dly = 0;
unsigned char seg_buf[8] = {0};
unsigned char seg_code[8] = {0};
unsigned char seg_pos = 0;
unsigned char seg_last[8] = {0};

//func
void seg_disp(unsigned char[8]);
void ultrasonic_test(void);


void main()
{
	Cls_Peripheral();
	Timer0_Init();
	Timer1_Init();
	EA = 1;
	
	while(1)
	{
		//ultrasonic_test();
		//sprintf(ultrasonic_disp, "     123", (unsigned int)dist);
		seg_disp("123     ");
	}
}


/*
void ultrasonic_test(void)
{
	if(ultrasonic_dly < 500)
		return;
		
	ultrasonic_dly = 0;
	dist = Wave_Recv();
}
*/

void Timer_1() interrupt 3
{
	seg_dly++;
	ultrasonic_dly++;
}


void seg_disp(unsigned char buf[8])
{
	if(strcmp(seg_last, buf))
		Seg_Tran(buf, seg_code);
	strcpy(seg_last, buf);
	
	while(seg_dly % 2 == 0)
	{
		seg_pos++;
		seg_pos %= 8;
		Seg_Display(seg_code, seg_pos);
	}
}