/**
  ******************************************************************************
  * @file    pwm.h 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   PWM Library
  ******************************************************************************
*/

#ifndef __PWM_H
#define __PWM_H

#include "pinmap_hal.h"
#include "stm32g0xx_ll_tim.h"

/** 
 ===============================================================================
              ##### Public Functions #####
 ===============================================================================
 */

void pwm_init(TIM_TypeDef *TIMx, uint32_t prescaler, uint32_t period);
void pwm_init1KHz(TIM_TypeDef *TIMx);
void pwm_init500Hz(TIM_TypeDef *TIMx);
void pwm_pinEnable(pin_t pin);
void pwm_write(pin_t pin, uint16_t val);

#endif
