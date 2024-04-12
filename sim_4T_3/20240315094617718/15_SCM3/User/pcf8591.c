#include "pcf8591.h"

void Delay(void)          //10ms
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

//
void InitPCF8591(void)
{
	IIC_Start();
	IIC_SendByte(0x90);
    IIC_WaitAck();
	IIC_SendByte(0x43);         
	IIC_WaitAck();
	IIC_Stop();
	Delay();
}

/**/
unsigned char GetADC(unsigned char channel)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0x90);
    IIC_WaitAck();
	IIC_SendByte(0x40 | channel);  //
	IIC_WaitAck();
    
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	
	return temp;
}
