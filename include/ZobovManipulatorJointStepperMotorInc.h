/*
 * ZobovManipulatorJointStepperMotorInc.h
 *
 *  Created on: 24 окт. 2015 г.
 *      Author: Administrator
 */

#pragma once

#include <stdlib.h>

#include "ZobovManipulatorJoint.h"

class ZobovManipulatorJointStepperMotorInc: public ZobovManipulatorJoint {
private:
	static constexpr auto step = 1.8;
	static constexpr auto microstep = 16;

	NVIC_InitTypeDef nvicStructure;

public:
	//t - tim for irq; o - ccmr; p - period
	ZobovManipulatorJointStepperMotorInc(ZobovJointTIM *t, char o, ZobovGPIOPort *s, ZobovGPIOPort *d, ZobovEncoderTIM* e = NULL) : ZobovManipulatorJoint(t, o, s, d, e) {};
	virtual error_joint rotate(degree theta);
	virtual ~ZobovManipulatorJointStepperMotorInc();
};

