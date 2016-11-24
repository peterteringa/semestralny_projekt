/*
 * semestralny_projekt.c
 *
 *  Created on: 23. 11. 2016
 *      Author: Peter
 */

#include "stm32l1xx.h"
#include "semestralny_projekt.h"
#include "defines.h"
#include "hd44780.h"

extern uint16_t rpm, AD_value;

/************************************ odcitanie hodnoty z ADC ***********/
void ADC1_IRQHandler (void){

	if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){				//ak prerusenie vyvolalo EOC
		AD_value = ADC_GetConversionValue(ADC1);			//citanie vysledku prevodu

	}
}

/************************************ DOLNA UVRAT ***********************/
void EXTI0_IRQHandler(void) {

	if (EXTI_GetFlagStatus(DU_interrup)) {
		EXTI_ClearFlag(DU_interrup);
		/*TIM_Cmd(TIM2, ENABLE);
		TIM_Cmd(TIM3, ENABLE);*/

		rpm++;
	}
}

/************************************ HORNA UVRAT ***********************/
void TIM2_IRQHandler(void) {

	if (TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1)) {
		TIM_ClearFlag(TIM2, TIM_FLAG_CC1);

		rpm++;

		/*rpm = 3000000/TIM_GetCapture1(TIM2);
		TIM_Cmd(TIM2, DISABLE);
		TIM_Cmd(TIM3, DISABLE);*/
	}
}

