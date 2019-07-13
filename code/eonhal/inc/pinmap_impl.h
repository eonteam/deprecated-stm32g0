/**
 ******************************************************************************
 * @file    pinmap_impl.h
 * @version V1.0.1
 * @date    2019
 * @brief   Implementation details of pins on STM32
 ******************************************************************************
 */

#ifndef PINMAP_IMPL_H
#define PINMAP_IMPL_H

#include "pinmap_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /* Estructura Pin Map -------------------------------------------------------*/
  typedef struct STM32_Pin_Info
  {
    GPIO_TypeDef *GPIOx;
    uint16_t pin;   // Maximum is always 0x00008000, so uint16_t is enough
    uint32_t adcCh; // must be uint32_t
    TIM_TypeDef *TIMx;
    uint16_t timerCh; // Maximum is always 0x00001000, so uint16_t is enough
    uint8_t timAF;    // Maximum AF is always 0x0000007, so uint8_t is enough
    uint8_t spiAF;
    uint8_t i2cAF;
    uint8_t uartAF;
  } STM32_Pin_Info;

  STM32_Pin_Info *HAL_Pin_Map(void);

#define ADC_CHANNEL_NONE ((uint8_t)(0xFFFFFFFF))
#define TIM_CHANNEL_NONE ((uint8_t)(0xFFFF))

#ifdef __cplusplus
}
#endif

#endif /* PINMAP_IMPL_H */
