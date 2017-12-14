#include "ALL_Includes.h"//���������ͷ�ļ�
#include <stdio.h>

void ALL_Config(void);

char string[20] = {0};
int i = 0;
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���
///*
//struct __FILE 
//{ 
//	int handle; 
//	/* Whatever you require here. If the only file you are using is */ 
//	/* standard output using printf() for debugging, no file handling */ 
//	/* is required. */ 
////};
//
///* FILE is typedef�� d in stdio.h. */ 
//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//void _sys_exit(int x) 
//{
//	x = x; 
//} 
//�ض���fputc���� 

#define Data_SIZE 10 //���ݳ���   9λ���� +  /r/n  -  /n =10λ

char RevBuf[Data_SIZE];      //���ݽ��ջ�����
char temp[Data_SIZE];        //�����ݵ���������
unsigned char flished_flag=0; //���ݽ��շ���Ҫ���־
int data_count=0;    //���ݳ���
int temp_length;    //���ݳ���
int data_flished_count = 0;  //
char data_flished;           //

int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=ch;
  return (ch);
}
//#endif 

void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//����PA10�ɵڶ���������  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = baud;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);                             //ʹ��USART1
	
	/* USART1��NVIC�ж����� */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
				
}

//=============================================================================
//�ļ����ƣ�
//���ܸ�Ҫ��USART1�жϺ���
//����˵������
//�������أ���
//=============================================================================
void USART1_IRQHandler(void)
{
  char c = 0;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    //USART_SendData(USART1,USART_ReceiveData(USART1));
    //printf("receive [%d]\r\n",USART_ReceiveData(USART1));
    c = USART_ReceiveData(USART1);
    if(c != 13){
      RevBuf[data_count] = c;
      data_count++;
    }else{
      temp_length = data_count;
      data_count = 0;
    }
    
    

    while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
  }

}


int main(void)
{	
	ALL_Config();
	unsigned int i = 0;
	
	while(1)
	{
          int str_cur;
			LED_ON();
			delay_ms(1000);
			printf("STM32F030F4P6 USART TEST\r\n");
			LED_OFF();
			delay_ms(5000);
/*
for(str_cur = 0;str_cur != temp_length;str_cur++){
  USART_SendData(USART1,RevBuf[str_cur]);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
  if(str_cur == temp_length-1){
USART_SendData(USART1,'\r');
USART_SendData(USART1,'\n');
while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
  }
}
str_cur = 0;
*/
	}

	
	
}

/************************
�������ܣ��ܳ�ʼ��
�����������
�����������
��    ע����
************************/
void ALL_Config(void)
{
    Delay_Init(48);
		LED_Init();
		USART1_Init(115200);
}






