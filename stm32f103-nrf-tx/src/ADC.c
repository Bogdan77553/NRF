/**
 * @author Parfeniuk Bogdan
 *
 * @section LICENSE
 *
 *2016 ARM
 *
 */

/**
 * Includes
 */
#include "ADC.h"

void init_adc(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1| RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);// включаем порта А

GPIO_InitTypeDef gpio_init;
gpio_init.GPIO_Mode = GPIO_Mode_AIN;
gpio_init.GPIO_Pin = GPIO_Pin_0;
gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &gpio_init);


gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
gpio_init.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOE, &gpio_init);
GPIO_SetBits(GPIOE,GPIO_Pin_2);
GPIO_SetBits(GPIOE,GPIO_Pin_3);

 //clock for ADC (max 14MHz --> 72/6=12MHz)
RCC_ADCCLKConfig (RCC_PCLK2_Div2);

ADC_InitTypeDef myadc;
myadc.ADC_Mode=ADC_Mode_Independent;
myadc.ADC_ScanConvMode=DISABLE;
myadc.ADC_ContinuousConvMode=DISABLE;
myadc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
myadc.ADC_DataAlign=ADC_DataAlign_Right;
myadc.ADC_NbrOfChannel=1;
ADC_Init(ADC1,&myadc);


ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1,ADC_SampleTime_1Cycles5); // define regular conversion config
ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
ADC_Cmd(ADC1,ENABLE);
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //0
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //1
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      NVIC_EnableIRQ(ADC1_2_IRQn);

}
