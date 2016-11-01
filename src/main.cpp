#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZobovGPIOPort.h>

#include "diag/Trace.h"

#include "main.h"
#include "stm32f2xx.h"
#include "misc.h"
#include "system_stm32f2xx.h"
#include "math/main_math.h"

#include "ZobovLimitingSwitch.h"
#include "ZobovManipulator.h"
#include "ZobovManipulatorJointStepperMotorInc.h"
#include "ZobovTIM.h"

#define PI 3.14159265358979323846

uint32_t    period;
uint16_t	pulse;

int blink_flag = 0;

extern "C" void TIM1_IRQHandler()
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[1]->IRQ();
    	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
    	//ZobovManipulator::tm2->IRQ();
    }
}

extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[2]->IRQ();
    	//ZobovManipulator::tm2->IRQ();
    }
}

extern "C" void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[3]->IRQ();
    	//ZobovManipulator::tm3->IRQ();
    	//ZobovJointTIM::IRQArray[3]->IRQ();
    }
}

extern "C" void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[4]->IRQ();
    	//ZobovManipulator::tm4->IRQ();
    }
}

extern "C" void TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[5]->IRQ();
    }
}

extern "C" void TIM6_IRQHandler()
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[6]->IRQ();
    }
}


extern "C" void TIM7_IRQHandler()
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[7]->IRQ();
    	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
    	//ZobovManipulator::tm2->IRQ();
    }
}


extern "C" void TIM8_UP_TIM13_IRQHandler()
{
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[8]->IRQ();
    	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
    	//ZobovManipulator::tm2->IRQ();
    }
}


extern "C" void TIM1_BRK_TIM9_IRQHandler()
{
    if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[9]->IRQ();
    	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
    	//ZobovManipulator::tm2->IRQ();
    }
}

extern "C" void TIM1_UP_TIM10_IRQHandler()
{
    if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
    	//ZobovManipulator::tm6->IRQ();
    	ZobovJointTIM::IRQFuncArray[10]->IRQ();
    	GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

extern "C" void TIM1_TRG_COM_TIM11_IRQHandler()
{
    if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
    	//ZobovManipulator::tm6->IRQ();
    	ZobovJointTIM::IRQFuncArray[11]->IRQ();
    	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

extern "C" void TIM8_BRK_TIM12_IRQHandler()
{
    if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
    	//ZobovManipulator::tm6->IRQ();
    	ZobovJointTIM::IRQFuncArray[12]->IRQ();
    	//GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

//int iq = 0;
//int iw = 0;
//int ie = 0;
//int ir[5] = {0};

extern "C" void EXTI0_IRQHandler()
{
	EXTIHelper::CallIRQn(0);
}

extern "C" void EXTI1_IRQHandler()
{
	EXTIHelper::CallIRQn(1);
}

extern "C" void EXTI4_IRQHandler()
{
	EXTIHelper::CallIRQn(4);
}

extern "C" void EXTI9_5_IRQHandler()
{
	for(uint8_t i = 5; i <= 9; ++i) {
		if (EXTI_GetITStatus(EXTIHelper::EXTI_Line[i]) != RESET) {
			/* Do your stuff when PB7 is changed */
			EXTIHelper::CallIRQn(i);
			/* Clear interrupt flag */
			EXTI_ClearITPendingBit(EXTIHelper::EXTI_Line[i]);
		}
	}
}

extern "C" void EXTI15_10_IRQHandler()
{
	for(uint8_t i = 10; i <= 15; ++i) {
		if (EXTI_GetITStatus(EXTIHelper::EXTI_Line[i]) != RESET) {
			/* Do your stuff when PB7 is changed */
			EXTIHelper::CallIRQn(i);
			/* Clear interrupt flag */
			EXTI_ClearITPendingBit(EXTIHelper::EXTI_Line[i]);
		}
	}
}

extern "C" void RTC_Alarm_IRQHandler() {
	ZobovManipulator::disableRTCAlarm();
	ZobovManipulator::releaseTimeLock();
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
}

int main() {
	volatile uint32_t i = 0;

	kinem2_new5_initialize();

	volatile char data;
	volatile uint32_t x,y;
	x = 1;

	typedef array<degree, 2> lim_type;
	array<lim_type, 4> q_lim = {};
	q_lim[0] = {-131, 129};
	q_lim[1] = {-180, 180};
	q_lim[2] = {-180, -114};
	q_lim[3] = {-180, 180};
	array<degree, 4> start_q = {130, 105, -115, -105};
	array<degree, 4> q;
	array<degree, 4> move_q = {};
	array<double, 4> math_q = {};

	ZobovManipulator::Init(2);

	while (true)
	{
		q = start_q;
		move_q = {};
		math_q = {};

		while(!ZobovManipulator::ready);

		do {
			ZobovManipulator::WriteChar(1);
			data = ZobovManipulator::ReadChar();
		} while(data != 1);

		do {
			ZobovManipulator::WriteChar(4);
			data = ZobovManipulator::ReadChar();
		} while(data != 4);


		do {
			ZobovManipulator::WriteChar(2);
			ZobovManipulator::WaitTime(1);
			x = ZobovManipulator::ReadUInt32();
		} while(x < 0 || x > 640);

		do {
			ZobovManipulator::WriteChar(3);
			ZobovManipulator::WaitTime(1);
			y = ZobovManipulator::ReadUInt32();
		} while(y < 0 || y > 480);


		const auto Xc = 200;
//		const auto Yc = 147+5;
		const auto Xpx = 640;
		const auto Ypx = 480;
		const auto Xlmin = 69;
		const auto Xlmax = 364;
		const auto Ylmin = 28;
		const auto Ylmax = 249;
		const auto Xl = Xlmax - Xlmin;
		const auto Yl = Ylmax - Ylmin;
		const auto Xd = Xc - (Xl/2 + Xlmin);
		const auto Ydlin = 5;
		const auto Ydf = Ylmin + Ydlin;
		const auto Ydm = 55;
		volatile double mx = -1.0*Xl/Xpx*x + Xl/2 + Xd;
		volatile double my = 1.0*y*Yl/Ypx + Ydm + Ydf;

		main_kinem2_new5(mx, my, math_q.data());

		bool skip = false;
		for( i = 0; i < 4; ++i) {
			math_q[i] = round(math_q[i]*180/PI);
			if (( math_q[i] >= q_lim[i][1] ) || ( math_q[i] <= q_lim[i][0] ) || (math_q[i] != math_q[i])) {
				skip = true;
				continue;
			}
			move_q[i] = math_q[i] - q[i];
		}
		if (skip) {
			ZobovManipulator::Rollback(q.data(), start_q.data());
			ZobovManipulator::RotateToStart();
			continue;
		}

		i = 6;

		//0: CLOCK = +
		//1: CLOCK = +
		//2: CLOCK = +
		//3: CLOCK = +
		if (move_q[0] != 0) ZobovManipulator::Rotate(0, abs(move_q[0]), (move_q[0] > 0)?CLOCK:COUNTERCLOCK );
		ZobovManipulator::WaitAll();
		q[0] = math_q[0];
		if (move_q[3] != 0) ZobovManipulator::Rotate(3, abs(move_q[3]), (move_q[3] > 0)?COUNTERCLOCK:CLOCK );
		ZobovManipulator::WaitAll();
		q[3] = math_q[3];
		if (move_q[2] != 0) ZobovManipulator::Rotate(2, abs(move_q[2]), (move_q[2] > 0)?CLOCK:COUNTERCLOCK );
		ZobovManipulator::WaitAll();
		q[2] = math_q[2];
		if (move_q[1] != 0) ZobovManipulator::Rotate(1, abs(move_q[1]), (move_q[1] > 0)?COUNTERCLOCK:CLOCK );
		ZobovManipulator::WaitAll();
		q[1] = math_q[1];

		ZobovManipulator::Grab(4);

		ZobovManipulator::RotateToStart();
		ZobovManipulator::UnGrab();

		ZobovManipulator::ready = false;
	}

  kinem2_new5_terminate();

  return 0;
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
