/*
 * ZobovManipulator.cpp
 *
 *  Created on: 30 окт. 2015 г.
 *      Author: Administrator
 */

#include <stdio.h>

#include "ZobovManipulator.h"

#include "stm32f2xx.h"
#include "stm32f2xx_adc.h"
#include "stm32f2xx_rcc.h"

volatile uint32_t      ADCValues;

/*
ZobovEncoderTIM *ZobovManipulator::tm2 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm3 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm4 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm5 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm6 = NULL;
*/

void ZobovManipulator::InitTIM() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10 , ENABLE);


	/*
	ZobovManipulator::tm2 = new ZobovEncoderTIM(TIM2);
	ZobovManipulator::tm3 = new ZobovEncoderTIM(TIM3);
	ZobovManipulator::tm4 = new ZobovEncoderTIM(TIM4);
	ZobovManipulator::tm5 = new ZobovEncoderTIM(TIM5);
	ZobovManipulator::tm6 = new ZobovEncoderTIM(TIM6);
	*/
}

void ZobovManipulator::InitPorts() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	//led
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ZobovManipulator::InitEXTI() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

void ZobovManipulator::InitNVIC() {
	/*
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	*/

	/*
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	*/

	NVIC_InitTypeDef nvicStructure;
	/*nvicStructure.NVIC_IRQChannel = ADC_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
	nvicStructure.NVIC_IRQChannelSubPriority = 2;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
	ADC_ITConfig(ADC1, ADC_IT_OVR, DISABLE);
	ADC_ITConfig(ADC1, ADC_IT_JEOC, DISABLE);
	*/
}
