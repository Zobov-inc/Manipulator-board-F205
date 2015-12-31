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
	ZobovManipulator::InitEXTI();
	ZobovManipulator::InitTIM();
	ZobovManipulator::InitNVIC();

	auto joint_1 = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(5), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 9, 5), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 12));
	auto joint_2 = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(11), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 15, 11), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOC, 2, 8));
	auto joint_3 = new ZobovManipulatorJointStepperMotorInc(new ZobovJointTIM(10), 1, new ZobovManipulatorStepGPIOPort(RCC_AHB1Periph_GPIOB, 1, 8, 10), new ZobovManipulatorDirGPIOPort(RCC_AHB1Periph_GPIOA, 0, 11));

	joint_1->setDirection(CLOCK);
	joint_1->rotate(3600);

	joint_2->setDirection(CLOCK);
	joint_2->rotate(3600);

	joint_3->setDirection(CLOCK);
	joint_3->rotate(3600);

	/*
	volatile uint32_t	j;
	extern volatile uint32_t ADCValues; // <- the results are here
	*/

    //Infinite loop
	for(;;)
	{
		/*
		j = ADCValues;
		if (ADCValues > 0){
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		*/
	}

	delete joint_1;
	delete joint_2;
	delete joint_3;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
