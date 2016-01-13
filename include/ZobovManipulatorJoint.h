/*
 * ZobovManipulatorJoint.h
 *
 *  Created on: 24 окт. 2015 г.
 *      Author: Administrator
 */
#pragma once

#include "stm32f2xx_tim.h"

#include "ZobovTIM.h"

class ZobovManipulatorStepGPIOPort;
class ZobovManipulatorDirGPIOPort;
class ZobovJointTIM;
class ZobovEncoderTIM;

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
enum jointStatus {IDLE, ROTATE};

class ZobovManipulatorJoint {
public:
	static const OCnFunc * OCnFunction[];
	static IRQn TIMIRQn[];
	static uint8_t GPIO_AF[];


protected:
	ZobovJointTIM *TIM;
	char OCn;
	ZobovManipulatorStepGPIOPort *st;
	ZobovManipulatorDirGPIOPort *dir;
	ZobovEncoderTIM* encTIM;
	jointStatus _status;
	uint8_t dir_lock[2];

	TIM_OCInitTypeDef TIM_OCInitStructure;

public:
	ZobovManipulatorJoint(ZobovJointTIM *t, char o, ZobovManipulatorStepGPIOPort *s, ZobovManipulatorDirGPIOPort *d, ZobovEncoderTIM* e);
	virtual error_joint setSpeed(speed s);
	virtual error_joint rotate(degree theta) = 0;
	virtual error_joint stop() = 0;
	void setDirection(direction d);
	direction getDirection();
	//void setStatus(jointStatus d);
	jointStatus getStatus();
	void setDirLock(direction d);
	void unsetDirLock(direction d);

	virtual ~ZobovManipulatorJoint();
};
