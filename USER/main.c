#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "LED.h"
 int main(void)
 {
	 
	 
	 u16 t;
	 u16 len;
	 u16 times=0;
	 delay_init();//��ʱ��ʼ��
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ
	 usart_Init(115200);
	 LED_Init();
	 while(1)
	 {
		 if(USART_RX_STA&0X8000)//��⵽�������
		 {
			 len=USART_RX_STA&0x3fff;//15 14λ��0
			 printf("\r\n�����͵���Ϣ��");
			 for(t=0;t<len;t++)
			 {
				 USART_SendData(USART1,USART_RX_BUF[t]);
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET); 
				 
			 }
			 printf("\r\n\r\n");//���뻻��
			 USART_RX_STA=0;
		 }
		else
		{
			times++;
			if(times%500==0)
			{
				printf("\rHAHAHHAHAHH\n");
				
			}
			if(times%200==0)
				printf("���������ݣ��Իس�������\n");
			if(times%30==0) 
			{
				led1=~led1;
			}
			delay_ms(10);
			delay_ms(200);
			
		}
	 }
 }
