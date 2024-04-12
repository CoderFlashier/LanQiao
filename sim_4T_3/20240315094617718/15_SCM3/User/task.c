#include "task.h"

unsigned char cntr_key = 0;
unsigned char cntr_dsp = 0;
unsigned char cntr_led = 0;
unsigned char cntr_adc = 0;
unsigned char cntr_dis = 0;

unsigned char ui = 0;
unsigned int adc = 0;
unsigned int distance = 0;
unsigned int temp_distance = 0;
unsigned char up_para = 60;
unsigned char down_para = 10;
unsigned char warn_count = 0;
unsigned char rot = 0;

bit mode = 0;

//1ms中断一次@12MHz
void initSysTick(void)
{
    AUXR |= 0x04;
    T2L = 0x20;
	T2H = 0xD1;
	AUXR |= 0x10;
    IE2 |= 0x04;
	EA = 1;
}

//初始化板级外设
void initBsp(void)
{
	BUZ(0x00);
	LED(0xFF);
	InitDsp();	
	InitKeyStruct();
	InitPCF8591();
	InitSonicTimer();
}

//定时器中断服务函数
void isrSysTick(void)    interrupt 12
{
	GetDisplayStruct()->SegDisplay();

	if(cntr_dis < 200){
		++cntr_dis;
	}
	
	if(cntr_adc < 150){
		++cntr_adc;
	}
	
	if(cntr_led < 100){
		++cntr_led;
	}
	
	if(cntr_dsp < 50){
		++cntr_dsp;
	}
		
	if(cntr_key < 10){
		++cntr_key;
	}

}
void taskCollect(void)
{
	if(cntr_dis == 200){
		cntr_dis = 0;
		temp_distance = distance;
		distance = CalDistance();

		if((temp_distance >= down_para)&&(temp_distance <= up_para)){
			if((distance < down_para)||(distance > up_para)){
				warn_count++;
			}
	
		}
	}

	if(cntr_adc == 150){
		cntr_adc = 0;
		adc = (unsigned int)(GetADC(3) * 500.0 / 255);
	}

	if(cntr_key == 10){
		cntr_key = 0;
		GetKeyStruct()->pFun();
	}
}

//
void taskControl(void)
{
	static unsigned char led = 0xff;
	
	//100ms 刷新
	if(cntr_led < 100)
		return;
	cntr_led = 0;
	
	if(ui == 0){
		led &= ~1;
		led |= 2;
		led |= 4;
	}else if(ui == 1){
		led &= ~2;
		led |= 1;
		led |= 4;
	}else{
		led &= ~4;
		led |= 1;
		led |= 2;
	}
	//L8
	if((distance >= down_para)&&(distance <= up_para)){
		led &= ~0x80;
	}else{
		led ^= 0x80;
	}
	
	LED(led);
}

void taskrot(void)
{
	if(rot == 1){
		if((adc >= 0)&&(adc < 100)){
			down_para = 0;
		}else if((adc >= 100)&&(adc < 200)){
			down_para = 10;
		}else if((adc >= 200)&&(adc < 300)){
			down_para = 20;
		}else if((adc >= 300)&&(adc < 400)){
			down_para = 30;
		}else if((adc >= 400)&&(adc < 500)){
			down_para = 40;
		}
	}else if(rot == 2){
		if((adc >= 0)&&(adc < 100)){
			up_para = 50;
		}else if((adc >= 100)&&(adc < 200)){
			up_para = 60;
		}else if((adc >= 200)&&(adc < 300)){
			up_para = 70;
		}else if((adc >= 300)&&(adc < 400)){
			up_para = 80;
		}else if((adc >= 400)&&(adc < 500)){
			up_para = 90;
		}
	}
}

//
void taskKey(void)
{	
	//按键部分处理
	switch(GetKeyStruct()->value){
		/* S4 */
		case 12:
			/* 切换界面 */
			if(++ui > 2){
				ui = 0;
			}
			GetKeyStruct()->value = 255;
			break;
		/* S5 */
		case 8:
			if(ui == 1){
				mode = ~mode;
			}else if(ui == 2){
				warn_count = 0;
			}

			if(mode == 0){
				rot = 0;
			}
			GetKeyStruct()->value = 255;
			break;
		/* S8 */
		case 13:
			if(ui == 1){
				if(mode == 0){
					down_para += 10;
					if(down_para > 40){
						down_para = 0;
					}
					
				}else{
					rot = 1;	//旋钮模式，调整参数下限
				}
			}
			GetKeyStruct()->value = 255;
			break;
		/* S9 */
		case 9:
			if(ui == 1){
				if(mode == 0){
					up_para += 10;
					if(up_para > 90){
						up_para = 50;
					}
				}else{
					rot = 2;	//旋钮模式，调整参数上限
				}
			}
			GetKeyStruct()->value = 255;
			break;
	}
}

//显示处理
void taskDisplay(void)
{
	DisplayStruct* seg = GetDisplayStruct();
	
	//显示部分处理
	if(cntr_dsp < 50){
		return;
	}	
	cntr_dsp = 0;
	
	if(ui == 0){
		seg->dot = 255;
		seg->buf[0] = 17;
		seg->buf[1] = 10;
		seg->buf[2] = 10;
		seg->buf[3] = 10;
		seg->buf[4] = 10;
		seg->buf[5] = (distance > 99)?(distance/100):(10);;
		seg->buf[6] = (distance > 9)?(distance%100/10):(10);
		seg->buf[7] = distance%10;
	}else if(ui == 1){
		seg->dot = 255;
		seg->buf[0] = 15;
		if(mode == 0){
			seg->buf[1] = 1;
		}else{
			seg->buf[1] = 2;
		}
		seg->buf[2] = 10;
		seg->buf[3] = down_para/10;
		seg->buf[4] = down_para%10;
		seg->buf[5] = 11;
		seg->buf[6] = up_para/10;
		seg->buf[7] = up_para%10;
	}else{
		seg->dot = 255;
		seg->buf[0] = 16;
		seg->buf[1] = 10;
		seg->buf[2] = 10;
		seg->buf[3] = 10;
		seg->buf[4] = 10;
		seg->buf[5] = 10;
		seg->buf[6] = 10;
		if(warn_count > 9){
			seg->buf[7] = 11;
		}else{
			seg->buf[7] = warn_count;
		}	
	}	
}
