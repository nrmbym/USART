#ifndef __USART_H
#define __USART_H
#include"stdio.h"
#include"sys.h"


#define  USART_REC_LEN  200    //�������ֽ���
#define  EN_USART_RX     1     //ʹ��1  /��ֹ 0  ����1����

extern u8 USART_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;         		//����״̬���	

void usart_Init(u32 bound);   //bound Ϊ������



#endif
