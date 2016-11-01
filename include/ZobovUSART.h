/*
 * ZobovUSART.h
 *
 *  Created on: 01 Aug 2016
 *      Author: Administrator
 */

#ifndef ZOBOVUSART_H_
#define ZOBOVUSART_H_

#include <ZobovGPIOPort.h>

#include "stm32f2xx.h"
#include "stm32f2xx_usart.h"

class ZobovUSART {
private:
	uint8_t usart;

public:

	static USART_TypeDef *USART[];
	static const uint8_t GPIO_AF_USART[];

	ZobovUSART(uint8_t, uint8_t, uint8_t);
	char readChar();
	uint32_t readUInt32();
	void writeChar(char);

	virtual ~ZobovUSART() {
		USART_Cmd(USART6, DISABLE);
		// TODO Auto-generated destructor stub
	}
};

#endif /* ZOBOVUSART_H_ */
