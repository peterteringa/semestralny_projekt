/*
 * setup.h
 *
 *  Created on: 22. 11. 2016
 *      Author: Peter
 */

#ifndef SETUP_H_
#define SETUP_H_

#include "stm32l1xx.h"

void gpio_init(void);
void adc_init(void);
void nvic_init(void);
void timer_init(void);

#endif /* SETUP_H_ */
