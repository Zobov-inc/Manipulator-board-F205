/*
 * ZobovManipulator.cpp
 *
 *  Created on: 30 окт. 2015 г.
 *      Author: Administrator
 */

#include <stdio.h>
#include <math.h>

#include "ZobovManipulator.h"
#include "ZobovLimitingSwitch.h"
#include "ZobovManipulatorJointStepperMotorInc.h"

#include "stm32f2xx.h"
#include "stm32f2xx_pwr.h"
#include "stm32f2xx_rtc.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_usart.h"

using std::array;
ZobovManipulatorJoint *ZobovManipulator::joint[JOINT_CT] = { NULL };
ZobovManipulatorJointLimitingSwitch *ZobovManipulator::lim_switch[LIM_SWITCH_CT] = { NULL };
ZobovManipulatorButtonLimitingSwitch *ZobovManipulator::button = NULL;
ZobovGrabber* ZobovManipulator::graber = NULL;
ZobovUSART* ZobovManipulator::usart = NULL;
bool ZobovManipulator::timeLock = false;
bool ZobovManipulator::ready = false;

void ZobovManipulator::Init(uint64_t dbg) {
	InitPorts();
	InitUSART();
	InitEXTI();
	InitTIM();
	InitNVIC();
	InitRTC();
	InitJoint();
	InitGraber();
	InitLimSwitch();
	InitButtonSwitch();

	if(dbg & 1<<0) {
		Rotate(1, 40, CLOCK);
		Rotate(2, 30, COUNTERCLOCK);
		WaitAll();
	}

	if(dbg & 1<<1) {
		degree q[JOINT_CT] = {};
		Rollback(q, q);
	}

	RotateToStart();
}

void ZobovManipulator::InitTIM() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);


	/*
	ZobovManipulator::tm2 = new ZobovEncoderTIM(TIM2);
	ZobovManipulator::tm3 = new ZobovEncoderTIM(TIM3);
	ZobovManipulator::tm4 = new ZobovEncoderTIM(TIM4);
	ZobovManipulator::tm5 = new ZobovEncoderTIM(TIM5);
	ZobovManipulator::tm6 = new ZobovEncoderTIM(TIM6);
	*/
}

void ZobovManipulator::InitPorts() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	//led
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void ZobovManipulator::InitUSART() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	usart = new ZobovUSART(6, 6, 7);
}


void ZobovManipulator::InitEXTI() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* EXTI configuration */
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

void ZobovManipulator::InitJoint() {

	joint[0] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(3), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 6, 3), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 1), 30, 3.4);//switch at COUNTERCLOCK
	joint[1] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(4), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 6, 4), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 3), 30, 47);//switch at COUNTERCLOCK
	joint[2] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(5), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 0, 5), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 7), 30, 50);//switch at COUNTERCLOCK
	joint[3] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(9), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 2, 9), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 9), 30, 0.52);//switch at COUNTERCLOCK
	joint[3]->setSpeed(20000);

}

void ZobovManipulator::InitLimSwitch() {
	lim_switch[0] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 13),  joint[0], CLOCK);
	lim_switch[1] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 12),  joint[1], COUNTERCLOCK);
	lim_switch[2] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 9),  joint[2], CLOCK);
	lim_switch[3] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 1),  joint[3], CLOCK);

	//lim_switch[4] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 2),  joint[4], CLOCK);
	//lim_switch[5] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 1),  joint[5], CLOCK);
}

void ZobovManipulator::InitButtonSwitch() {
	button = new ZobovManipulatorButtonLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 11), &ready);
}

void ZobovManipulator::InitGraber() {
	graber = new ZobovGrabber(new ZobovGraberGPIOPort(RCC_AHB1Periph_GPIOC, 2, 4));
}

void ZobovManipulator::Rollback(degree q[JOINT_CT], degree sq[JOINT_CT]) {
	uint32_t a;
	array<uint8_t, JOINT_CT> seq = { 1, 2, 3, 0 };
	for(uint8_t i : seq) {
		a = abs(q[i]-sq[i]);
		if ( a > joint[i]->getRollbackDegree() ) {
			joint[i]->setDirection( lim_switch[i]->getDir());
			joint[i]->rotate(a - joint[i]->getRollbackDegree());
		}
		else {
			joint[i]->setDirection( lim_switch[i]->getDir() == CLOCK ? COUNTERCLOCK : CLOCK );
			joint[i]->rotate(joint[i]->getRollbackDegree() - a);
		}
	}
	WaitAll();
}

void ZobovManipulator::RotateToStart() {
	assert(JOINT_CT <= LIM_SWITCH_CT);

	array<uint8_t, JOINT_CT> seq = { 1, 2, 3, 0 };
	for(int8_t i : seq) {
		joint[i]->setDirectionToZero( lim_switch[i]->getDir());
		joint[i]->setDirection( lim_switch[i]->getDir() );
		joint[i]->rotate(3600);
		WaitAll();
	}
}

void ZobovManipulator::InitNVIC() {

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void ZobovManipulator::InitRTC() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    /* Enable the PWR clock */
	PWR_BackupAccessCmd(ENABLE);                          /* Allow access to RTC */
	RCC_LSICmd(ENABLE);                                   /* Enable the LSI OSC */

	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);   /* Wait till LSI is ready */

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);               /* Select the RTC Clock Source */
	RCC_RTCCLKCmd(ENABLE);                                /* Enable the RTC Clock */
	RTC_WaitForSynchro();                  /* Wait for RTC APB registers synchronisation */

	RTC_InitTypeDef RTC_InitStruct;
	RTC_StructInit(&RTC_InitStruct);
	RTC_InitStruct.RTC_AsynchPrediv = 127;
	RTC_InitStruct.RTC_SynchPrediv = 249;
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStruct);

    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);   /* disable before setting or cann't write */
	RTC_AlarmCmd(RTC_Alarm_B, DISABLE);   /* disable before setting or cann't write */
}


void ZobovManipulator::enableRTCAlarm(seconds sec) {
	/* set time to 8:00:00 */
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_TimeStruct.RTC_Hours = 0x01;
	RTC_TimeStruct.RTC_Minutes = 0x01;
	RTC_TimeStruct.RTC_Seconds = 0x01;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);

	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);   /* disable before setting or cann't write */

	sec += RTC_TimeStruct.RTC_Seconds + RTC_TimeStruct.RTC_Minutes*60 + RTC_TimeStruct.RTC_Hours*3600;
	uint16_t min = (uint16_t)sec/60;
	uint8_t hour = (uint8_t)min/60;
	assert(hour < 12);
	min -= hour*60;
	sec = sec%60;
	assert(RTC_TimeStruct.RTC_Hours + hour < 12);
	assert(RTC_TimeStruct.RTC_Minutes < 60);
	assert(RTC_TimeStruct.RTC_Seconds < 60);
	/* set alarm time 8:00:00 + user time everyday */
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = hour;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = min;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = sec;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31; // Nonspecific
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay; // Everyday
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

	/* Enable Alarm */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

void ZobovManipulator::disableRTCAlarm() {
	/* set time to 8:00:00 */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);   /* disable before setting or cann't write */

	/* Disable Alarm */
	RTC_ITConfig(RTC_IT_ALRA, DISABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}


//USER API BEGIN

//void ZobovManipulator::Rotate(uint8_t num, degree deg) {
//	assert(num <= JOINT_CT);
//	joint[num]->rotate(deg);
//}

//void ZobovManipulator::Rotate(uint8_t num, degree deg, speed spd) {
//	assert(num <= JOINT_CT);
//	joint[num]->setSpeed(spd);
//	Rotate(num, deg);
//}

//void ZobovManipulator::Rotate(uint8_t num, degree deg, direction dir) {
//	assert(num <= JOINT_CT);
//	joint[num]->setDirection(dir);
//	Rotate(num, deg);
//}

void ZobovManipulator::Rotate(uint8_t num, degree deg, direction dir, speed spd) {
	assert(num <= JOINT_CT);
	if (dir != NONE)joint[num]->setDirection(dir);
	if (spd >= 0) joint[num]->setSpeed(spd);
//	Rotate(num, deg);
	joint[num]->rotate(deg);
}

void ZobovManipulator::WaitTime(seconds sec) {
    lockTimeLock();
	enableRTCAlarm(sec);
    /* Waiting for release... */
    while(timeLock);
}

void ZobovManipulator::WaitAll() {
	for(uint8_t i = 0; i < JOINT_CT; ++i)
		while(joint[i]->getStatus() != IDLE);
}
//USER API END

ZobovManipulator::~ZobovManipulator() {
	for(uint8_t i = 0; i < JOINT_CT; ++i)
		delete joint[i];
	for(uint8_t i = 0; i < LIM_SWITCH_CT; ++i)
		delete lim_switch[i];
}

void ZobovManipulatorMathHelper::calcManipulatorRotateOld(const Point Manipulator, const array<degree, 3> in_angles, const array<dimention, 4> d, array<degree, 6> res_angles) {
	Point P2;
	P2.x = Manipulator.x - d[3]*cosf(in_angles[1])*cosf(in_angles[0]);
	P2.y = Manipulator.y - d[3]*cosf(in_angles[1])*sinf(in_angles[0]);
	P2.z = Manipulator.z - d[3]*cosf(in_angles[1])*sinf(in_angles[1]);
	res_angles[0] = atanf(P2.x/P2.y);
	return;
}

void ZobovManipulator::SetJointAngles(degree q[JOINT_CT]) {
	for(int i = 0; i < JOINT_CT; ++i)
		joint[i]->setAngle(q[i]);
}
