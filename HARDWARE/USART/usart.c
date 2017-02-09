#include "stm32f10x.h"
#include "usart.h"
#include"sys.h"


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)
		;//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

u8 USART_RX_BUF[USART_REC_LEN];    //���ջ���  �������ֽ�Ϊ200   ͷ�ļ��ĺ궨��
 


u16 USART_RX_STA=0;               //����״̬���  bit15 ������ɱ�־  bit14 ���յ�0X0D��־ ���س����з�  ������־��  bit13~0  ���ܵ�����Ч���ݸ���   



void usart_Init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStrue;   //����һ���ṹ��������������GPIO��ģʽ���ٶȣ�pin
	USART_InitTypeDef USART_InitStructure;//
	NVIC_InitTypeDef NVIC_InitStruct;  //
	
	/*****��һ�����ڡ�GPIOʹ��;����*******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//����һʱ��ʹ��
	

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;   //�������ģʽΪ�������
	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9;       //
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);        //GPIO�˿�ģʽ����
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;//����
	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);             //���ڲ�����ʼ��
	
	
	
	/********�ڶ�������ʼ������**************/
	
	USART_InitStructure.USART_BaudRate = 115200;                 //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;             //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);       //��ʼ������1


//***********�����ж�************//

	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//��ڲ���  ָ����һ��ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=	ENABLE;     //ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;  //��ռ���ȼ�Ϊ1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;       //�����ȼ�   ��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);          //�ж����ȼ���ʼ�����ú���  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
	
	
	/******ʹ�ܴ���*********/
  USART_Cmd(USART1, ENABLE);                  //ʹ�ܴ���1 


	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ܻ������ǿ�  ʹ��
	





}


void USART1_IRQHandler(void)//�жϷ�����    �����ļ�  �ж�������
{
	u8 res;
	 if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  //�����жϣ���0x0d  0x0a��β��   �Ž���
		 {
			 res= USART_ReceiveData(USART1);           //���ն�ȡ
			 if((USART_RX_STA&0x8000)==0) //�������δ���             
//�ж��Ƿ������� 8000  1000 ....������ɣ�15λ��14λ����1      
			 {
				 if(USART_RX_STA&0X4000)   // 14λΪ1 ���յ���0x0d
				 {
					 if(res!=0x0a)USART_RX_STA=0;  //14λ���մ���
					 else USART_RX_STA |= 0X8000;    //������ɣ�15 ��1
				 }
				 else //   δ�յ�0x0d
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
//			 USART_SendData(USART1,res);               //�������ݵ����� 
   }
 }







