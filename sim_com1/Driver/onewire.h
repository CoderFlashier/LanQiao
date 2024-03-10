#include <STC15F2K60S2.H>

sbit DQ = P1^4;

unsigned int rd_temperature(void);