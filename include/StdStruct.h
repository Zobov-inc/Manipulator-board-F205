#pragma once

#include "stm32f2xx.h"

#include "ZobovManipulatorJoint.h"

class ZobovManipulatorJoint;

struct ZobovGPIOPort {
	static const uint16_t pin[];
	static const uint16_t pinSource[];

public:
	uint32_t 		RCC_AHB;
	GPIO_TypeDef 	*GPIO;
	uint8_t 		pinNum;

	inline const uint16_t getPin() { return pin[pinNum]; };
	inline const uint16_t getPinSource() { return pinSource[pinNum]; };
};

struct ZobovJointTIM {
private:
	static TIM_TypeDef * TIMArray[];
	static IRQn_Type TIMIRQnArray[];

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	bool _lock = 0;
	uint32_t count = 0;
	uint32_t limit;

public:
	static ZobovJointTIM *IRQFuncArray[];

	static const uint32_t period;

	//number
	const char num;
	ZobovManipulatorJoint *Joint;

	ZobovJointTIM(char n);
	inline void lock() { _lock = true; };
	inline void unlock() { _lock = false; };
	inline bool isLock() { return _lock; };
	TIM_TypeDef * getTIM() { return TIMArray[num]; };
	void EnableTim() {
		count = 0;
		TIM_ITConfig(TIMArray[num], TIM_IT_Update, ENABLE);
		TIM_Cmd(TIMArray[num], ENABLE);
	};
	void DisableTim() {
		TIM_Cmd(TIMArray[num], DISABLE);
		TIM_ITConfig(TIMArray[num], TIM_IT_Update, DISABLE);
	};
	void IRQ();
	void setLimit(uint32_t l) {
		limit = l;
	}
};


struct ZobovEncoderTIM {
private:
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
public:
	TIM_TypeDef *TIM;

public:
	ZobovManipulatorJoint *Joint;

	ZobovEncoderTIM(TIM_TypeDef *t);
	void IRQ();
	void setLimit(uint32_t click) {
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.TIM_Period = click;
		TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	}
};
