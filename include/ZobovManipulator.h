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

#include "ZobovUSART.h"
#include "ZobovGPIOPort.h"
#include "ZobovLimitingSwitch.h"
#include "ZobovGrabber.h"

using std::array;

typedef float dimention;

class ZobovManipulator {
public:
	static constexpr uint8_t JOINT_CT = 4;
	static constexpr uint8_t LIM_SWITCH_CT = JOINT_CT;//*2 if you want two limiting switches per axe.
	static bool ready;

private:
	static ZobovManipulatorJoint *joint[JOINT_CT];
	static ZobovManipulatorJointLimitingSwitch *lim_switch[LIM_SWITCH_CT];
	static ZobovManipulatorButtonLimitingSwitch *button;
	static ZobovGrabber* graber;
	static ZobovUSART* usart;
	static bool timeLock;

private:
	static void InitTIM();
	static void InitUSART();
	static void InitEXTI();
	static void InitPorts();
	static void InitNVIC();
	static void InitRTC();
	static void InitJoint();
	static void InitLimSwitch();
	static void InitButtonSwitch();
	static void InitGraber();

	static void enableRTCAlarm(seconds sec);

public:
	static void Rollback(degree q[JOINT_CT], degree sq[JOINT_CT]);
	static void SetJointAngles(degree q[JOINT_CT]);
	static void RotateToStart();
	static void Init(uint64_t dbg = 0);
	static void lockTimeLock() { timeLock = true; };
	static void releaseTimeLock() { timeLock = false; };
	static void disableRTCAlarm();

	//USER API BEGIN
//	static void Rotate(uint8_t num, degree deg);
//	static void Rotate(uint8_t num, degree deg, speed spd);
//	static void Rotate(uint8_t num, degree deg, direction dir);
	static void Rotate(uint8_t num, degree deg, direction dir = NONE, speed spd = -1);
	static void Grab(seconds sec = 30) { graber->take(); ZobovManipulator::WaitTime(sec); };
	static void UnGrab() { graber->put(); };
	static void WaitTime(seconds sec);
	static void WaitAll();
	static char ReadChar() { return usart->readChar(); };
	static uint32_t ReadUInt32() { return usart->readUInt32(); };
	static void WriteChar(char c) { usart->writeChar(c); } ;
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

