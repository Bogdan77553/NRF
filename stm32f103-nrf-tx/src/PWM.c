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
#include "PWM.h"


void Hard_PWM(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    //Тут настраиваем порт PB0 на выход
    //Подробнее об этом в статье про GPIO
    GPIO_InitTypeDef port;

    port.GPIO_Mode = GPIO_Mode_AF_PP;
    port.GPIO_Pin = GPIO_Pin_13;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &port);
    //А тут настройка таймера
    //Заполняем поля структуры дефолтными значениями
    TIM_TimeBaseInitTypeDef timer;
    //Выставляем предделитель
    timer.TIM_Prescaler = 72;

    timer.TIM_Period = 15000-1;
    //Инициализируем TIM4 нашими значениями
    TIM_TimeBaseInit(TIM1, &timer);


    TIM_OCInitTypeDef timerPWM;
    timerPWM.TIM_Pulse = 1500;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM1, &timerPWM);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM1,ENABLE);

}


void Hard_PWM1()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |  RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    //Тут настраиваем порт PB0 на выход
    //Подробнее об этом в статье про GPIO
    GPIO_InitTypeDef port;
    port.GPIO_Mode = GPIO_Mode_AF_PP;
    port.GPIO_Pin = GPIO_Pin_6;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &port);
    TIM_DeInit(TIM1);
    //А тут настройка таймера
    //Заполняем поля структуры дефолтными значениями
    TIM_TimeBaseInitTypeDef timer;
    //Выставляем предделитель
    timer.TIM_Prescaler = 72;

    timer.TIM_Period = 15000-1;
    //Инициализируем TIM4 нашими значениями
    TIM_TimeBaseInit(TIM3, &timer);


    TIM_OCInitTypeDef timerPWM;
    timerPWM.TIM_Pulse = 1500;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM3, &timerPWM);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_Cmd(TIM3,ENABLE);

}


int qw(void)
{
//SysTick_Config(SystemCoreClock / 1000);
// GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//GPIO_InitTypeDef port;
    // port.GPIO_Mode = GPIO_Mode_Out_PP;
    //  port.GPIO_Pin = GPIO_Pin_6;
    //  port.GPIO_Speed = GPIO_Speed_50MHz;
    //  GPIO_Init(GPIOA, &port);
    Hard_PWM();
    //uint16_t a;
    while(1)
    {
        //for(a=0;a<60000;a++){
        // TIM_SetCompare1(TIM3, a);
        //  delay(1);
        //  a++;
// }
        /*
        GPIO_SetBits(GPIOA,GPIO_Pin_6);

        delay(1000);
        GPIO_ResetBits(GPIOA,GPIO_Pin_6);
        delay(100);
        */
    }
}





void InitPWM(void)
{

    TIM_TimeBaseInitTypeDef  TIM1_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM1_BDTRInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable GPIOA and GPIOB clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // GPIOA Configuration: Channel 1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Enable TIM1 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // TIM1 Peripheral Configuration
    TIM_DeInit(TIM1);

    // Time Base configuration
    TIM1_TimeBaseStructure.TIM_Prescaler = 0;
    TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // TIM1_TimeBaseStructure.TIM1_Period = 0xFFF; // 12 bit
    TIM1_TimeBaseStructure.TIM_Period = 400-1; // 10 bit
    TIM1_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM1_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseStructure);


    // Channel 1 Configuration in PWM mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1,&TIM_OCInitStructure);

    // Automatic Output enable, Break, dead time and lock configuration
    TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
    TIM1_BDTRInitStructure.TIM_DeadTime = 0x00;
    TIM1_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStructure);

    // TIM1 counter enable
    TIM_Cmd(TIM1,ENABLE);

    // Main Output Enable
    TIM_CtrlPWMOutputs(TIM1,ENABLE);

}

void asd ()
{
    /*
    // Time Base configuration
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // Channel 1, 2 and 3 Configuration in PWM mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    // Automatic Output enable, Break, dead time and lock configuration
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 0;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_Cmd(TIM1, ENABLE);


    TIM_CtrlPWMOutputs(TIM1, ENABLE);*/
}


