/*
 * ZobovUSART.cpp
 *
 *  Created on: 01 Aug 2016
 *      Author: Administrator
 */

#include "ZobovUSART.h"
#include <assert.h>

USART_TypeDef *ZobovUSART::USART[] = {USART1, USART2, USART3, UART4, UART5, USART6};
const uint8_t ZobovUSART::GPIO_AF_USART[] = {GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6};


ZobovUSART::ZobovUSART(uint8_t u, uint8_t pinRX, uint8_t pinTX) : usart(u-1) {
	assert(usart<6);
	//Заполняем структуру настройками UARTa
	USART_InitTypeDef uart_struct;
	uart_struct.USART_BaudRate            = 9600;
	uart_struct.USART_WordLength          = USART_WordLength_8b;
	uart_struct.USART_StopBits            = USART_StopBits_1;
	uart_struct.USART_Parity              = USART_Parity_No;
	uart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_struct.USART_Mode                = USART_Mode_Rx|USART_Mode_Tx;
	//Инициализируем UART
	USART_Init(USART[usart], &uart_struct);
//	USART_ClearFlag(UART4, USART_FLAG_LBD | USART_FLAG_TC  | USART_FLAG_RXNE );
	//Включаем UART
	USART_Cmd(USART[usart], ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = ZobovGPIOPort::pin[pinRX]|ZobovGPIOPort::pin[pinTX];
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, ZobovGPIOPort::pinSource[pinTX], GPIO_AF_USART[usart]); //PC6 to TX USART6
	GPIO_PinAFConfig(GPIOC, ZobovGPIOPort::pinSource[pinRX], GPIO_AF_USART[usart]); //PC7 to RX USART6
}

char ZobovUSART::readChar() {
	while(USART_GetFlagStatus(USART[usart], USART_FLAG_RXNE) == RESET);
	char c = (char)USART_ReceiveData(USART[usart]);
//	USART_ClearFlag(USART[usart], USART_FLAG_RXNE);
	return c;
}

uint32_t ZobovUSART::readUInt32() {
	union {
		uint32_t i;
		char c[4];
	} u;
	for(uint8_t j = 0; j < 4; ++j) {
		u.c[j] = readChar();
		writeChar(100);
	}
	return u.i;
}

void ZobovUSART::writeChar(char c) {
	USART_SendData(USART[usart], c);
	while(USART_GetFlagStatus(USART[usart], USART_FLAG_TC) == RESET);
//	USART_ClearFlag(USART[usart], USART_FLAG_TC);
}
