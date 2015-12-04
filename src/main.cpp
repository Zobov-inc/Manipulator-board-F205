#include <stdio.h>
#include <stdlib.h>

#include "diag/Trace.h"

#include "main.h"
#include "stm32f2xx.h"
#include "misc.h"
#include "system_stm32f2xx.h"

#include "StdStruct.h"
#include "ZobovManipulator.h"
#include "ZobovManipulatorJointStepperMotorInc.h"

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
    	//ZobovManipulator::tm5->IRQ();
    }
}

extern "C" void TIM6_IRQHandler()
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    	ZobovJointTIM::IRQFuncArray[6]->IRQ();
    	//ZobovManipulator::tm6->IRQ();
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
    	GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

extern "C" void TIM8_BRK_TIM12_IRQHandler()
{
    if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
    {
    	TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
    	//ZobovManipulator::tm6->IRQ();
    	ZobovJointTIM::IRQFuncArray[12]->IRQ();
    	GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}


//int argc, char* argv[]
int main()	{
	//trace_puts("Hello ARM World!");

	ZobovManipulator::InitPorts();
	ZobovManipulator::InitTIM();
	//ZobovManipulator::InitNVIC();

	//ZobovJointTIM *tm3 = new ZobovJointTIM(3);

	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
	//TIM_ClearITPendingBit(TIM12, TIM_IT_Update);

	//ZobovJointTIM *tm10 = new ZobovJointTIM(10);

	//auto joint_3 = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(4), 1, new ZobovGPIOPort{RCC_AHB1Periph_GPIOB, GPIOB, 8}, new ZobovGPIOPort{RCC_AHB1Periph_GPIOA, GPIOA, 11});// ZobovManipulator::tm2);

	auto joint_1 = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(5), 1, new ZobovGPIOPort{RCC_AHB1Periph_GPIOA, GPIOA, 0}, new ZobovGPIOPort{RCC_AHB1Periph_GPIOA, GPIOA, 10});
	auto joint_4 = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(10), 1, new ZobovGPIOPort{RCC_AHB1Periph_GPIOB, GPIOB, 8}, new ZobovGPIOPort{RCC_AHB1Periph_GPIOA, GPIOA, 11});

	//auto new_joint = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(4), 1, new ZobovGPIOPort{RCC_AHB1Periph_GPIOB, GPIOB, 8}, new ZobovGPIOPort{RCC_AHB1Periph_GPIOA, GPIOA, 11});// ZobovManipulator::tm2);

	joint_1->setDirection(CLOCK);
	joint_1->rotate(3600);

	joint_4->setDirection(CLOCK);
	joint_4->rotate(3600);
	volatile unsigned int i = 0;

    //Infinite loop
	while (1)
		++i;

	delete joint_1;
	delete joint_4;
	++i;
	//delete tm10;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
