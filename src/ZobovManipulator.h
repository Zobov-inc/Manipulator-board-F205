/*
 * ZobovManipulator.h
 *
 *  Created on: 30 окт. 2015 г.
 *      Author: Administrator
 */

#pragma once

#include "stdint.h"
#include "system_stm32f2xx.h"

#include "StdStruct.h"

class ZobovManipulator {
public:
	/*
	static ZobovEncoderTIM *tm2;
	static ZobovEncoderTIM *tm3;
	static ZobovEncoderTIM *tm4;
	static ZobovEncoderTIM *tm5;
	static ZobovEncoderTIM *tm6;
	*/

	static void InitTIM();
	static void InitPorts();
	static void InitNVIC();

private:
	ZobovManipulator() { }
public:
	~ZobovManipulator() { }
};
