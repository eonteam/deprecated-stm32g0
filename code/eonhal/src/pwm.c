/**
  ******************************************************************************
  * @file    pwm.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   PWM Functions
  ******************************************************************************
*/

#include "pwm.h"
#include "gpio.h"
#include "tim.h"
#include "pinmap_impl.h"
#include "stm32g0xx_ll_bus.h"

/** 
 ===============================================================================
              ##### Helper functions #####
 ===============================================================================
 */

static void pwm_enableTim(TIM_TypeDef *TIMx)
{
#if defined(TIM1)
	if (TIMx == TIM1)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
		return;
	}
#endif

#if defined(TIM2)
	if (TIMx == TIM2)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
		return;
	}
#endif

#if defined(TIM3)
	if (TIMx == TIM3)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
		return;
	}
#endif

#if defined(TIM14)
	if (TIMx == TIM14)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM14);
		return;
	}
#endif

#if defined(TIM15)
	if (TIMx == TIM15)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM15);
		return;
	}
#endif

#if defined(TIM16)
	if (TIMx == TIM16)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);
		return;
	}
#endif

#if defined(TIM17)
	if (TIMx == TIM17)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM17);
		return;
	}
#endif

#if defined(TIM6)
	if (TIMx == TIM6)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
		return;
	}
#endif

#if defined(TIM7)
	if (TIMx == TIM7)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);
		return;
	}
#endif
}

/** 
 ===============================================================================
              ##### Public Functions #####
 ===============================================================================
 */

void pwm_init(TIM_TypeDef *TIMx, uint32_t prescaler, uint32_t period)
{
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};
	LL_TIM_InitTypeDef pwm_time_base;

	pwm_enableTim(TIMx);
	pwm_time_base.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	pwm_time_base.CounterMode = LL_TIM_COUNTERMODE_UP;
	pwm_time_base.Autoreload = period - 1;
	pwm_time_base.Prescaler = prescaler - 1;
	LL_TIM_Init(TIMx, &pwm_time_base);
	LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_EnableARRPreload(TIMx);

	TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_ENABLE;
	TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_ENABLE;
	TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
	TIM_BDTRInitStruct.DeadTime = 0;
	TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
	TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
	TIM_BDTRInitStruct.BreakFilter = LL_TIM_BREAK_FILTER_FDIV1;
	TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_ENABLE;
	LL_TIM_BDTR_Init(TIMx, &TIM_BDTRInitStruct);

	LL_TIM_EnableCounter(TIMx);
}

void pwm_init1KHz(TIM_TypeDef *TIMx)
{
	uint32_t timer_source_freq;
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};
	LL_TIM_InitTypeDef pwm_time_base;

	timer_source_freq = tim_getSrcClk(TIMx);

	pwm_enableTim(TIMx);
	pwm_time_base.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	pwm_time_base.CounterMode = LL_TIM_COUNTERMODE_UP;
	pwm_time_base.Autoreload = 999; /* 1000 - 1 */
	pwm_time_base.Prescaler = (timer_source_freq / 1000000) - 1;
	LL_TIM_Init(TIMx, &pwm_time_base);
	LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_EnableARRPreload(TIMx);

	TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_ENABLE;
	TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_ENABLE;
	TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
	TIM_BDTRInitStruct.DeadTime = 0;
	TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
	TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
	TIM_BDTRInitStruct.BreakFilter = LL_TIM_BREAK_FILTER_FDIV1;
	TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_ENABLE;
	LL_TIM_BDTR_Init(TIMx, &TIM_BDTRInitStruct);

	LL_TIM_EnableCounter(TIMx);
}

void pwm_init500Hz(TIM_TypeDef *TIMx)
{
	uint32_t timer_source_freq;
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};
	LL_TIM_InitTypeDef pwm_time_base;

	timer_source_freq = tim_getSrcClk(TIMx);

	pwm_enableTim(TIMx);
	pwm_time_base.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	pwm_time_base.CounterMode = LL_TIM_COUNTERMODE_UP;
	pwm_time_base.Autoreload = 999; /* 1000 - 1 */
	pwm_time_base.Prescaler = (2 * timer_source_freq / 1000000) - 1;
	LL_TIM_Init(TIMx, &pwm_time_base);
	LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_EnableARRPreload(TIMx);

	TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_ENABLE;
	TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_ENABLE;
	TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
	TIM_BDTRInitStruct.DeadTime = 0;
	TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
	TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
	TIM_BDTRInitStruct.BreakFilter = LL_TIM_BREAK_FILTER_FDIV1;
	TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_ENABLE;
	LL_TIM_BDTR_Init(TIMx, &TIM_BDTRInitStruct);

	LL_TIM_EnableCounter(TIMx);
}

/** 
 ===============================================================================
              ##### Pin enable function #####
 ===============================================================================
 */

void pwm_pinEnable(pin_t pin)
{
	LL_TIM_OC_InitTypeDef pwm_output_compare = {0};
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	gpio_modePWM(pin);
	pwm_output_compare.OCMode = LL_TIM_OCMODE_PWM1;
	pwm_output_compare.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	pwm_output_compare.OCState = LL_TIM_OCSTATE_ENABLE;
	pwm_output_compare.OCNState = LL_TIM_OCSTATE_DISABLE;
	pwm_output_compare.CompareValue = 0;
	LL_TIM_OC_Init(pin_map[pin].TIMx, pin_map[pin].timerCh, &pwm_output_compare);
	LL_TIM_OC_DisableFast(pin_map[pin].TIMx, pin_map[pin].timerCh);
	LL_TIM_OC_EnablePreload(pin_map[pin].TIMx, pin_map[pin].timerCh);
	LL_TIM_CC_EnableChannel(pin_map[pin].TIMx, pin_map[pin].timerCh);
}

/** 
 ===============================================================================
              ##### Write function #####
 ===============================================================================
 */

void pwm_write(pin_t pin, uint16_t val)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	if (val > pin_map[pin].TIMx->ARR)
		val = pin_map[pin].TIMx->ARR;
	if (pin_map[pin].timerCh == LL_TIM_CHANNEL_CH1)
		pin_map[pin].TIMx->CCR1 = val;
	else if (pin_map[pin].timerCh == LL_TIM_CHANNEL_CH2)
		pin_map[pin].TIMx->CCR2 = val;
	else if (pin_map[pin].timerCh == LL_TIM_CHANNEL_CH3)
		pin_map[pin].TIMx->CCR3 = val;
	else if (pin_map[pin].timerCh == LL_TIM_CHANNEL_CH4)
		pin_map[pin].TIMx->CCR4 = val;
}
