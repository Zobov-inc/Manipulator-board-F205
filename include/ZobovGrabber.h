/*
 * ZobovGrabber.h
 *
 *  Created on: 31 џэт. 2016 у.
 *      Author: Administrator
 */

#pragma once

#include "ZobovGPIOPort.h"

class ZobovGrabber {
private:
	ZobovGraberGPIOPort* port;
public:
	ZobovGrabber(ZobovGraberGPIOPort* p) : port(p) {}
	void take() {
    	GPIO_SetBits(port->getGPIO(), port->getPin());
	}
	void put() {
    	GPIO_ResetBits(port->getGPIO(), port->getPin());
	}
	virtual ~ZobovGrabber() {
		if (port) delete port;
	}
};
