/**
  ******************************************************************************
  * @file    exti.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   External Interrupts Functions
  ******************************************************************************
*/

#include "exti.h"
#include "gpio.h"
#include "pinmap_impl.h"

#if !defined(GPIO_GET_INDEX)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA)) ? LL_EXTI_CONFIG_PORTA : ((__GPIOx__) == (GPIOB)) ? LL_EXTI_CONFIG_PORTB : ((__GPIOx__) == (GPIOC)) ? 2U : ((__GPIOx__) == (GPIOH)) ? 5U : 6U)
#endif

#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

/** 
 ===============================================================================
              ##### Private functions #####
 ===============================================================================
 */

static uint32_t get_exticfg_port_enclk(GPIO_TypeDef *GPIOx, uint8_t enclk)
{
#ifdef GPIOA
	if (pin_map[pin].GPIOx == GPIOA)
	{
		if (enclk)
			SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
		return LL_EXTI_CONFIG_PORTA;
	}
#endif
#ifdef GPIOB
	if (pin_map[pin].GPIOx == GPIOB)
	{
		if (enclk)
			SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN);
		return LL_EXTI_CONFIG_PORTB;
	}
#endif
#ifdef GPIOC
	if (pin_map[pin].GPIOx == GPIOC)
	{
		if (enclk)
			SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOCEN);
		return LL_EXTI_CONFIG_PORTC;
	}
#endif
#ifdef GPIOD
	if (pin_map[pin].GPIOx == GPIOD)
	{
		if (enclk)
			SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIODEN);
		return LL_EXTI_CONFIG_PORTD;
	}
#endif
#ifdef GPIOE
	if (pin_map[pin].GPIOx == GPIOE)
	{
		if (enclk)
			SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOEEN);
		return LL_EXTI_CONFIG_PORTE;
	}
#endif
#ifdef GPIOF
	if (pin_map[pin].GPIOx == GPIOF)
	{
		if (enclk)
			SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOFEN);
		return LL_EXTI_CONFIG_PORTF;
	}
#endif
}

static uint32_t get_exticfg_line(uint32_t ll_pin)
{
	if ((ll_pin &
			 (LL_GPIO_PIN_0 || LL_GPIO_PIN_4 ||
				LL_GPIO_PIN_8 || LL_GPIO_PIN_12)) != 0)
	{
		return LL_EXTI_CONFIG_LINE0;
	}
	else if ((ll_pin &
						(LL_GPIO_PIN_1 || LL_GPIO_PIN_5 ||
						 LL_GPIO_PIN_9 || LL_GPIO_PIN_13)) != 0)
	{
		return LL_EXTI_CONFIG_LINE1;
	}
	else if ((ll_pin &
						(LL_GPIO_PIN_2 || LL_GPIO_PIN_6 ||
						 LL_GPIO_PIN_10 || LL_GPIO_PIN_14)) != 0)
	{
		return LL_EXTI_CONFIG_LINE2;
	}
	else if ((ll_pin &
						(LL_GPIO_PIN_3 || LL_GPIO_PIN_7 ||
						 LL_GPIO_PIN_11 || LL_GPIO_PIN_15)) != 0)
	{
		return LL_EXTI_CONFIG_LINE3;
	}
}

static uint32_t GPIO_EXTIConfig(pin_t pin, uint8_t exti_mode, pull_t pull)
{
	uint32_t exticfgPort = 0x00;
	uint32_t exticfgLine = 0x00;
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	exticfgPort = get_exticfg_port_enclk(pin_map[pin].GPIOx, true);
	exticfgLine = get_exticfg_line(pin_map[pin].pin);

	LL_GPIO_SetPinMode(pin_map[pin].GPIOx, pin_map[pin].pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(pin_map[pin].GPIOx, pin_map[pin].pin, pull);

	LL_EXTI_SetEXTISource(exticfgPort, exticfgLine);

	EXTI_InitStruct.Line_0_31 = pin_map[pin].pin;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = exti_mode;
	LL_EXTI_Init(&EXTI_InitStruct);

	if (pin_map[0].pin >= LL_GPIO_PIN_4)
	{
		return EXTI4_15_IRQn;
	}
	else if (pin_map[0].pin >= LL_GPIO_PIN_2)
	{
		return EXTI2_3_IRQn;
	}
	else
	{
		return EXTI0_1_IRQn;
	}
}

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

void exti_attach(pin_t pin, pull_t pull, uint8_t exti_mode)
{
	uint32_t gpio_irqn = 0;

	gpio_irqn = GPIO_EXTIConfig(pin, exti_mode, pull);

	NVIC_SetPriority((IRQn_Type)gpio_irqn, 0);
	NVIC_EnableIRQ((IRQn_Type)gpio_irqn);
}

void exti_attachPriority(pin_t pin, pull_t pull, uint8_t priority, uint8_t exti_mode)
{
	uint32_t gpio_irqn = 0;

	gpio_irqn = GPIO_EXTIConfig(pin, exti_mode, pull);

	NVIC_SetPriority((IRQn_Type)gpio_irqn, priority);
	NVIC_EnableIRQ((IRQn_Type)gpio_irqn);
}

void exti_detach(uint16_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	LL_GPIO_SetPinMode(pin_map[pin].GPIOx, pin_map[pin].pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinSpeed(pin_map[pin].GPIOx, pin_map[pin].pin, LL_GPIO_SPEED_LOW);
	LL_GPIO_SetPinOutputType(pin_map[pin].GPIOx, pin_map[pin].pin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(pin_map[pin].GPIOx, pin_map[pin].pin, LL_GPIO_PULL_NO);
	if (pin_map[pin].pin < LL_GPIO_PIN_8)
		LL_GPIO_SetAFPin_0_7(pin_map[pin].GPIOx, pin_map[pin].pin, AF_0);
	else
		LL_GPIO_SetAFPin_8_15(pin_map[pin].GPIOx, pin_map[pin].pin, AF_0);

	LL_EXTI_DisableRisingTrig_0_31(pin_map[pin].pin);
	LL_EXTI_DisableFallingTrig_0_31(pin_map[pin].pin);
	LL_EXTI_DisableEvent_0_31(pin_map[pin].pin);
	LL_EXTI_DisableIT_0_31(pin_map[pin].pin);
}

void exti_softTrigger(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	LL_EXTI_GenerateSWI_0_31(pin_map[pin].pin)
}

/** 
 ===============================================================================
   ##### Weak functions #####
 ===============================================================================
 */

#if defined(__CC_ARM)
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
__weak void __EXTI0(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
__weak void __EXTI1(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
__weak void __EXTI2(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
__weak void __EXTI3(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
__weak void __EXTI4(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
__weak void __EXTI5(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
__weak void __EXTI6(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
__weak void __EXTI7(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
__weak void __EXTI8(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
__weak void __EXTI9(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
__weak void __EXTI10(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
__weak void __EXTI11(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
__weak void __EXTI12(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
__weak void __EXTI13(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
__weak void __EXTI14(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
__weak void __EXTI15(void)
{
	/* Weak Function: No Modificar */
}
#endif
#elif defined(__GNUC__)
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
void __EXTI0(void) __attribute__((weak));
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
void __EXTI1(void) __attribute__((weak));
#endif
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
void __EXTI2(void) __attribute__((weak));
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
void __EXTI3(void) __attribute__((weak));
#endif
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
void __EXTI4(void) __attribute__((weak));
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
void __EXTI5(void) __attribute__((weak));
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
void __EXTI6(void) __attribute__((weak));
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
void __EXTI7(void) __attribute__((weak));
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
void __EXTI8(void) __attribute__((weak));
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
void __EXTI9(void) __attribute__((weak));
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
void __EXTI10(void) __attribute__((weak));
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
void __EXTI11(void) __attribute__((weak));
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
void __EXTI12(void) __attribute__((weak));
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
void __EXTI13(void) __attribute__((weak));
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
void __EXTI14(void) __attribute__((weak));
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
void __EXTI15(void) __attribute__((weak));
#endif
#endif

/** 
 ===============================================================================
              ##### Interruptions #####
 ===============================================================================
 */

#if defined(USE_EXTI0) || defined(USE_EXTI1) || defined(USE_ALL_EXTI)
void EXTI0_1_IRQHandler(void)
{
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_0) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_0) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_0);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_0);
		__EXTI0();
	}
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_1) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_1) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_1);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_1);
		__EXTI1();
	}
#endif
}
#endif

#if defined(USE_EXTI2) || defined(USE_EXTI3) || defined(USE_ALL_EXTI)
void EXTI2_3_IRQHandler(void)
{
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_2) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_2) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_2);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_2);
		__EXTI2();
	}
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_3) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_3) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_3);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_3);
		__EXTI3();
	}
#endif
}
#endif

#if defined(USE_EXTI4) || defined(USE_EXTI5) || defined(USE_EXTI6) || defined(USE_EXTI7) || defined(USE_EXTI8) || defined(USE_EXTI9) || defined(USE_EXTI10) || defined(USE_EXTI11) || defined(USE_EXTI12) || defined(USE_EXTI13) || defined(USE_EXTI14) || defined(USE_EXTI15) || defined(USE_ALL_EXTI)
void EXTI4_15_IRQHandler(void)
{
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_4) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_4) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_4);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_4);
		__EXTI4();
	}
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_5) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_5) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_5);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_5);
		__EXTI5();
	}
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_6) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_6) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_6);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_6);
		__EXTI6();
	}
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_7) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_7) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_7);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_7);
		__EXTI7();
	}
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_8) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_8) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_8);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_8);
		__EXTI8();
	}
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_9) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_9) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_9);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_9);
		__EXTI9();
	}
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_10) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_10) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_10);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_10);
		__EXTI10();
	}
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_11) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_11) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_11);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_11);
		__EXTI11();
	}
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_12) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_12) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_12);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_12);
		__EXTI12();
	}
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_13) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_13) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_13);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_13);
		__EXTI13();
	}
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_14) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_14) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_14);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_14);
		__EXTI14();
	}
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
	if ((LL_EXTI_IsActiveFallingFlag_0_31(LL_GPIO_PIN_15) != 0) ||
			(LL_EXTI_IsActiveRisingFlag_0_31(LL_GPIO_PIN_15) != 0))
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_GPIO_PIN_15);
		L_EXTI_ClearFallingFlag_0_31(LL_GPIO_PIN_15);
		__EXTI15();
	}
#endif
}
#endif
