/*
 * ZobovLimitingSwitch.h
 *
 *  Created on: 27 дек. 2015 г.
 *      Author: Administrator
 */
#pragma once

#include "stm32f2xx_syscfg.h"

#include <ZobovGPIOPort.h>
#include "EXTIHelper.h"

class ZobovLimitingSwitch {
public:

protected:
	ZobovSwitchGPIOPort *port;

public:
	ZobovLimitingSwitch(ZobovSwitchGPIOPort *p ) : port(p)  {
		EXTI_InitTypeDef   EXTI_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

		SYSCFG_EXTILineConfig(EXTIHelper::EXTI_PortSource[port->GPIONum], EXTIHelper::EXTI_PinSource[port->pinNum]);

		/* Configure EXTI Line0 */
		EXTI_InitStructure.EXTI_Line = EXTIHelper::EXTI_Line[port->pinNum];
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		/* Enable and set EXTI Line0 Interrupt to the highest priority */
		NVIC_InitStructure.NVIC_IRQChannel = EXTIHelper::EXTI_IRQ[port->pinNum];
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		EXTIHelper::SetLineLetter(port->pinNum, port->GPIONum);
	}
	virtual ~ZobovLimitingSwitch() {};
};

class ZobovManipulatorJointLimitingSwitch : public ZobovLimitingSwitch, public EXTIIRQn_Interface {
protected:
	ZobovManipulatorJoint *joint;
	direction dir;

public:
	ZobovManipulatorJointLimitingSwitch(ZobovSwitchGPIOPort *p, ZobovManipulatorJoint *j, direction d) : ZobovLimitingSwitch(p), joint(j), dir(d) {
		EXTIHelper::SetEXTIIRQn(port->pinNum, port->GPIONum, this);
	};

	direction getDir() { return dir; };

	virtual void IRQn() {
		assert(joint != NULL);
		if (joint->getDirection() == dir) {
			joint->stop();
			joint->setDirLock(joint->getDirection());
		}
	};

	virtual ~ZobovManipulatorJointLimitingSwitch() {};
};

class ZobovManipulatorButtonLimitingSwitch : public ZobovLimitingSwitch, public EXTIIRQn_Interface {
private:
	bool *control;
	uint8_t mode;

public:
	ZobovManipulatorButtonLimitingSwitch(ZobovSwitchGPIOPort *p, bool *c, uint8_t m = 1) : ZobovLimitingSwitch(p), control(c), mode(m) {
		EXTIHelper::SetEXTIIRQn(port->pinNum, port->GPIONum, this);
	}

	virtual void IRQn() {
		switch (mode) {
			case 0:
				*control = !*control;
				break;
			case 1:
				*control = true;
				break;
			case 2:
				*control = false;
				break;
			default:
				assert("Unsupported button mode!");
				break;
		}
	};
};
