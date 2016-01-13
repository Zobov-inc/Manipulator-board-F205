#include "stm32f2xx_gpio.h"

#include <stdio.h>
#include <assert.h>

#include <ZobovGPIOPort.h>

const uint16_t ZobovGPIOPort::pin[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
GPIO_TypeDef *ZobovGPIOPort::GPIO[] = {GPIOA, GPIOB, GPIOC, GPIOD};
const uint16_t ZobovGPIOPort::pinSource[] = {GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3, GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7, GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10, GPIO_PinSource11, GPIO_PinSource12, GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource15};
