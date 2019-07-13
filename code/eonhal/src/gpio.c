/**
  ******************************************************************************
  * @file    gpio.c 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   GPIO Functions
  ******************************************************************************
*/

#include "stm32g0xx_ll_bus.h"
#include "pinmap_impl.h"
#include "gpio.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

#define GPIO_OUTPUT_TYPE ((uint32_t)0x00000010)
#define GPIO_MODE ((uint32_t)0x00000003)

/** 
 ===============================================================================
              ##### Variables #####
 ===============================================================================
 */

static uint8_t _af_speed = SPEED_LOW;

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

/**
 * @brief Configure the specified pin
 * 
 * @param {pin} Pin to be configured
 * @param {mode} INPUT, ANALOG, OUTPUT_PP or OUTPUT_OD
 * @param {pull} NOPULL, PULLDOWN or PULLUP 
 * @param {speed} SPEED_LOW, SPEED_MEDIUM, SPEED_HIGH, SPEED_VER_HIGH
 */
void gpio_mode(pin_t pin, mode_t mode, pull_t pull, speed_t speed)
{

	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	// Clock enable
	if (pin_map[pin].GPIOx == GPIOA)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
	}
#ifdef GPIOB
	if (pin_map[pin].GPIOx == GPIOB)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN);
	}
#endif
#ifdef GPIOC
	if (pin_map[pin].GPIOx == GPIOC)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOCEN);
	}
#endif
#ifdef GPIOD
	if (pin_map[pin].GPIOx == GPIOD)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIODEN);
	}
#endif
#ifdef GPIOE
	if (pin_map[pin].GPIOx == GPIOE)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOEEN);
	}
#endif
#ifdef GPIOF
	if (pin_map[pin].GPIOx == GPIOF)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOFEN);
	}
#endif

	LL_GPIO_SetPinMode(pin_map[pin].GPIOx, pin_map[pin].pin, (mode & GPIO_MODE));
	if (mode != INPUT && mode != ANALOG)
	{
		LL_GPIO_SetPinSpeed(pin_map[pin].GPIOx, pin_map[pin].pin, speed);
		LL_GPIO_SetPinOutputType(pin_map[pin].GPIOx, pin_map[pin].pin, ((mode & GPIO_OUTPUT_TYPE) >> 4));
	}
	LL_GPIO_SetPinPull(pin_map[pin].GPIOx, pin_map[pin].pin, pull);
}

/**
 * @brief Configure the Alternate Function of the specified pin
 * 
 * @param {pin}				Pin to be configured
 * @param {afmode} 		AF_PP or AF_OD
 * @param {pull} 			NOPULL, PULLDOWN or PULLUP
 * @param {afx} 			AF_1, AF_2, ... , AF_7
 */
void gpio_modeAF(pin_t pin, mode_t afmode, pull_t pull, uint8_t afx)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	gpio_mode(pin, afmode, pull, _af_speed);

	if (pin_map[pin].pin < LL_GPIO_PIN_8)
		LL_GPIO_SetAFPin_0_7(pin_map[pin].GPIOx, pin_map[pin].pin, afx);
	else
		LL_GPIO_SetAFPin_8_15(pin_map[pin].GPIOx, pin_map[pin].pin, afx);
}

/**
 * @brief Configure the specified pin for UART use
 * 
 * @param {pin}				Pin to be configured
 */
void gpio_modeUART(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	gpio_mode(pin, AF_PP, NOPULL, _af_speed);

	if (pin_map[pin].pin < LL_GPIO_PIN_8)
		LL_GPIO_SetAFPin_0_7(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].uartAF);
	else
		LL_GPIO_SetAFPin_8_15(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].uartAF);
}

/**
 * @brief Configure the specified pin for SPI use
 * 
 * @param {pin}				Pin to be configured
 */
void gpio_modeSPI(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	gpio_mode(pin, AF_PP, NOPULL, _af_speed);

	if (pin_map[pin].pin < LL_GPIO_PIN_8)
		LL_GPIO_SetAFPin_0_7(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].spiAF);
	else
		LL_GPIO_SetAFPin_8_15(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].spiAF);
}

/**
 * @brief Configure the specified pin for I2C use
 * 
 * @param {pin}				Pin to be configured
 */
void gpio_modeI2C(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	gpio_mode(pin, AF_OD, NOPULL, _af_speed);

	if (pin_map[pin].pin < LL_GPIO_PIN_8)
		LL_GPIO_SetAFPin_0_7(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].i2cAF);
	else
		LL_GPIO_SetAFPin_8_15(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].i2cAF);
}

/**
 * @brief Configure the specified pin for PWM use
 * 
 * @param {pin}				Pin to be configured
 */
void gpio_modePWM(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	gpio_mode(pin, AF_PP, NOPULL, _af_speed);

	if (pin_map[pin].pin < LL_GPIO_PIN_8)
		LL_GPIO_SetAFPin_0_7(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].timAF);
	else
		LL_GPIO_SetAFPin_8_15(pin_map[pin].GPIOx, pin_map[pin].pin, pin_map[pin].timAF);
}

/**
 * @brief Configure the speed of the pin configured as Alternate Function
 * 
 * @param {speed}	 SPEED_LOW, SPEED_MEDIUM, SPEED_HIGH, SPEED_VER_HIGH
 */
void gpio_setAFSpeed(uint8_t speed)
{
	_af_speed = speed;
}

/**
 * @brief Write HIGH or LOW to the specified pin
 * 
 * @param {pin} Pin to be written 
 * @param {state} HIGH or LOW 
 */
void gpio_write(pin_t pin, uint8_t state)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	if (state != 0)
		LL_GPIO_SetOutputPin(pin_map[pin].GPIOx, pin_map[pin].pin);
	else
		LL_GPIO_ResetOutputPin(pin_map[pin].GPIOx, pin_map[pin].pin);
}

/**
 * @brief Toggle the specified pin
 * 
 * @param {pin} Pin to be toggled 
 */
void gpio_toggle(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	pin_map[pin].GPIOx->ODR ^= pin_map[pin].pin;
}

/**
 * @brief Read the state of the specified pin
 * 
 * @param {pin} Pin to be read 
 * @return {uint8_t} State of the pin: HIGH or LOW
 */
uint8_t gpio_read(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	return ((pin_map[pin].GPIOx->IDR & pin_map[pin].pin) == 0 ? LOW : HIGH);
}
