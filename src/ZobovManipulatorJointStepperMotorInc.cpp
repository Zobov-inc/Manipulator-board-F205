/*
 * ZobovManipulatorJointStepperMotorInc.cpp
 *
 *  Created on: 24 окт. 2015 г.
 *      Author: Administrator
 */

#include "ZobovManipulatorJointStepperMotorInc.h"
#include <cmath>
#include <assert.h>
#include "ZobovTIM.h"

error_joint ZobovManipulatorJointStepperMotorInc::rotate(degree theta) {
	//auto limit = theta/step*microstep;

	assert(!dir_lock[getDirection()]);

	assert(!TIM->isLock());
	TIM->lock();

	//limit = ceil(theta/step*microstep);
	uint32_t limit = (uint32_t)ceil(theta/360*200*32*2);

	_status = ROTATE;

	//set TIM
	if (encTIM) encTIM->setLimit(limit);
	else TIM->setLimit(limit);

	setSpeed(1); //TODO set speed
	TIM->EnableTim();
	dir_lock[!getDirection()] = false;
	return 0;
}

error_joint ZobovManipulatorJointStepperMotorInc::stop() {
	//assert(TIM->isLock());

	setSpeed(0); //TODO set speed
	TIM->DisableTim();
	TIM->unlock();
	_status = IDLE;
	return 0;
}
/*
void StepperMotorIncTIM11Func() {
}*/

ZobovManipulatorJointStepperMotorInc::~ZobovManipulatorJointStepperMotorInc() {
}
