#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZobovGPIOPort.h>

#include "diag/Trace.h"

#include "main.h"
#include "stm32f2xx.h"
#include "misc.h"
#include "system_stm32f2xx.h"

#include "ZobovLimitingSwitch.h"
#include "ZobovManipulator.h"
#include "ZobovManipulatorJointStepperMotorInc.h"
#include "ZobovTIM.h"


uint32_t    period;
uint16_t	pulse;

int blink_flag = 0;

/*
extern "C" void ADC_IRQHandler()
{
	volatile static int i;
	i = 0;

	if (ADC_GetFlagStatus (ADC1, ADC_FLAG_STRT)) {
		ADC_ClearFlag(ADC1, ADC_FLAG_STRT);
		i = 1;
	}

}
*/
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
//	++iq;
	//GPIO_SetBits(GPIOB, GPIO_Pin_0);
	EXTIHelper::CallIRQn(0);
}

extern "C" void EXTI1_IRQHandler()
{
//	++iw;
	EXTIHelper::CallIRQn(1);
}

extern "C" void EXTI4_IRQHandler()
{
//	++ie;
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
		/*
	ZobovManipulator::Init();
	//ToDo: set it
	array<dimention, 4> dim = {0, 0, 0, 0};
	//ToDo: set it
	Point Manipulator = {0, 0, 0};

	//ToDo: set it
	array<degree, 3> target_degree = {30, 60, 0};
	array<degree, 6> res_degree;
	array<direction, 6> res_dir;

	ZobovManipulatorMathHelper::calcManipulatorRotate(Manipulator, target_degree, dim, res_degree);
	for(auto i = 0; i < res_degree.size(); ++i)
		if (res_degree[i] >= 0) {noteps
			res_dir[i] = CLOCK;
		}
		else {
			res_degree[i] *= -1;
			res_dir[i] = COUNTERCLOCK;
		}

	for(auto i = 0; i < 5; ++i)
		ZobovManipulator::Rotate(0, res_degree[i], res_dir[i]);
	ZobovManipulator::WaitAll();
	//ZobovManipulator::Rotate(1, 30, COUNTERCLOCK);
	//ZobovManipulator::WaitAll();
*/


/*
	array<dimention, 4> dim = {111, 150, 135, 45};
	Point p = { 140, 140, 140 };
	//SET TO RAD!!111
	array<degree, 3> trg = { 45, 45, 0 };
	auto res = ZobovManipulatorMathHelper::calcManipulatorRotate(p, trg, dim);
*/
	ZobovManipulator::Init();
	//ZobovManipulator::WaitAll();

	//ZobovManipulator::Rotate(2, 10000, CLOCK);//ok
//	ZobovManipulator::WaitAll();
//	ZobovManipulator::Rotate(1, 1000, CLOCK);//ok
//	ZobovManipulator::WaitAll();
//	ZobovManipulator::Rotate(2, 1000, CLOCK);//ok
//	ZobovManipulator::WaitAll();
//	ZobovManipulator::Rotate(3, 1000, CLOCK);//ok
//	ZobovManipulator::WaitAll();
//	ZobovManipulator::Rotate(0, 10000, COUNTERCLOCK);//ok
//	ZobovManipulator::Rotate(1, 1000, COUNTERCLOCK);//ok
//	ZobovManipulator::Rotate(2, 1000, COUNTERCLOCK);//ok
//	ZobovManipulator::Rotate(3, 1000, COUNTERCLOCK);//ok
//	ZobovManipulator::WaitAll();
//	ZobovManipulator::Grab(3600);
//	ZobovManipulator::Rotate(0, 36, COUNTERCLOCK);//ok
//	ZobovManipulator::Rotate(1, 720, COUNTERCLOCK);//ok
//	ZobovManipulator::Rotate(2, 36, COUNTERCLOCK);//ok
//	ZobovManipulator::Rotate(3, 36, COUNTERCLOCK);//ok
//	ZobovManipulator::WaitAll();
//	ZobovManipulator::UnGrab();
	ZobovManipulator::Rotate(0, 100, CLOCK);
//	int16_t uart_data[2];
//	memset(uart_data, 0, 8);
//	uint16_t led;
volatile uint32_t i = 0;
/*
	for(;;)
	{

		++i;
		ZobovManipulator::Rotate(0, 10, COUNTERCLOCK);
		ZobovManipulator::WaitAll();
	//	ZobovManipulator::Rotate(1, 100, COUNTERCLOCK);
	//  ZobovManipulator::WaitAll();
	//	ZobovManipulator::Rotate(2, 100, COUNTERCLOCK);
	//	ZobovManipulator::WaitAll();
	//	ZobovManipulator::Rotate(3, 100, COUNTERCLOCK);
	//	ZobovManipulator::WaitAll();
	ZobovManipulator::RotateToStart();
	}
	*/
//		i = 0;
//		uart_data[0] = 0;
//		while(true) {
//			if (UART4->SR & USART_SR_RXNE) {
//				if (i++ % 2) {
//					uart_data[0] += UART4->DR << 8;
//					if (uart_data[0] == 1000) {
//						i = 0;
//						break;
//					}
//					else
//						uart_data[0] = 0;
//				}
//				else
//					uart_data[0] += UART4->DR;
//				USART_ClearFlag(UART4, USART_FLAG_LBD | USART_FLAG_TC  | USART_FLAG_RXNE );
//			}
//		}
//		i = 0;
//		memset(uart_data, 0, 8);
//		while(i < 4)
//		{
//			if (UART4->SR & USART_SR_RXNE) {
//				if (i % 2)
//					uart_data[i++/2] += UART4->DR << 8;
//				else
//					uart_data[i++/2] += UART4->DR;
//				USART_ClearFlag(UART4, USART_FLAG_LBD | USART_FLAG_TC  | USART_FLAG_RXNE );
//			}
//		}
//
////		USART_ClearFlag(UART4, USART_FLAG_LBD | USART_FLAG_TC  | USART_FLAG_RXNE );
//		while(!(UART4->SR & USART_SR_TC)); //�������� ���������� �������� ���������� ������
//		UART4->DR = 7;
//		USART_ClearFlag(UART4, USART_FLAG_LBD | USART_FLAG_TC  | USART_FLAG_RXNE );

/* It works. Grab something hold it 5 secs and ungrab.
		GPIO_SetBits(GPIOD, GPIO_Pin_2);
		ZobovManipulator::Grab(5);
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		ZobovManipulator::UnGrab();
		ZobovManipulator::WaitTime(1);
*/


		//ZobovManipulator::Grab(10);
		//ZobovManipulator::RotateToStart();
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);
/*
		ZobovManipulator::Rotate(0, 360, COUNTERCLOCK);//ok
		ZobovManipulator::WaitAll();

		ZobovManipulator::Rotate(1, 360, COUNTERCLOCK);//ok
		ZobovManipulator::WaitAll();

		ZobovManipulator::Rotate(2, 360, COUNTERCLOCK);//ok
		ZobovManipulator::WaitAll();

		ZobovManipulator::Rotate(3, 360, COUNTERCLOCK);//ok
		ZobovManipulator::WaitAll();
*/
//		ZobovManipulator::Rotate(0, 720, COUNTERCLOCK);//ok
//		ZobovManipulator::WaitAll();
//		ZobovManipulator::WaitTime(5);
//		ZobovManipulator::RotateToStart();
//		ZobovManipulator::WaitTime(5);

//		ZobovManipulator::Rotate(0, 90, CLOCK);//ok
//		ZobovManipulator::WaitTime(3);
//		ZobovManipulator::RotateToStart();

//		ZobovManipulator::Rotate(1, 10000, CLOCK);//ok
//		ZobovManipulator::Rotate(2, 10000, CLOCK);//ok
//		ZobovManipulator::Rotate(3, 10000, CLOCK);//ok
//		ZobovManipulator::Rotate(4, 10000, CLOCK);//ok
//		ZobovManipulator::Rotate(5, 10000, CLOCK);//ok
		//ZobovManipulator::Grab(36);
		//ZobovManipulator::WaitAll();


}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
