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
typedef uint16_t error_joint;
typedef float degree;
//degree per sec
typedef float speed;
typedef uint16_t seconds;
typedef void (OCnFunc)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);

enum direction {CLOCK, COUNTERCLOCK};
enum jointStatus {IDLE, ROTATE};

class ZobovManipulatorJoint {
public:
	static const OCnFunc * OCnFunction[];
	static IRQn TIMIRQn[];
	static uint8_t GPIO_AF[];

	direction dirToZero;
	jointStatus status;

protected:
//	Right Way
//	static constexpr direction dirToZero = CLOCK;

	ZobovJointTIM *TIM;
	uint8_t OCn;
	ZobovManipulatorStepGPIOPort *st;
	ZobovManipulatorDirGPIOPort *dir;
	ZobovEncoderTIM* encTIM;
//	RIGHT WAY
//	jointStatus status;
	uint8_t dir_lock[2];
	direction dr;
	degree degreeToZero;

	TIM_OCInitTypeDef TIM_OCInitStructure;

public:
	ZobovManipulatorJoint(ZobovJointTIM *t, uint8_t o, ZobovManipulatorStepGPIOPort *s, ZobovManipulatorDirGPIOPort *d, degree dToZ, ZobovEncoderTIM* e);
	virtual error_joint setSpeed(speed s);
	virtual error_joint rotate(degree theta) = 0;
	virtual error_joint stop() = 0;
	void setDirection(direction d);
	direction getDirection();
	//void setStatus(jointStatus d);
	jointStatus getStatus();
	void setStatus(jointStatus s);
	void setDirLock(direction d);
	void unsetDirLock(direction d);
	void rotateToZero();
	void setDirectionToZero(direction d) { dirToZero = d; };

	virtual ~ZobovManipulatorJoint();
};
