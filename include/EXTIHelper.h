/*
 * EXTIHelper.h
 *
 *  Created on: 6 џэт. 2016 у.
 *      Author: Administrator
 */

#ifndef EXTIHELPER_H_
#define EXTIHELPER_H_

#include "stdlib.h"
#include "assert.h"

#include "stm32f2xx_exti.h"

//interfacetypedef void(*FunctionPointer)();


class EXTIIRQn_Interface {
public:
	EXTIIRQn_Interface() {};
	virtual void IRQn() = 0;
	virtual ~EXTIIRQn_Interface() {};
};

class EXTIHelper {
public:
	static const uint8_t EXTI_PortSource[];
	static const uint8_t EXTI_PinSource[];
	static const uint8_t EXTI_IRQ[];
	static const uint32_t EXTI_Line[];

	static EXTIIRQn_Interface *EXTIIQRn[4][16];
	static char EXTIIRQn_LineLetter[16];

public:
	EXTIHelper() {
		// TODO Auto-generated constructor stub

	}

	static void SetLineLetter(uint8_t pin, uint8_t num) {
		assert(EXTIIRQn_LineLetter[pin] == -1);
		EXTIIRQn_LineLetter[pin] = num;
	}

	static void SetEXTIIRQn(uint8_t pin, uint8_t num, EXTIIRQn_Interface* interface) {
		assert(EXTIIQRn[num][pin] == NULL);
		EXTIIQRn[num][pin] = interface;
	}

	static void CallIRQn(uint8_t pin) {
		EXTI_ClearFlag(EXTI_Line[pin]);
		assert(EXTIIQRn[EXTIIRQn_LineLetter[pin]][pin] != NULL);
		EXTIIQRn[EXTIIRQn_LineLetter[pin]][pin]->IRQn();
	}

	virtual ~EXTIHelper() {
		// TODO Auto-generated destructor stub
	}
};
#endif /* EXTIHELPER_H_ */
