/*
 * ZobovManipulator.h
 *
 *  Created on: 30 окт. 2015 г.
 *      Author: Administrator
 */

#pragma once

#include <stdint.h>

#include "system_stm32f2xx.h"

#include "ZobovGPIOPort.h"
#include "ZobovLimitingSwitch.h"


class ZobovManipulator {
public:
	static constexpr uint8_t JOINT_CT = 1;//3;
	static constexpr uint8_t LIM_SWITCH_CT = JOINT_CT*2;

private:
	static ZobovManipulatorJoint *joint[JOINT_CT];
	static ZobovLimitingSwitch *lim_switch[LIM_SWITCH_CT];

	/*
	static ZobovEncoderTIM *tm2;
	static ZobovEncoderTIM *tm3;
	static ZobovEncoderTIM *tm4;
	static ZobovEncoderTIM *tm5;
	static ZobovEncoderTIM *tm6;
	*/


private:
	static void InitTIM();
	static void InitEXTI();
	static void InitPorts();
	static void InitNVIC();
	static void InitJoint();
	static void InitLimSwitch();
	static void InitLimits();

public:
	static void Init();

private:
	ZobovManipulator() { }
public:
	~ZobovManipulator();
};
