#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "LED.h"
 int main(void)
 {
	 
	 
	 u16 t;
	 u16 len;
	 u16 times=0;
	 delay_init();//延时初始化
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位
	 usart_Init(115200);
	 LED_Init();
	 while(1)
	 {
		 if(USART_RX_STA&0X8000)//检测到发送完毕
		 {
			 len=USART_RX_STA&0x3fff;//15 14位归0
			 printf("\r\n您发送的消息：");
			 for(t=0;t<len;t++)
			 {
				 USART_SendData(USART1,USART_RX_BUF[t]);
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET); 
				 
			 }
			 printf("\r\n\r\n");//插入换行
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
				printf("请输入数据，以回车键结束\n");
			if(times%30==0) 
			{
				led1=~led1;
			}
			delay_ms(10);
			delay_ms(200);
			
		}
	 }
 }
