#pragma once

#include <stdlib.h>

#include "stm32f2xx.h"

#include "ZobovManipulatorJoint.h"

extern const uint32_t CoreSystemClock;

class ZobovManipulatorJoint;

class ZobovTIM {
public:
	ZobovManipulatorJoint *Joint;

public:
	ZobovTIM() : Joint(NULL) {};
	virtual void IRQ() = 0;
	virtual ~ZobovTIM() {};
};

class ZobovJointTIM : public ZobovTIM {
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
	const uint8_t num;

	ZobovJointTIM(uint8_t n);
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
	virtual void IRQ();
	void setLimit(uint32_t l) {
		limit = l;
	}
};


class ZobovEncoderTIM : public ZobovTIM {
private:
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
public:
	TIM_TypeDef *TIM;

public:

	ZobovEncoderTIM(TIM_TypeDef *t);
	virtual void IRQ();
	void setLimit(uint32_t click) {
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.TIM_Period = click;
		TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	}
};
