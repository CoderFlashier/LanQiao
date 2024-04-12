#include "STC15F2K60S2.H"


sbit Tx = P1^0;
sbit Rx = P1^1;


unsigned char Wave_Recv(void);