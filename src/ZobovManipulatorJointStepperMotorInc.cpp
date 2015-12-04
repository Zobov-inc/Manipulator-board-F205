/*
 * ZobovManipulatorJointStepperMotorInc.cpp
 *
 *  Created on: 24 окт. 2015 г.
 *      Author: Administrator
 */

#include "ZobovManipulatorJointStepperMotorInc.h"
#include "StdStruct.h"

#include <cmath>
#include <assert.h>

error_joint ZobovManipulatorJointStepperMotorInc::rotate(degree theta) {
	//auto limit = theta/step*microstep;
	assert(!TIM->isLock());
	TIM->lock();

	//limit = ceil(theta/step*microstep);
	uint32_t limit = (uint32_t)ceil(theta/360*200*32*2);

	//set EncoderTIM
	if (encTIM) encTIM->setLimit(limit);
	else TIM->setLimit(limit);

	setSpeed(1); //TODO set speed
	TIM->EnableTim();
	return 0;
}
/*
void StepperMotorIncTIM11Func() {
}*/

ZobovManipulatorJointStepperMotorInc::~ZobovManipulatorJointStepperMotorInc() {
}
