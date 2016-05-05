#include <stdlib.h>

#include "ZobovTIM.h"
#include "assert.h"

TIM_TypeDef *ZobovJointTIM::TIMArray[] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13};
IRQn_Type ZobovJointTIM::TIMIRQnArray[] = {TIM1_UP_TIM10_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM5_IRQn, TIM6_DAC_IRQn, TIM7_IRQn, TIM8_UP_TIM13_IRQn, TIM1_BRK_TIM9_IRQn, TIM1_UP_TIM10_IRQn, TIM1_TRG_COM_TIM11_IRQn, TIM8_BRK_TIM12_IRQn, TIM8_UP_TIM13_IRQn};
ZobovJointTIM *ZobovJointTIM::IRQFuncArray[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

const uint32_t ZobovJointTIM::period = (uint32_t)((SystemCoreClock / 7000 ) - 1); //17000

void ZobovEncoderTIM::IRQ() {
	//Stop jount TIM
	Joint->setSpeed(0);
};

ZobovEncoderTIM::ZobovEncoderTIM(TIM_TypeDef *t) : TIM(t) {
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);

	TIM->SMCR &= ~(TIM_SMCR_SMS);
	TIM->SMCR |= (TIM_SMCR_SMS_0);
	TIM->CCMR1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S);
	TIM->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
	TIM->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);

	TIM_Cmd(TIM, ENABLE);
}

ZobovJointTIM::ZobovJointTIM(uint8_t n) : limit(0), num(n-1) {
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIMArray[num], &TIM_TimeBaseStructure);
	/* Very much needed.  Enable Preload register on CCR1. */
	TIM_OC1PreloadConfig(TIMArray[num], TIM_OCPreload_Enable);

	ZobovJointTIM::IRQFuncArray[num+1] = this;

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIMIRQnArray[num];
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	TIM_ClearITPendingBit(TIMArray[num], TIM_IT_Update);
	//TIM_Cmd(TIMArray[num], ENABLE);
}

void ZobovJointTIM::IRQ() {
	assert(Joint != NULL);
	++count;
	//Stop jount TIM
	if (count >= limit) {
		DisableTim();
		unlock();
		Joint->setStatus(IDLE);
	}

};
