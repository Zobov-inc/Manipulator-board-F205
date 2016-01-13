/*
 * ZobovManipulator.cpp
 *
 *  Created on: 30 окт. 2015 г.
 *      Author: Administrator
 */

#include <stdio.h>

#include "ZobovManipulator.h"
#include "ZobovLimitingSwitch.h"
#include "ZobovManipulatorJointStepperMotorInc.h"

#include "stm32f2xx.h"
#include "stm32f2xx_rcc.h"

/*
ZobovEncoderTIM *ZobovManipulator::tm2 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm3 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm4 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm5 = NULL;
ZobovEncoderTIM *ZobovManipulator::tm6 = NULL;
*/
ZobovManipulatorJoint *ZobovManipulator::joint[JOINT_CT] = {NULL};
ZobovLimitingSwitch *ZobovManipulator::lim_switch[LIM_SWITCH_CT] = {NULL};

void ZobovManipulator::Init() {
	InitPorts();
	InitEXTI();
	InitTIM();
	InitNVIC();
	InitJoint();
	InitLimSwitch();
	InitLimits();
}

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

void ZobovManipulator::InitJoint() {
//	joint[0] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(5), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 9, 5), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 12));
//	joint[1] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(11), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 15, 11), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOC, 2, 8));
	joint[0] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(10), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 8, 10), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 11));
}

void ZobovManipulator::InitLimSwitch() {
	lim_switch[0] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 0), joint[0]);
	lim_switch[1] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 1), joint[0]);
}

void ZobovManipulator::InitLimits() {
/*
	joint[0]->setDirection(CLOCK);
	joint[0]->rotate(360);
	for(int i = 0; i < 1000000; ++i);
	joint[0]->setDirection(COUNTERCLOCK);
	joint[0]->rotate(360);
*/
	for(uint8_t i = 0; i < JOINT_CT; ++i) {
		joint[i]->setDirection(CLOCK);
		joint[i]->rotate(3600);
	}

	for(uint8_t i = 0; i < JOINT_CT; ++i)
		while(joint[i]->getStatus() != IDLE);

	for(uint8_t i = 0; i < JOINT_CT; ++i) {
		joint[i]->setDirection(COUNTERCLOCK);
		joint[i]->rotate(3600);
	}

	for(uint8_t i = 0; i < JOINT_CT; ++i)
		while(joint[i]->getStatus() != IDLE);

}

void ZobovManipulator::InitNVIC() {
	//NVIC_InitTypeDef nvicStructure;

	/*
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	*/

	/*
	nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	*/

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

ZobovManipulator::~ZobovManipulator() {
	for(uint8_t i = 0; i < JOINT_CT; ++i)
		delete joint[i];
	for(uint8_t i = 0; i < LIM_SWITCH_CT; ++i)
		delete lim_switch[i];
}
