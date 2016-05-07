/*
 * ZobovManipulator.h
 *
 *  Created on: 30 окт. 2015 г.
 *      Author: Administrator
 */

#pragma once

#include <stdint.h>
#include <array>

#include "system_stm32f2xx.h"

#include "ZobovGPIOPort.h"
#include "ZobovLimitingSwitch.h"
#include "ZobovGrabber.h"

using std::array;

typedef float dimention;

class ZobovManipulator {
public:
	static constexpr uint8_t JOINT_CT = 3;
	static constexpr uint8_t LIM_SWITCH_CT = JOINT_CT;//*2 if you want two limiting switches per axe.

private:
	static ZobovManipulatorJoint *joint[JOINT_CT];
	static ZobovManipulatorJointLimitingSwitch *lim_switch[LIM_SWITCH_CT];
	static ZobovGrabber* graber;
	static bool timeLock;

private:
	static void InitTIM();
	static void InitUART();
	static void InitEXTI();
	static void InitPorts();
	static void InitNVIC();
	static void InitRTC();
	static void InitJoint();
	static void InitLimSwitch();
	static void InitGraber();

	static void enableRTCAlarm(seconds sec);

public:
	static void RotateToStart();
	static void Init();
	static void lockTimeLock() { timeLock = true; };
	static void releaseTimeLock() { timeLock = false; };
	static void disableRTCAlarm();

	//USER API BEGIN
	static void Rotate(uint8_t num, degree deg);
	static void Rotate(uint8_t num, degree deg, speed spd);
	static void Rotate(uint8_t num, degree deg, direction dir);
	static void Rotate(uint8_t num, degree deg, direction dir, speed spd);
	static void Grab(seconds sec = 30) { graber->take(); ZobovManipulator::WaitTime(sec); };
	static void UnGrab() { graber->put(); };
	static void WaitTime(seconds sec);
	static void WaitAll();
	//USER API END

private:
	ZobovManipulator() { }
public:
	~ZobovManipulator();
};

struct Point {
	float x,y,z;
};


class ZobovManipulatorMathHelper {
private:
	ZobovManipulatorMathHelper() {};

public:
	static void calcManipulatorRotateOld(const Point Manipulator, const array<degree, 3> in_angles, const array<dimention, 4> d, array<degree, 6> res_angles);
	static array<degree, 6> calcManipulatorRotate(Point P, const array<degree, 3> trg_angles, const array<dimention, 4> d);
	~ZobovManipulatorMathHelper() {};
};

