/*
 * ZobovManipulatorJoint.cpp
 *
 *  Created on: 24 окт. 2015 г.
 *      Author: Administrator
 */

#include <stdio.h>
#include <assert.h>

#include "stm32f2xx_gpio.h"

#include "ZobovGPIOPort.h"
#include "ZobovTIM.h"
#include "ZobovManipulatorJoint.h"

OCnFunc* ZobovManipulatorJoint::OCnFunction[] = {TIM_OC1Init, TIM_OC2Init, TIM_OC3Init, TIM_OC4Init};
IRQn ZobovManipulatorJoint::TIMIRQn[] = {TIM2_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM2_IRQn, TIM2_IRQn, TIM2_IRQn, TIM2_IRQn, TIM2_IRQn, TIM1_UP_TIM10_IRQn, TIM1_TRG_COM_TIM11_IRQn, TIM8_BRK_TIM12_IRQn, TIM2_IRQn};
uint8_t ZobovManipulatorJoint::GPIO_AF[] = {GPIO_AF_TIM1, GPIO_AF_TIM2, GPIO_AF_TIM3, GPIO_AF_TIM4, GPIO_AF_TIM5, 0, 0, GPIO_AF_TIM8, GPIO_AF_TIM9, GPIO_AF_TIM10, GPIO_AF_TIM11};

ZobovManipulatorJoint::ZobovManipulatorJoint(ZobovJointTIM *t, char o, ZobovManipulatorStepGPIOPort *s, ZobovManipulatorDirGPIOPort *d, ZobovEncoderTIM* e = NULL) : TIM(t), OCn(o-1), st(s), dir(d), encTIM(e), _status(IDLE), dir_lock{0,0} {
	assert(OCn >= 0);
	assert(OCn <= 3);

	assert(RCC->AHB1ENR & st->RCC_AHB);
	assert(RCC->AHB1ENR & dir->RCC_AHB);

	assert(!(TIM->getTIM()->CR1 & TIM_CR1_CEN));

	GPIO_InitTypeDef GPIO_InitStructure;

	//PWM pin config
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = st->getPin();
	GPIO_Init(st->getGPIO(), &GPIO_InitStructure);
	GPIO_PinAFConfig(st->getGPIO(), st->getPinSource(), GPIO_AF[TIM->num]);

	//direction pin config
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = dir->getPin();
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(dir->getGPIO(), &GPIO_InitStructure);

	setDirection(COUNTERCLOCK);

	setSpeed(0); //TODO set speed

	//assert(encTIM->Joint == NULL);
	if (encTIM->Joint != NULL)
		encTIM->Joint = this;
	else {
		TIM->Joint = this;
	}
}

void ZobovManipulatorJoint::setDirection(direction d) {
	if (d == COUNTERCLOCK) {
		GPIO_ResetBits(dir->getGPIO(), dir->getPin());
	}
	else {
		GPIO_SetBits(dir->getGPIO(), dir->getPin());
	}

}

direction ZobovManipulatorJoint::getDirection() {
	return (direction)GPIO_ReadOutputDataBit(dir->getGPIO(), dir->getPin());
}

void ZobovManipulatorJoint::setDirLock(direction d) {
	dir_lock[d] = true;
}

void ZobovManipulatorJoint::unsetDirLock(direction d) {
	dir_lock[d] = false;
}

jointStatus ZobovManipulatorJoint::getStatus() {
	return _status;
}

error_joint ZobovManipulatorJoint::setSpeed(speed s) {
	uint32_t pulse;
	if (!s) {
		pulse = 0;
	}
	else {
		//if (TIM->isLock()) return 1;
		pulse = (5 * (TIM->period - 1) / 10); //TODO get pulse from speed
	}

/* Channel 1 output configuration */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = pulse;
	OCnFunction[OCn](TIM->getTIM(), &TIM_OCInitStructure);

	return 0;
}

ZobovManipulatorJoint::~ZobovManipulatorJoint() {
}

