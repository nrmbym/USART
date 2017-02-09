#include "stm32f10x.h"
#include "usart.h"
#include"sys.h"


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)
		;//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

u8 USART_RX_BUF[USART_REC_LEN];    //接收缓冲  ，最大的字节为200   头文件的宏定义
 


u16 USART_RX_STA=0;               //接收状态标记  bit15 接收完成标志  bit14 接收到0X0D标志 （回车换行符  结束标志）  bit13~0  接受到的有效数据个数   



void usart_Init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStrue;   //定义一个结构体变量，用来存放GPIO的模式，速度，pin
	USART_InitTypeDef USART_InitStructure;//
	NVIC_InitTypeDef NVIC_InitStruct;  //
	
	/*****第一步串口、GPIO使能;配置*******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//串口一时钟使能
	

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;   //设置输出模式为推挽输出
	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9;       //
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);        //GPIO端口模式配置
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;//设置
	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);             //串口参数初始化
	
	
	
	/********第二步，初始化串口**************/
	
	USART_InitStructure.USART_BaudRate = 115200;                 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;             //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    	//收发模式
  USART_Init(USART1, &USART_InitStructure);       //初始化串口1


//***********开启中断************//

	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//入口参数  指明哪一个通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=	ENABLE;     //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;  //抢占优先级为1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;       //子优先级   响应优先级
	NVIC_Init(&NVIC_InitStruct);          //中断优先级初始化配置函数  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	
	
	
	/******使能串口*********/
  USART_Cmd(USART1, ENABLE);                  //使能串口1 


	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接受缓冲器非空  使能
	





}


void USART1_IRQHandler(void)//中断服务函数    启动文件  中断配置里
{
	u8 res;
	 if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  //接收中断（以0x0d  0x0a结尾）   才接收
		 {
			 res= USART_ReceiveData(USART1);           //接收读取
			 if((USART_RX_STA&0x8000)==0) //如果接收未完成             
//判断是否接收完成 8000  1000 ....接收完成，15位、14位被置1      
			 {
				 if(USART_RX_STA&0X4000)   // 14位为1 接收到了0x0d
				 {
					 if(res!=0x0a)USART_RX_STA=0;  //14位接收错误
					 else USART_RX_STA |= 0X8000;    //接收完成，15 置1
				 }
				 else //   未收到0x0d
				 {
					 if(res==0x0d) USART_RX_STA|=0X4000;
					 else
					 {
						 USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
						 USART_RX_STA++;
						 if(USART_RX_STA>(USART_REC_LEN-1))  USART_RX_STA=0;
						 
					 }
					 
			 }
		 }
//			 USART_SendData(USART1,res);               //发送数据到串口 
   }
 }







