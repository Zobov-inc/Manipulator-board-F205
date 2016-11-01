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

enum direction {CLOCK, COUNTERCLOCK, NONE};
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
	degree rollback_degree;
	float coef;
	degree q;

	TIM_OCInitTypeDef TIM_OCInitStructure;

public:
	ZobovManipulatorJoint(ZobovJointTIM *t, uint8_t o, ZobovManipulatorStepGPIOPort *s, ZobovManipulatorDirGPIOPort *d, degree rd, float k, ZobovEncoderTIM* e);
	virtual error_joint setSpeed(speed s);
	virtual error_joint rotate(degree theta) = 0;
	degree getRollbackDegree() { return rollback_degree; };
	virtual error_joint stop() = 0;
	void setDirection(direction d);
	direction getDirection();
	jointStatus getStatus();
	void setStatus(jointStatus s);
	void setDirLock(direction d);
	void setAngle(degree qq);
	void unsetDirLock(direction d);
	void rotateToZero();
	void setDirectionToZero(direction d) { dirToZero = d; };

	virtual ~ZobovManipulatorJoint();
};
