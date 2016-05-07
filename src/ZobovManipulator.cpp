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
ZobovGrabber* ZobovManipulator::graber = NULL;
bool ZobovManipulator::timeLock = false;

void ZobovManipulator::Init() {
	InitPorts();
	//InitUART();
	InitEXTI();
	InitTIM();
	InitNVIC();
	InitRTC();
	InitJoint();
	InitGraber();
	InitLimSwitch();
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12 , ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10 , ENABLE);
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

/*
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4); //PD5 to TX USART2
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); //PD6 to RX USART2

	// Настраиваем ногу PC10 как выход TX UARTа (TxD)
	// Причем не просто выход, а выход с альтернативной функцией
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Настраиваем ногу PC11 как вход RX UARTа (RxD)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
*/
}

/*
void ZobovManipulator::InitUART() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	//Включаем порты и UART1
	//Выключаем JTAG (он занимает ноги нужные нам)
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);


	//Заполняем структуру настройками UARTa
	USART_InitTypeDef uart_struct;
	uart_struct.USART_BaudRate            = 9600;
	uart_struct.USART_WordLength          = USART_WordLength_8b;
	uart_struct.USART_StopBits            = USART_StopBits_1;
	uart_struct.USART_Parity              = USART_Parity_No ;
	uart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_struct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	//Инициализируем UART
	USART_Init(UART4, &uart_struct);
	USART_ClearFlag(UART4, USART_FLAG_LBD | USART_FLAG_TC  | USART_FLAG_RXNE );
	//Включаем UART
	USART_Cmd(UART4, ENABLE);

}
*/

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
	//joint[3] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(2), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 5, 2), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 4), 30);//switch at CLOCK


	joint[0] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(3), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 6, 3), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 1), 0);//switch at COUNTERCLOCK
	joint[1] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(4), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 6, 4), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 3), 0);//switch at COUNTERCLOCK
	joint[2] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(5), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 0, 5), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 7), 0);//switch at COUNTERCLOCK
	//joint[3] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(9), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOA, 0, 2, 9), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 9), 0);//switch at COUNTERCLOCK

	//joint[5] = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(8), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOC, 2, 6, 8), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 8), 30);//switch at COUNTERCLOCK

}

void ZobovManipulator::InitLimSwitch() {


	lim_switch[0] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 13),  joint[0], CLOCK);
	lim_switch[1] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 12),  joint[1], COUNTERCLOCK);
	lim_switch[2] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 11),  joint[2], CLOCK);
	//lim_switch[3] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 10),  joint[3], CLOCK);

	//lim_switch[4] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 2),  joint[4], CLOCK);
	//lim_switch[5] = new ZobovManipulatorJointLimitingSwitch(new ZobovSwitchGPIOPort(RCC_AHB1Periph_GPIOB, 1, 1),  joint[5], CLOCK);
}

void ZobovManipulator::InitGraber() {
	graber = new ZobovGrabber(new ZobovGraberGPIOPort(RCC_AHB1Periph_GPIOC, 2, 4));
}

void ZobovManipulator::RotateToStart() {
//	for(uint8_t i = 0; i < 2; ++i) {
//		joint[i]->setDirection(CLOCK);
//		joint[i]->rotate(3600*2);
//	}

	assert(JOINT_CT <= LIM_SWITCH_CT);

	for(uint8_t i = 0; i < JOINT_CT; ++i) {
		joint[i]->setDirectionToZero( (lim_switch[i]->getDir() == CLOCK)?COUNTERCLOCK:CLOCK );
		joint[i]->setDirection( lim_switch[i]->getDir() );
		joint[i]->rotate(3600);
	}
/*
	joint[3]->setDirectionToZero(CLOCK);
	joint[3]->setDirection(CLOCK);
	joint[3]->setSpeed(6000);
	joint[3]->rotate(3600);
*/
	WaitAll();
	/* мешала работать
	for(uint8_t i = 0; i < JOINT_CT; ++i) {
			joint[i]->rotateToZero();
		}

		WaitAll();
	*/
}

void ZobovManipulator::InitNVIC() {
	//NVIC_InitTypeDef nvicStructure;

	/*
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	*/

	/*
	nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	*/

	/*nvicStructure.NVIC_IRQChannel = ADC_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
	nvicStructure.NVIC_IRQChannelSubPriority = 2;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
	ADC_ITConfig(ADC1, ADC_IT_OVR, DISABLE);
	ADC_ITConfig(ADC1, ADC_IT_JEOC, DISABLE);
	*/

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
//
//	RTC_AlarmTypeDef RTC_AlarmStructure;
//
//	/* set alarm time 8:30:0 everyday */
//	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
//	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x08;
//	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x30;
//	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x0;
//	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31; // Nonspecific
//	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
//	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay; // Everyday
//	//RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All; // Everysecond
//	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
//
//	/* Enable Alarm */
//	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
//	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
//	RTC_ClearFlag(RTC_FLAG_ALRAF);
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

void ZobovManipulator::Rotate(uint8_t num, degree deg) {
	assert(num <= JOINT_CT);
	joint[num]->rotate(deg);
}

void ZobovManipulator::Rotate(uint8_t num, degree deg, direction dir, speed spd) {
	assert(num <= JOINT_CT);
	joint[num]->setDirection(dir);
	joint[num]->setSpeed(spd);
	Rotate(num, deg);
}

void ZobovManipulator::Rotate(uint8_t num, degree deg, speed spd) {
	assert(num <= JOINT_CT);
	joint[num]->setSpeed(spd);
	Rotate(num, deg);
}

void ZobovManipulator::Rotate(uint8_t num, degree deg, direction dir) {
	assert(num <= JOINT_CT);
	joint[num]->setDirection(dir);
	Rotate(num, deg);
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

/*
 * P - point to
 * trg_angles - degree to target
 */
/*
array<degree, 6> ZobovManipulatorMathHelper::calcManipulatorRotate(Point P, const array<degree, 3> trg_angles, const array<dimention, 4> d) {
	Point tmpP;
	array<degree, 6> res;
	tmpP.x=P.x-d[3]*cos(trg_angles[1])*cos(trg_angles[0]);
	tmpP.y=P.y-d[3]*cos(trg_angles[1])*sin(trg_angles[0]);
	tmpP.z=P.z-d[3]*sin(trg_angles[1]);

	res[0]=atan(tmpP.x/tmpP.y);

	auto cosRes3 = -1*(tmpP.x*tmpP.x+tmpP.y*tmpP.y+(tmpP.z-d[0])*(tmpP.z-d[0])-d[1]*d[1]-d[2]*d[2])/(2*d[1]*d[2]);

	res[2]=atan(-sqrt(1-cosRes3*cosRes3)/cosRes3);

	auto tmpAngle0 = atan((tmpP.z-d[0])/sqrt(tmpP.x*tmpP.x+tmpP.y*tmpP.y));
	auto tmpAngle1 = atan(d[2]*sin(res[2])/(d[1]-d[2]*cos(res[2])));
	res[1]=tmpAngle0-tmpAngle1;

	tmpAngle0=trg_angles[0]-res[0];
	tmpAngle1=trg_angles[1]-res[1]-res[2];

	res[3] = 0;

	res[4] = res[1] - res[2] + trg_angles[1];
	res[5] = trg_angles[2];

	return res;
}
*/
