/*
 * defines.h
 *
 *  Created on: 23. 11. 2016
 *      Author: Peter
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/************************************ Port a Piny pre LCD *******************************/
#define LCD_Port 		GPIOA
#define RS 				GPIO_Pin_1
#define EN 				GPIO_Pin_4
#define D4 				GPIO_Pin_7
#define D5 				GPIO_Pin_8
#define D6 				GPIO_Pin_9
#define D7 				GPIO_Pin_10

/************************************ Port a Pin pre InputCapture vstup *****************/
#define TIM_IC_Port		GPIOA
#define TIM_IC_Pin		GPIO_Pin_15

/************************************ Port a Pin pre OutputCompare vystup ****************/
#define TIM_OC_Port		GPIOA
#define TIM_OC_Pin		GPIO_Pin_6

/************************************ Port a Pin pre externe prerusenie v DU *************/
#define DU_Sense_Port	EXTI_PortSourceGPIOC
#define DU_Sense_Pin	EXTI_PinSource0
#define DU_interrup		EXTI_Line0

/************************************ Port a Pin pre snimanie plynu(pozadovane otacky) ***/
#define PLYN_Sense_Port	GPIOA
#define PLYN_Sense_Pin	GPIO_Pin_0
#define PLYN_Sense_Chan ADC_Channel_0

#define TIM_prescal		320

#define MAX_rpm			12000;

#endif /* DEFINES_H_ */
