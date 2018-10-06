#include "encoder.h"

#define Read_Enc_A			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)
#define Read_Enc_B			GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)
//#define FORWARD     0
//#define BACKWARD    1

//#define NOREADY     0
//#define READY       1
//#define INIT        3

//volatile uint8_t encoder_status = INIT;
//volatile uint8_t encoder_direction = FORWARD;


#define	ENC_MAX					5

uint16_t Enc_counter=0;
uint16_t Enc_Mode=0;
//----------------------------------------------------
void encoder_ini(void)
{
	GPIO_InitTypeDef GPIO_Enc_Ini;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_Enc_Ini.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
	GPIO_Enc_Ini.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Enc_Ini.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_Enc_Ini);
}


//----------------------------------------------------
uint16_t Enc_A_count=0;
uint16_t Enc_A_state=0;

uint16_t Enc_B_count=0;
uint16_t Enc_B_state=0;

void Systick_Enc(void)
{
	if (Read_Enc_A == 1)
	{
		if (Enc_A_count < ENC_MAX)
		{
			Enc_A_count++;
		} else {
			if (Enc_A_state == 0)
			{
				if (Enc_Mode == 0)
				{
					Enc_Mode = 1;
					Enc_counter++;
				} else {
					Enc_Mode = 0;
					Enc_counter--;
				}
			}
			Enc_A_state = 1;
		}
	} else {
		if (Enc_A_count > 0)
		{
			Enc_A_count--;
		} else {
			if (Enc_A_state == 1)
			{
				if (Enc_Mode == 0)
				{
					Enc_Mode = 1;
					Enc_counter++;
				} else {
					Enc_Mode = 0;
					Enc_counter--;
				}
			}
			Enc_A_state = 0;
		}
	}

	if (Read_Enc_B == 1)
	{
		if (Enc_B_count < ENC_MAX)
		{
			Enc_B_count++;
		} else {
			if (Enc_B_state == 0)
			{
				if (Enc_Mode == 0)
				{
					Enc_Mode = 1;
					Enc_counter--;
				} else {
					Enc_Mode = 0;
					Enc_counter++;
				}
			}
			Enc_B_state = 1;
		}
	} else {
		if (Enc_B_count > 0)
		{
			Enc_B_count--;
		} else {
			if (Enc_B_state == 1)
			{
				if (Enc_Mode == 0)
				{
					Enc_Mode = 1;
					Enc_counter--;
				} else {
					Enc_Mode = 0;
					Enc_counter++;
				}
			}
			Enc_B_state = 0;
		}
	}
}

void encoder_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //Канали TIM3_CH1, TIM3_CH2 як вхід з підтяжкою
    GPIO_InitTypeDef gpio_cfg;
    gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;
    gpio_cfg.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio_cfg.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio_cfg);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Налаштовуємо TIM3 */
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    TIM_TimeBaseStructInit(&TIMER_InitStructure);
    // Вказуємо TIM_Period - до скількох рахувати таймеру при обертах енкодера
    TIMER_InitStructure.TIM_Period = 400;
    // Дозволяємо рахувати у обидва боки
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up | TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM3, &TIMER_InitStructure);

    /* Налаштовуємо Encoder Interface */
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM3, ENABLE);
}



//----------------------------------------------------
uint16_t getEnc_Counter(void)
{
	return Enc_counter;
}
