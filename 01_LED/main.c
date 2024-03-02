#include "led.h"
#include "init.h"

void main(void)
{
	Cls_Peripheral();
	LED_Display(0x0f);
	while(1)
	{
		;
	}
}