#include "STC15F2K60S2.H"
#include "intrins.h"

sbit sda = P2^1;
sbit scl = P2^0;


unsigned char PCF5891_ADC(unsigned char add);

void PCF8591_DAC(unsigned char dat);