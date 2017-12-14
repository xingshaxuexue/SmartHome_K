#include "ALL_Includes.h"//包含所需的头文件
#include <stdio.h>

void ALL_Config(void);

char string[20] = {0};
int i = 0;
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////标准库需要的支持函数
///*
//struct __FILE 
//{ 
//	int handle; 
//	/* Whatever you require here. If the only file you are using is */ 
//	/* standard output using printf() for debugging, no file handling */ 
//	/* is required. */ 
////};
//
///* FILE is typedef’ d in stdio.h. */ 
//FILE __stdout;       
////定义_sys_exit()以避免使用半主机模式    
//void _sys_exit(int x) 
//{
//	x = x; 
//} 
//重定义fputc函数 

#define Data_SIZE 10 //数据长度   9位数据 +  /r/n  -  /n =10位

char RevBuf[Data_SIZE];      //数据接收缓冲区
char temp[Data_SIZE];        //防数据抵消缓冲区
unsigned char flished_flag=0; //数据接收符合要求标志
int data_count=0;    //数据长度
int temp_length;    //数据长度
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
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = baud;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
	USART_Cmd(USART1, ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
				
}

//=============================================================================
//文件名称：
//功能概要：USART1中断函数
//参数说明：无
//函数返回：无
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
函数功能：总初始化
输入参数：无
输出参数：无
备    注：无
************************/
void ALL_Config(void)
{
    Delay_Init(48);
		LED_Init();
		USART1_Init(115200);
}






