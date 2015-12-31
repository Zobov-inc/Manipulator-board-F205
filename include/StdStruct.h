#pragma once

#include "stm32f2xx.h"

#include "ZobovManipulatorJoint.h"

class ZobovManipulatorJoint;

class ZobovGPIOPort {
public:
	static const uint16_t pin[];
	static const uint16_t pinSource[];
	static GPIO_TypeDef *GPIO[];

public:
	uint32_t 		RCC_AHB;
	uint8_t 	 	GPIONum;
	uint8_t 		pinNum;

public:
	ZobovGPIOPort(uint32_t r, uint8_t g, uint8_t p) : RCC_AHB(r), GPIONum(g), pinNum(p) {};
	inline const uint16_t getPin() { return pin[pinNum]; };
	inline GPIO_TypeDef *getGPIO() { return GPIO[GPIONum]; };
	inline const uint16_t getPinSource() { return pinSource[pinNum]; };
	virtual ~ZobovGPIOPort() {};
};

class ZobovManipulatorStepGPIOPort : public ZobovGPIOPort {
private:
	uint8_t afn;

public:
	ZobovManipulatorStepGPIOPort(uint32_t r, uint8_t g, uint8_t p, uint8_t a) : afn(a), ZobovGPIOPort(r, g, p) {
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = pin[pinNum];
		GPIO_Init(GPIO[GPIONum], &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIO[GPIONum], pinSource[p], ZobovManipulatorJoint::GPIO_AF[a]);
	}
};


class ZobovManipulatorDirGPIOPort : public ZobovGPIOPort {
public:
	ZobovManipulatorDirGPIOPort(uint32_t r, uint8_t g, uint8_t p) : ZobovGPIOPort(r, g, p) {
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin = pin[pinNum];
		GPIO_Init(GPIO[GPIONum], &GPIO_InitStructure);
	}
};

class ZobovSwitchGPIOPort : public ZobovGPIOPort {
public:

	ZobovSwitchGPIOPort(uint32_t r, uint8_t g, uint8_t p) : ZobovGPIOPort(r, g, p) {
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = pin[pinNum];
		GPIO_Init(GPIO[GPIONum], &GPIO_InitStructure);
	}
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
