#include"LED.h"
#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//GPIOBʹ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;   //����IO�ں�
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//����Ƶ��
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);//����Ϊ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	
}
