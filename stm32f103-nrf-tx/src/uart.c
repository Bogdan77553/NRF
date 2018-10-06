#include "stm32f10x_conf.h"
#include "uart.h"

void initUsart1()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    //NVIC_InitTypeDef  NVIC_InitStructure;

//------------------------------------------------------------
//	UART1
//------------------------------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
//Configure GPIO pin
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//	Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//	Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	Configure UART1
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
/*
__enable_irq();
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
*/

//------------------------------------------------------------
}


void send_Uart(USART_TypeDef* USARTx, unsigned char c)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)== RESET){}
		USART_SendData(USARTx, c);
}

void send_comand(USART_TypeDef* USARTx, const char * ss)
{
 uint32_t i=0;
 while(ss[i]){
   send_Uart(USARTx,ss[i]);
  i++;
 }

}

unsigned char getch_Uart(USART_TypeDef* USARTx)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) == RESET){}
	return USART_ReceiveData(USARTx);
}



