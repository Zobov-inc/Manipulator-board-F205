//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f2xx.h"

// ----- LED definitions ------------------------------------------------------

// Adjust these definitions for your own board.

// STM3220G-EVAL definitions (the GREEN led, G6, active high)
// (SEGGER J-Link device name: STM32F207IG).

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define BLINK_PORT_NUMBER               (6)
#define BLINK_PIN_NUMBER                (6)
#define BLINK_ACTIVE_LOW                (0)

#define BLINK_GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define BLINK_PIN_MASK(_N)              (1 << (_N))
#define BLINK_RCC_MASKx(_N)             (RCC_AHB1Periph_GPIOA << (_N))
// ----------------------------------------------------------------------------

class BlinkLed
{
public:
  BlinkLed() = default;

  void
  powerUp();

  inline void
  __attribute__((always_inline))
  turnOn()
  {
#if (BLINK_ACTIVE_LOW)
    GPIO_ResetBits(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER));
#else
    GPIO_SetBits(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER));
#endif
  }

  inline void
  __attribute__((always_inline))
  turnOff()
  {
#if (BLINK_ACTIVE_LOW)
    GPIO_SetBits(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER));
#else
    GPIO_ResetBits(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER));
#endif
  }
};

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
