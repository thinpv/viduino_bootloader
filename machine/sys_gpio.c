#include "sys_gpio.h"
#include "sys_io.h"
#include "reg_ccu.h"



/*
GPIO ��ʼ��
*/
void GPIO_Congif
	(
		GPIO_TypeDef * GPIOx, //GPIO
		unsigned int GPIO_Pin, //Pin?
		GPIOMode_TypeDef GPIOMode,//??????
		GPIOPuPd_TypeDef GPIO_PuPd//???
	)
{ 	
	GPIOx->CFG[GPIO_Pin/8] &=~  (0xF<<(GPIO_Pin%8*4));//
	
	if(GPIOMode==GPIO_Mode_IN)       GPIOx->CFG[GPIO_Pin/8] &= ~ (0x1<<(GPIO_Pin%8*4));//	
	else if(GPIOMode==GPIO_Mode_OUT) GPIOx->CFG[GPIO_Pin/8] |=   (0x1<<(GPIO_Pin%8*4));//
  else                             GPIOx->CFG[GPIO_Pin/8] |=   (GPIOMode<<(GPIO_Pin%8*4));//
	//������
	GPIOx->PUL[GPIO_Pin/16]&= ~ (0x3<<(GPIO_Pin%16*2));//	��0
	GPIOx->PUL[GPIO_Pin/16]|=  (GPIO_PuPd<<(GPIO_Pin%16*2));//	д��
}

/*
GPIO���1
*/
void GPIO_SET
	(
		GPIO_TypeDef * GPIOx, //GPIO
		unsigned int GPIO_Pin //Pin?
	)
{ 
	GPIOx->DAT |=   (0x1<<GPIO_Pin);
}
/*
GPIO���0
*/
void GPIO_RESET
	(
		GPIO_TypeDef * GPIOx, //GPIO
		unsigned int GPIO_Pin //Pin?
	)
{ 
	GPIOx->DAT &= ~ (0x1<<GPIO_Pin);
}
/*
GPIO�� ����0-1
*/
u8 GPIO_READ 
	(
		GPIO_TypeDef * GPIOx, //GPIO
		unsigned int GPIO_Pin //Pin?
	)
{ 
	if(GPIOx->DAT&((1)<<GPIO_Pin))return 1;
	else return 0;
}


