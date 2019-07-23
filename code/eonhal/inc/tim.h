/**
  ******************************************************************************
  * @file    tim.h 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   Timer Library
  ******************************************************************************
*/

#ifndef __TIM_H
#define __TIM_H

#include "stm32g0xx_ll_tim.h"

/** 
 ===============================================================================
              ##### Type #####
 ===============================================================================
 */

typedef volatile uint32_t tick_t;

/** 
 ===============================================================================
              ##### Structure #####
 ===============================================================================
 */

typedef struct
{
  uint32_t period;
  uint32_t prescaler;
} timebase_t;

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

#define ___TIM_GET_IT_UPD(__TIMX__) ((LL_TIM_ReadReg(__TIMX__, DIER) & LL_TIM_DIER_UIE) == LL_TIM_DIER_UIE)
#define ___TIM_GET_FLAG_UPD(__TIMX__) ((LL_TIM_ReadReg(__TIMX__, SR) & LL_TIM_SR_UIF) == LL_TIM_SR_UIF)

#ifdef TIM1
#define IRQ_TIM1()                                                              \
  void __shadow_tim1(void);                                                     \
  void TIM1_BRK_UP_TRG_COM_IRQHandler(void)                                     \
  {                                                                             \
    if ((___TIM_GET_IT_UPD(TIM1) != RESET) && (___TIM_GET_FLAG_UPD(TIM1) == 1)) \
    {                                                                           \
      LL_TIM_ClearFlag_UPDATE(TIM1);                                            \
      __shadow_tim1();                                                          \
    }                                                                           \
  }                                                                             \
  void __shadow_tim1(void)
#endif

#ifdef TIM2
#define IRQ_TIM2()                                                              \
  void __shadow_tim2(void);                                                     \
  void TIM2_IRQHandler(void)                                                    \
  {                                                                             \
    if ((___TIM_GET_IT_UPD(TIM2) != RESET) && (___TIM_GET_FLAG_UPD(TIM2) == 1)) \
    {                                                                           \
      LL_TIM_ClearFlag_UPDATE(TIM2);                                            \
      __shadow_tim2();                                                          \
    }                                                                           \
  }                                                                             \
  void __shadow_tim2(void)
#endif

#ifdef TIM3
#define IRQ_TIM3()                                                              \
  void __shadow_tim3(void);                                                     \
  void TIM3_IRQHandler(void)                                                    \
  {                                                                             \
    if ((___TIM_GET_IT_UPD(TIM3) != RESET) && (___TIM_GET_FLAG_UPD(TIM3) == 1)) \
    {                                                                           \
      LL_TIM_ClearFlag_UPDATE(TIM3);                                            \
      __shadow_tim3();                                                          \
    }                                                                           \
  }                                                                             \
  void __shadow_tim3(void)
#endif

#ifdef TIM14
#define IRQ_TIM14()                                                               \
  void __shadow_tim14(void);                                                      \
  void TIM14_IRQHandler(void)                                                     \
  {                                                                               \
    if ((___TIM_GET_IT_UPD(TIM14) != RESET) && (___TIM_GET_FLAG_UPD(TIM14) == 1)) \
    {                                                                             \
      LL_TIM_ClearFlag_UPDATE(TIM14);                                             \
      __shadow_tim14();                                                           \
    }                                                                             \
  }                                                                               \
  void __shadow_tim14(void)
#endif

#ifdef TIM15
#define IRQ_TIM15()                                                               \
  void __shadow_tim15(void);                                                      \
  void TIM15_IRQHandler(void)                                                     \
  {                                                                               \
    if ((___TIM_GET_IT_UPD(TIM15) != RESET) && (___TIM_GET_FLAG_UPD(TIM15) == 1)) \
    {                                                                             \
      LL_TIM_ClearFlag_UPDATE(TIM15);                                             \
      __shadow_tim15();                                                           \
    }                                                                             \
  }                                                                               \
  void __shadow_tim15()
#endif

#ifdef TIM16
#define IRQ_TIM16()                                                               \
  void __shadow_tim16(void);                                                      \
  void TIM16_IRQHandler(void)                                                     \
  {                                                                               \
    if ((___TIM_GET_IT_UPD(TIM16) != RESET) && (___TIM_GET_FLAG_UPD(TIM16) == 1)) \
    {                                                                             \
      LL_TIM_ClearFlag_UPDATE(TIM16);                                             \
      __shadow_tim16();                                                           \
    }                                                                             \
  }                                                                               \
  void __shadow_tim16(void)
#endif

#ifdef TIM17
#define IRQ_TIM17()                                                               \
  void __shadow_tim17(void);                                                      \
  void TIM17_IRQHandler(void)                                                     \
  {                                                                               \
    if ((___TIM_GET_IT_UPD(TIM17) != RESET) && (___TIM_GET_FLAG_UPD(TIM17) == 1)) \
    {                                                                             \
      LL_TIM_ClearFlag_UPDATE(TIM17);                                             \
      __shadow_tim17();                                                           \
    }                                                                             \
  }                                                                               \
  void __shadow_tim17(void)
#endif

#ifdef TIM6
#define IRQ_TIM6()                                                              \
  void __shadow_tim6(void);                                                     \
  void TIM6_IRQHandler(void)                                                    \
  {                                                                             \
    if ((___TIM_GET_IT_UPD(TIM6) != RESET) && (___TIM_GET_FLAG_UPD(TIM6) == 1)) \
    {                                                                           \
      LL_TIM_ClearFlag_UPDATE(TIM6);                                            \
      __shadow_tim6();                                                          \
    }                                                                           \
  }                                                                             \
  void __shadow_tim6(void)
#endif

#ifdef TIM7
#define IRQ_TIM7()                                                              \
  void __shadow_tim7(void);                                                     \
  void TIM7_IRQHandler(void)                                                    \
  {                                                                             \
    if ((___TIM_GET_IT_UPD(TIM7) != RESET) && (___TIM_GET_FLAG_UPD(TIM7) == 1)) \
    {                                                                           \
      LL_TIM_ClearFlag_UPDATE(TIM7);                                            \
      __shadow_tim7();                                                          \
    }                                                                           \
  }                                                                             \
  void __shadow_tim7(void)
#endif

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

/* Timer Clocks */
uint8_t tim_clkEnableAndGetIRQn(TIM_TypeDef *TIMx);
uint32_t tim_getSrcClk(TIM_TypeDef *TIMx);

/* Period and Prescalers from desired frequency, return timer frequency clock */
uint32_t tim_getMinPrescalerAndMaxPeriod(timebase_t *parameter, TIM_TypeDef *TIMx, uint32_t desired_frecuency);

/* Funciones Timer Interrupt */
void tim_interrupt(TIM_TypeDef *TIMx, uint32_t prescaler, uint32_t period);
void tim_interruptMs(TIM_TypeDef *TIMx, uint32_t ms);

#endif
