/*
 * ZobovManipulatorJoint.h
 *
 *  Created on: 24 окт. 2015 г.
 *      Author: Administrator
 */
#pragma once

#include "stm32f2xx_tim.h"

struct ZobovGPIOPort;
struct ZobovJointTIM;
struct ZobovEncoderTIM;

/*
 * 0 - no errors;
 * 1 - setSpeed Tim locked
 */
typedef unsigned short error_joint;
typedef float degree;
//degree per sec
typedef float speed;
typedef void (OCnFunc)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);

enum direction {CLOCK, COUNTERCLOCK};

class ZobovManipulatorJoint {
protected:
	static const OCnFunc * OCnFunction[];
	static IRQn TIMIRQn[];
	static uint8_t GPIO_AF[];

	ZobovJointTIM *TIM;
	char OCn;
	ZobovGPIOPort *st, *dir;
	ZobovEncoderTIM* encTIM;

	TIM_OCInitTypeDef TIM_OCInitStructure;

public:
	ZobovManipulatorJoint(ZobovJointTIM *t, char o, ZobovGPIOPort *s, ZobovGPIOPort *d, ZobovEncoderTIM* e);
	virtual error_joint setSpeed(speed s);
	virtual error_joint rotate(degree theta) = 0;
	void setDirection(direction d);

	virtual ~ZobovManipulatorJoint();
};
