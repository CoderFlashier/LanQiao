#include "main.h"

//���������
void main(void)
{
    initSysTick();
	initBsp();
	
    while(1){
		taskDisplay();
		taskKey();
		taskControl();
		taskCollect();
		taskrot();
    }
}