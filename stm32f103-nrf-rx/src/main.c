#include "stm32f10x_conf.h"
#include "uart.h"
#include "spi.h"
#include "spim.h"
#include "nrf24.h"
#include <string.h>

#define LED_Port        GPIOB
#define LED_Pin         GPIO_Pin_1
#define LED_ON()        GPIO_WriteBit(LED_Port, LED_Pin, Bit_SET)
#define LED_OFF()       GPIO_WriteBit(LED_Port, LED_Pin, Bit_RESET)


#define nRF_CHANNEL     70
#define nRF_isTX_DEMO   0       // Флаг работы демо-проекта в качестве передатчика
#define nRF_PaketLen    32      // Если размер пакета = 0, то используется переменная длина пакета


#define MAX_BUFFER_SIZE 250

char output_buf[MAX_BUFFER_SIZE];

uint8_t Addr[] = {0x01, 0x02, 0x03, 0x04, 0x05};
// Буфер пакета
uint8_t Buff[] = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x11, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x21, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x31, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

/* itoa:  конвертируем n в символы в s */
 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

 /* reverse:  переворачиваем строку s на месте */
 void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
uint8_t Pipe = 0;
uint8_t Len = 0;
uint16_t Counter = 0;
uint16_t TryCounter = 0;
uint16_t Err = 0;

void add_token(char * target, char *token_name, uint16_t value) {
	char ch_value[10];
	strcat(target, token_name);
	itoa(value, ch_value);
	strcat(target,ch_value );
}

void delay(unsigned long ms)// delay 1 ms per count @ Crystal 8.0 MHz and PLL9x or SYSCLK = 72 MHz
{
    volatile unsigned long i,j;
    for (i = 0; i < ms; i++ )
    for (j = 0; j < 5525; j++ );
}

int main(void)
{
    SystemInit();
    initUsart1();

  gpio_PortClockStart(LED_Port);
  gpio_SetGPIOmode_Out(LED_Port, LED_Pin);

  // init nRF24
  nrf24_init(SPI1, nRF_CHANNEL);
  // астройка зэднання, установка адресу
  nrf24_RxPipe_Setup(0, Addr, nRF_PaketLen);  // Если размер пакета = 0, то используется переменная длина пакета

#if (nRF_isTX_DEMO)
  Len = nRF_PaketLen;
  while (1)
  {
    LED_ON();
    int8_t TryNum = nrf24_Send(Addr, Buff, Len);        // Îòïðàâêà ïàêåòà
    if (TryNum >= 0)            // Пакет успешно был отправлен
    {
      LED_OFF();
      Counter++;                // Считаем кол-во отправленных пакетов
      TryCounter += TryNum;     // Считаем общее кол-во попыток отправки
    }
    else
      Err++;                     // Считаем кол-во ошибок при отправке


    add_token(output_buf,"\nPacet-", Counter);
    add_token(output_buf," Povtor-", TryCounter);
    add_token(output_buf," Error-", Err);
    strcat(output_buf, '\n' );
    send_comand(USART1,output_buf);
    memset(output_buf, 0, sizeof(output_buf));

    delay_ms(1000);
  }
#else
  while (1)
  {
    Len = nrf24_Recv(&Pipe, Buff);
    if (Len)
    {

      Counter++;
    }
  }
#endif
}





