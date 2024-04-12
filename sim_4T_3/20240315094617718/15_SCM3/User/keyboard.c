#include "keyboard.h"

KEYStruct key;

KEYStruct* GetKeyStruct(void)
{
    return &key;
}

//
static unsigned char ReadKeyPort(void)
{
	unsigned char port = 0;

	port = (P3 & 0x3F);

	(P42)?(port |= 0x40):(port &= 0xBF);
	(P44)?(port |= 0x80):(port &= 0x7F);

	return port;
}

//
static void ClsC(void)
{
	P3 = 0x0F;
	P42 = 0;
	P44 = 0;
}

//
static void ClsR(void)
{
	P3 = 0xF0;
	P42 = 1;
	P44 = 1;		
}

//
void ScanKeyboard(void)
{
	static unsigned char col = 0;	
    switch(key.status)
    {
        case 0: 
			ClsR();
			if(ReadKeyPort() != 0xF0){
				key.status = 1;
			}
            break;
        case 1:
			ClsR();
			switch(ReadKeyPort()){
				case 0xE0:
					col = 4;
					key.status = 2;
					break;
				case 0xD0:
					col = 3;
					key.status = 2;
					break;
				case 0xB0:
					col = 2;
					key.status = 2;
					break;
				case 0x70:
					col = 1;
					key.status = 2;
					break;
				default:
					key.status = 0;	//抖动
					break;
			}
			break;
        case 2:
			ClsC();
			switch(ReadKeyPort()){
				case 0x0E:
					key.value = col - 1;
					key.status = 3;
					break;
				case 0x0D:
					key.value = col + 3;
					key.status = 3;
					break;
				case 0x0B:
					key.value = col + 7;
					key.status = 3;
					break;
				case 0x07:
					key.value = col + 11;
					key.status = 3;
					break;
				default:
					key.status = 0;	//抖动
					break;
			}

			if(key.value > 15) key.value = 255;
            break; 
		case 3:
			ClsC();
			if(ReadKeyPort() == 0x0F){
				key.status = 0;
			}
			break; 
    }
}

void InitKeyStruct(void)
{
    key.pFun = ScanKeyboard;
    key.value = 0xFF;
    key.status = 0;
}

