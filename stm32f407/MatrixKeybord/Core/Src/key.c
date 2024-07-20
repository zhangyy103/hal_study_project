#include "key.h"

static uint8_t regval = 0x07; // 初始值

uint8_t key_scan(void){
		/*置位 0x07->0x0B->0x0D->0x0E->0x07 循环*/
		GPIOA->BSRR |= regval;
		/**/
		uint8_t temp = GPIOA->IDR & 0xF0;
		/*消抖*/
		HAL_Delay(5);
		/*当有按键按下时*/
		if(temp != 0xF0)
		{
			temp = GPIOA->IDR & (0xF0 + regval);
				switch(temp & 0xF0)
					{
					case 0xE0:
						return (int)(1 + (3 - log2(~temp&0x0F))*4);
					case 0xD0:
						return (int)(2 + (3 - log2(~temp&0x0F))*4);
					case 0xB0:
						return (int)(3 + (3 - log2(~temp&0x0F))*4);
					case 0x70:
						return (int)(4 + (3 - log2(~temp&0x0F))*4);
					}	
		}
		GPIOA->BSRR |= regval << 16;/*置零寄存器数据位*/
		
		/*0x07 0x0B 0x0D 0x0E 循环*/
		regval = (~((~regval& 0x0F) >> 1))& 0x0F;
		if((regval & 0x0F) == 0x0F)
			regval = 0x07;
		return 255;
}

uint8_t get_each_key(void){
    uint8_t key = key_scan();
	switch(key){
		case 1: return 1;
		case 2: return 2;
		case 3: return 3;
		case 4: return 254;
		case 5: return 4;
		case 6: return 5;
		case 7: return 6;
		case 8: return 253;
		case 9: return 7;
		case 10: return 8;
		case 11: return 9;
		case 12: return 252;
		case 13: return 251;
		case 14: return 0;
		case 15: return 249;
		case 16: return 248;
	}
	return 255;
}


