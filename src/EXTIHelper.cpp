/*
 * EXTIHelper.cpp
 *
 *  Created on: 6 џэт. 2016 у.
 *      Author: Administrator
 */

#include "EXTIHelper.h"

const uint8_t EXTIHelper::EXTI_PortSource[]={EXTI_PortSourceGPIOA,EXTI_PortSourceGPIOB,EXTI_PortSourceGPIOC, EXTI_PortSourceGPIOD};
const uint8_t EXTIHelper::EXTI_PinSource[]={EXTI_PinSource0, EXTI_PinSource1, EXTI_PinSource2, EXTI_PinSource3, EXTI_PinSource4, EXTI_PinSource5, EXTI_PinSource6, EXTI_PinSource7, EXTI_PinSource8, EXTI_PinSource9, EXTI_PinSource10, EXTI_PinSource11, EXTI_PinSource12, EXTI_PinSource13, EXTI_PinSource14, EXTI_PinSource15};
const uint32_t EXTIHelper::EXTI_Line[]={EXTI_Line0, EXTI_Line1, EXTI_Line2, EXTI_Line3, EXTI_Line4, EXTI_Line5, EXTI_Line6, EXTI_Line7, EXTI_Line8, EXTI_Line9, EXTI_Line10, EXTI_Line11, EXTI_Line12, EXTI_Line13, EXTI_Line14, EXTI_Line15};
const uint8_t EXTIHelper::EXTI_IRQ[]={EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn};

EXTIIRQn_Interface *EXTIHelper::EXTIIQRn[4][16] = {NULL};
char EXTIHelper::EXTIIRQn_LineLetter[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
