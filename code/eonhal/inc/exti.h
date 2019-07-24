/**
  ******************************************************************************
  * @file    exti.h 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   External Interrupts Library
  ******************************************************************************
*/

#ifndef __EXTI_H
#define __EXTI_H

#include "pinmap_hal.h"
#include "stm32g0xx_ll_exti.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

// Exti Modes
#define MODE_CHANGE LL_EXTI_TRIGGER_RISING_FALLING
#define MODE_RISING LL_EXTI_TRIGGER_RISING
#define MODE_FALLING LL_EXTI_TRIGGER_FALLING

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

void exti_attach(pin_t pin, pull_t pull, uint8_t exti_mode);
void exti_attachPriority(pin_t pin, pull_t pull, uint8_t priority, uint8_t exti_mode);
void exti_detach(uint16_t pin);

void exti_softTrigger(pin_t pin);

// Interruptions
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
#define IRQ_EXTI0() void __EXTI0(void)
IRQ_EXTI0();
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
#define IRQ_EXTI1() void __EXTI1(void)
IRQ_EXTI1();
#endif
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
#define IRQ_EXTI2() void __EXTI2(void)
IRQ_EXTI2();
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
#define IRQ_EXTI3() void __EXTI3(void)
IRQ_EXTI3();
#endif
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
#define IRQ_EXTI4() void __EXTI4(void)
IRQ_EXTI4();
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
#define IRQ_EXTI5() void __EXTI5(void)
IRQ_EXTI5();
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
#define IRQ_EXTI6() void __EXTI6(void)
IRQ_EXTI6();
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
#define IRQ_EXTI7() void __EXTI7(void)
IRQ_EXTI7();
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
#define IRQ_EXTI8() void __EXTI8(void)
IRQ_EXTI8();
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
#define IRQ_EXTI9() void __EXTI9(void)
IRQ_EXTI9();
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
#define IRQ_EXTI10() void __EXTI10(void)
IRQ_EXTI10();
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
#define IRQ_EXTI11() void __EXTI11(void)
IRQ_EXTI11();
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
#define IRQ_EXTI12() void __EXTI12(void)
IRQ_EXTI12();
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
#define IRQ_EXTI13() void __EXTI13(void)
IRQ_EXTI13();
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
#define IRQ_EXTI14() void __EXTI14(void)
IRQ_EXTI14();
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
#define IRQ_EXTI15() void __EXTI15(void)
IRQ_EXTI15();
#endif

#endif
