#include "STC15F2K60S2.h"
#include "intrins.h"


sbit sda = P2^1;
sbit scl = P2^0;


unsigned char PCF8591_ADC(void);

void PCF8591_DAC(unsigned char dat);