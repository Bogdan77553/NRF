#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "font.h"
#include "LCD.h"
#include "ADC.h"
#include "PWM.h"
#include "encoder.h"
#include <math.h>

#include "uart.h"
#include "spim.h"
#include "nrf24.h"
#include <string.h>

#define VREF (float)0.005999
#define MinTemper (float)1.75
#define MaxTemper (float)1.31
#define ChastotaGistogramu 2  //(ms); 1000ms==1s
#define ChasotaVumir 80 //(ms);  1000ms==1s


    uint16_t enco1 =0;
    uint16_t enco2 =0;
    float AdcAll = 0;
    float AdcFirstP = 0;
    float AdcSecondP = 0;
    uint16_t AdcFirstD = 0;
    uint16_t BuffAdcFirstD = 0;
    uint16_t AdcSecondD = 0;
    uint16_t BuffAdcSecondD = 0;

void initDAC(void){

     /* Включаем порт А */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* Включаем ЦАП */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* Включаем таймер 6 */
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

  /* Настраиваем ногу ЦАПа */
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  /* Включить DAC1 */
  DAC->CR |= DAC_CR_EN2;

}

void initPeriph()
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
        | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    // È òàêòèðîâàíèå FSMC
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    // Èíèöèàëèçàöèÿ ïèíîâ, çàäåéñòîâàííûõ â îáùåíèè ïî FSMC
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5
                    | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;

    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
                    | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
                    | GPIO_Pin_15;

    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_13;
    GPIO_Init(GPIOD, &gpio);

    // Çäåñü ó íàñ Reset
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOE, &gpio);

    // CS
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOD, &gpio);

    // RS
    gpio.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOD, &gpio);

    // CS -> 1
    // Reset -> 0
    // RD -> 1
    // RW -> 1
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    GPIO_SetBits(GPIOD, GPIO_Pin_7);
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    GPIO_SetBits(GPIOD, GPIO_Pin_4);
    GPIO_SetBits(GPIOD, GPIO_Pin_5);
}

void initFSMC()
{
    FSMC_NORSRAMInitTypeDef fsmc;
    FSMC_NORSRAMTimingInitTypeDef fsmcTiming;
    fsmcTiming.FSMC_AddressSetupTime = 0x00;
    fsmcTiming.FSMC_AddressHoldTime = 0x00;
    fsmcTiming.FSMC_DataSetupTime = 0x01;
    fsmcTiming.FSMC_BusTurnAroundDuration = 0x00;
    fsmcTiming.FSMC_CLKDivision = 0x00;
    fsmcTiming.FSMC_DataLatency = 0x00;
    fsmcTiming.FSMC_AccessMode = FSMC_AccessMode_B;

    fsmc.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    fsmc.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    fsmc.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    fsmc.FSMC_MemoryType = FSMC_MemoryType_NOR;
    fsmc.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    fsmc.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    fsmc.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    fsmc.FSMC_WrapMode = FSMC_WrapMode_Disable;
    fsmc.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    fsmc.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    fsmc.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    fsmc.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    fsmc.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    fsmc.FSMC_ReadWriteTimingStruct = &fsmcTiming;
    fsmc.FSMC_WriteTimingStruct = &fsmcTiming;

    FSMC_NORSRAMInit(&fsmc);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

float seredn_ADC(uint16_t k)
{
    float ADC_ser=0;
    uint16_t kk=k;
    uint32_t sumADC=0;
    while(k>0)
    {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        sumADC=sumADC+adc_res;
        k--;
    }
    sumADC=sumADC/kk;
    ADC_ser=sumADC*VREF;

    return ADC_ser;
}

void showLow(){
if(AdcSecondD!=BuffAdcSecondD)
        {
            uint16_t bbb = AdcSecondD%10;
            PrintSevenSegNumFont(bbb, 256, 20,2, YELLOW0);
            bbb = (AdcSecondD - bbb)/10;
            PrintSevenSegNumFont(bbb, 192, 20,2, YELLOW0);



            BuffAdcSecondD = AdcSecondD;
            /* drawFloat(BuffAdcSecondD,4,20,0,4,BLUE0);

             drawFloat(AdcSecondD,4,20,0,4,YELLOW0);*/


        }
}

void showHigh(){
if(AdcFirstD!=BuffAdcFirstD)
        {
            uint16_t bb = AdcFirstD%10;
            PrintSevenSegNumFont(bb, 98, 20,3, RED);

            bb = (AdcFirstD - bb)/10;
             PrintSevenSegNumFont(bb, 0, 20,3, RED);
            BuffAdcFirstD = AdcFirstD;

            // drawFloat(adc1,4,20,0,4,BLUE0);
            //
            // drawFloat(adc1,4,20,0,4,YELLOW0);


        }


}

void SetEncodValue()
{
    AdcAll = seredn_ADC(1850);
    AdcFirstD = AdcAll;
    AdcSecondD = (AdcAll-AdcFirstD)*100;



    enco2=TIM_GetCounter(TIM3);
    TIM_SetCompare1(TIM1,(enco2));

    if(enco1!=enco2)
    {
        //drawNumber(enco1,100,100,4,BLUE0);
        enco1=enco2;
        DAC->DHR12R2=enco2*10;
        //drawNumber(enco1,100,100,4,WHITE);
    }

}

#define LED_Port        GPIOD
#define LED_Pin         GPIO_Pin_3
#define LED_ON()        GPIO_WriteBit(LED_Port, LED_Pin, Bit_SET)
#define LED_OFF()       GPIO_WriteBit(LED_Port, LED_Pin, Bit_RESET)

#define LED_Port_Blink       GPIOD
#define LED_Pin_Blink        GPIO_Pin_6
#define LED_ON_Blink()       GPIO_WriteBit(LED_Port_Blink, LED_Pin_Blink, Bit_SET)
#define LED_OFF_Blink()      GPIO_WriteBit(LED_Port_Blink, LED_Pin_Blink, Bit_RESET)

#define nRF_CHANNEL     70
#define nRF_isTX_DEMO   1       // Ð¤Ð»Ð°Ð³ Ñ€Ð°Ð±Ð¾Ñ‚Ñ‹ Ð´ÐµÐ¼Ð¾-Ð¿Ñ€Ð¾ÐµÐºÑ‚Ð° Ð² ÐºÐ°Ñ‡ÐµÑÑ‚Ð²Ðµ Ð¿ÐµÑ€ÐµÐ´Ð°Ñ‚Ñ‡Ð¸ÐºÐ°
#define nRF_PaketLen    32      // Ð•ÑÐ»Ð¸ Ñ€Ð°Ð·Ð¼ÐµÑ€ Ð¿Ð°ÐºÐµÑ‚Ð° = 0, Ñ‚Ð¾ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÐµÑ‚ÑÑ Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ð°Ñ Ð´Ð»Ð¸Ð½Ð° Ð¿Ð°ÐºÐµÑ‚Ð°

#define MAX_BUFFER_SIZE 250

char output_buf[MAX_BUFFER_SIZE];

uint8_t Addr[] = {0x01, 0x02, 0x03, 0x04, 0x05};
// Ð‘ÑƒÑ„ÐµÑ€ Ð¿Ð°ÐºÐµÑ‚Ð°
uint8_t Buff[] = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x11, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x21, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x31, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

/* itoa:  ÐºÐ¾Ð½Ð²ÐµÑ€Ñ‚Ð¸Ñ€ÑƒÐµÐ¼ n Ð² ÑÐ¸Ð¼Ð²Ð¾Ð»Ñ‹ Ð² s */
 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÐµÐ¼ Ð·Ð½Ð°Ðº */
         n = -n;          /* Ð´ÐµÐ»Ð°ÐµÐ¼ n Ð¿Ð¾Ð»Ð¾Ð¶Ð¸Ñ‚ÐµÐ»ÑŒÐ½Ñ‹Ð¼ Ñ‡Ð¸ÑÐ»Ð¾Ð¼ */
     i = 0;
     do {       /* Ð³ÐµÐ½ÐµÑ€Ð¸Ñ€ÑƒÐµÐ¼ Ñ†Ð¸Ñ„Ñ€Ñ‹ Ð² Ð¾Ð±Ñ€Ð°Ñ‚Ð½Ð¾Ð¼ Ð¿Ð¾Ñ€ÑÐ´ÐºÐµ */
         s[i++] = n % 10 + '0';   /* Ð±ÐµÑ€ÐµÐ¼ ÑÐ»ÐµÐ´ÑƒÑŽÑ‰ÑƒÑŽ Ñ†Ð¸Ñ„Ñ€Ñƒ */
     } while ((n /= 10) > 0);     /* ÑƒÐ´Ð°Ð»ÑÐµÐ¼ */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

 /* reverse:  Ð¿ÐµÑ€ÐµÐ²Ð¾Ñ€Ð°Ñ‡Ð¸Ð²Ð°ÐµÐ¼ ÑÑ‚Ñ€Ð¾ÐºÑƒ s Ð½Ð° Ð¼ÐµÑÑ‚Ðµ */
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

int main(void)
{
    SystemInit();
    initUsart1();
    gpio_PortClockStart(LED_Port);
    gpio_SetGPIOmode_Out(LED_Port, LED_Pin);
    gpio_PortClockStart(LED_Port_Blink);
    gpio_SetGPIOmode_Out(LED_Port_Blink, LED_Pin_Blink);
    LED_ON_Blink();
    initPeriph();
    initFSMC();
    initLCD();
    fillRectangle(0,0,320,240,BLUE0);
    delay(1000);
    fillRectangle(0,0,320,240,BLACK);
    drawString("Test NRF24L01+",5,5,2,WHITE);
    drawString("Board: stm32F103Vet6",10,40,1,WHITE);
    drawString("Speed: 72MHz",10,50,1,WHITE);

 // init nRF24
  nrf24_init(SPI2, nRF_CHANNEL);
  // Ð°ÑÑ‚Ñ€Ð¾Ð¹ÐºÐ° Ð·ÑÐ´Ð½Ð°Ð½Ð½Ñ, ÑƒÑÑ‚Ð°Ð½Ð¾Ð²ÐºÐ° Ð°Ð´Ñ€ÐµÑÑƒ
  nrf24_RxPipe_Setup(0, Addr, nRF_PaketLen);  // Ð•ÑÐ»Ð¸ Ñ€Ð°Ð·Ð¼ÐµÑ€ Ð¿Ð°ÐºÐµÑ‚Ð° = 0, Ñ‚Ð¾ Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÐµÑ‚ÑÑ Ð¿ÐµÑ€ÐµÐ¼ÐµÐ½Ð½Ð°Ñ Ð´Ð»Ð¸Ð½Ð° Ð¿Ð°ÐºÐµÑ‚Ð°

#if (nRF_isTX_DEMO)
  Len = nRF_PaketLen;
  while (1)
  {
    LED_ON();

    int8_t TryNum = nrf24_Send(Addr, Buff, Len);        // ÃŽÃ²Ã¯Ã°Ã Ã¢ÃªÃ  Ã¯Ã ÃªÃ¥Ã²Ã 
    if (TryNum >= 0)            // ÐŸÐ°ÐºÐµÑ‚ ÑƒÑÐ¿ÐµÑˆÐ½Ð¾ Ð±Ñ‹Ð» Ð¾Ñ‚Ð¿Ñ€Ð°Ð²Ð»ÐµÐ½
    {
      LED_OFF();
      Counter++;                // Ð¡Ñ‡Ð¸Ñ‚Ð°ÐµÐ¼ ÐºÐ¾Ð»-Ð²Ð¾ Ð¾Ñ‚Ð¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð½Ñ‹Ñ… Ð¿Ð°ÐºÐµÑ‚Ð¾Ð²
      TryCounter += TryNum;     // Ð¡Ñ‡Ð¸Ñ‚Ð°ÐµÐ¼ Ð¾Ð±Ñ‰ÐµÐµ ÐºÐ¾Ð»-Ð²Ð¾ Ð¿Ð¾Ð¿Ñ‹Ñ‚Ð¾Ðº Ð¾Ñ‚Ð¿Ñ€Ð°Ð²ÐºÐ¸
    }
    else
      Err++;                     // Ð¡Ñ‡Ð¸Ñ‚Ð°ÐµÐ¼ ÐºÐ¾Ð»-Ð²Ð¾ Ð¾ÑˆÐ¸Ð±Ð¾Ðº Ð¿Ñ€Ð¸ Ð¾Ñ‚Ð¿Ñ€Ð°Ð²ÐºÐµ


    add_token(output_buf,"\nPacet-", Counter);
    add_token(output_buf," Povtor-", TryCounter);
    add_token(output_buf," Error-", Err);
    strcat(output_buf, '\n' );


    drawString(output_buf,10,60,1,WHITE);

    //send_comand(USART1,output_buf);


    LED_ON_Blink();
    delay_ms(500);
    LED_OFF_Blink();
    delay_ms(500);
    drawString(output_buf,10,60,1,BLACK);
    memset(output_buf, 0, sizeof(output_buf));
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

void ADC1_2_IRQHandler()
{
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC))
    {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
        adc_res = ADC_GetConversionValue(ADC1);
    }
}

