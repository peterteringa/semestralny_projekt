#include "stm32l1xx.h"
#include "nastav.h"
#include "defines.h"


volatile uint16_t predstih = 0xfff;

/************************************ inicializuj GPIO ******************/
void gpio_init(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= TIM_IC_Pin;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;

	GPIO_Init(TIM_IC_Port, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM_IC_Port, GPIO_PinSource15, GPIO_AF_TIM2);


	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= TIM_OC_Pin;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;

	GPIO_Init(TIM_OC_Port, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM_OC_Port, GPIO_PinSource6, GPIO_AF_TIM3);


	GPIO_InitStructure.GPIO_Pin 	= PLYN_Sense_Pin;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;

	GPIO_Init(PLYN_Sense_Port, &GPIO_InitStructure);

	RCC_HSICmd(ENABLE);
}

/************************************ inicializuj ADC *******************/
void adc_init(void) {

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	 ADC_InitTypeDef	ADC_InitStructure;

	 ADC_InitStructure.ADC_Resolution 				= ADC_Resolution_12b;
	 ADC_InitStructure.ADC_ContinuousConvMode 		= DISABLE;
	 ADC_InitStructure.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_None;
	 ADC_InitStructure.ADC_DataAlign 				= ADC_DataAlign_Right;
	 ADC_InitStructure.ADC_NbrOfConversion 			= 1;

	 ADC_Init(ADC1, &ADC_InitStructure);

	 ADC_RegularChannelConfig(ADC1, PLYN_Sense_Chan, 1, ADC_SampleTime_384Cycles);

	 /* Enable the ADC */
	 ADC_Cmd(ADC1, ENABLE);

	 /* Wait until the ADC1 is ready */
	 while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET);

}

/************************************ inicializuj NVIC ******************/
void nvic_init(void) {

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitTypeDef 	NVIC_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;


	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel 						= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&NVIC_InitStructure);


	EXTI_InitStructure.EXTI_Line		= DU_interrup;
	EXTI_InitStructure.EXTI_Mode		= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger		= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd		= ENABLE;
	SYSCFG_EXTILineConfig(DU_Sense_Port, DU_Sense_Pin);

	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel 						= ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

}

/************************************ inicializuj TIMER *****************/
void timer_init(void) {

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN, ENABLE);

	TIM_TimeBaseInitTypeDef 	TIM_InitStructure;
	TIM_OCInitTypeDef 			TIM_OC_InitStructure;
	TIM_ICInitTypeDef 			TIM_IC_InitStructure;

	/**************************TIM3 riadni zapalovanie v OutputCompare mode*******************/

	TIM_InitStructure.TIM_Prescaler 		= TIM_prescal;
	TIM_InitStructure.TIM_Period 			= predstih;
	TIM_InitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision 	= 0;

	TIM_TimeBaseInit(TIM3, &TIM_InitStructure);

	TIM_OC_InitStructure.TIM_OCMode 		= TIM_OCMode_Active;
	TIM_OC_InitStructure.TIM_OCPolarity 	= TIM_OCPolarity_Low;
	TIM_OC_InitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
	TIM_OC_InitStructure.TIM_Pulse 			= predstih;

	TIM_OC1Init(TIM3, &TIM_OC_InitStructure);

	/****************************TIM2 pocita otacky v InputCapture mode************************/

	TIM_InitStructure.TIM_Prescaler 		= TIM_prescal;
	TIM_InitStructure.TIM_Period 			= 0xffff;
	TIM_InitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision 	= 0;

	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

	TIM_IC_InitStructure.TIM_Channel 		= TIM_Channel_1;
	TIM_IC_InitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;
	TIM_IC_InitStructure.TIM_ICPrescaler 	= 320;
	TIM_IC_InitStructure.TIM_ICFilter 		= 0x0;
	TIM_IC_InitStructure.TIM_ICSelection 	= TIM_ICSelection_DirectTI;

	TIM_ICInit(TIM2, &TIM_IC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

}
