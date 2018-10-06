#ifndef __UART_H
#define __UART_H

void initUsart1();
void send_comand(USART_TypeDef* USARTx, const char * ss);

#endif /* __UART_H */
