#ifndef __USART_H
#define __USART_H
#include"stdio.h"
#include"sys.h"


#define  USART_REC_LEN  200    //最大接收字节数
#define  EN_USART_RX     1     //使能1  /禁止 0  串口1接收

extern u8 USART_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;         		//接收状态标记	

void usart_Init(u32 bound);   //bound 为波特率



#endif
