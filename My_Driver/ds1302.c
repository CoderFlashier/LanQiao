/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/								

//
#include "ds1302.h"

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}



void Set_DS1302(unsigned char ds1302_val[])
{
	unsigned char temp = 0;
	Write_Ds1302_Byte(0x8e, 0x00);
	
	temp = ((ds1302_val[0] / 10) << 4) | (ds1302_val[0] % 10);
	Write_Ds1302_Byte(0x84, temp);
	
	temp = ((ds1302_val[1] / 10) << 4) | (ds1302_val[1] % 10);
	Write_Ds1302_Byte(0x82, temp);
	
	temp = ((ds1302_val[2] / 10) << 4) | (ds1302_val[2] % 10);
	Write_Ds1302_Byte(0x80, temp);
	
	Write_Ds1302_Byte(0x8e, 0x80);
}


void Read_DS1302(unsigned char ds1302_val[])
{
	unsigned char temp = 0;
	temp = Read_Ds1302_Byte(0x85);
	ds1302_val[0] = ((temp >> 4) * 10) + (temp & 0x0f);
	
	temp = Read_Ds1302_Byte(0x83);
	ds1302_val[1] = ((temp >> 4) * 10) + (temp & 0x0f);
	
	temp = Read_Ds1302_Byte(0x81);
	ds1302_val[2] = ((temp >> 4) * 10) + (temp & 0x0f);
}
