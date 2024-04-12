#ifndef __TASK_H
#define __TASK_H

#include "main.h"

void initSysTick(void);
void initBsp(void);

void taskDisplay(void);
void taskKey(void);
void taskControl(void);
void taskCollect(void);
void taskrot(void);

#endif