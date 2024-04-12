#ifndef __SONIC_H
#define __SONIC_H

#include "stc15f2k60s2.h"
#include "intrins.h"

sbit SONIC_TX = P1^0;
sbit SONIC_RX = P1^1;

unsigned int CalDistance(void);
void InitSonicTimer(void);

#endif