#include"LED.h"
#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//GPIOB使能
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//设置推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;   //设置IO口号
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//设置频率
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//设置为高
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	
}
