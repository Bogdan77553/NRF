#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f10x.h"

void encoder_ini(void);
uint16_t getEnc_Counter(void);
void Systick_Enc(void);
void encoder_ini2(void);
void encoder_init(void);


#endif
