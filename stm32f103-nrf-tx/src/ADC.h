/**
 * @author Parfeniuk Bogdan
 *
 * @section LICENSE
 *
 *2016 ARM
 *
 */

#ifndef ADC_H
#define ADC_H

/**
 * Includes
 */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

/**
 * Defines
 */

unsigned int adc_res; // переменная для записи результата АЦП


void init_adc(void);

#endif /* ADC_H */
