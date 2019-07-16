/**
  ******************************************************************************
  * @file    adc.h 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.0
  * @date    2016
  * @brief   Header de ADC Library
  ******************************************************************************
*/

#ifndef __ADC_H
#define __ADC_H

#include "stm32g0xx_ll_adc.h"
#include "pinmap_hal.h"

/** 
 ===============================================================================
              ##### Public functions #####
 ===============================================================================
 */

/**
 * @brief Set ADC Sample time
 * 
 * @param {ADC_SampleTime} ADC Sample time
 */
void adc_setSampleTime(uint8_t ADC_SampleTime);

/**
 * @brief Read the adc value from the pin specified
 * 
 * @param {pin} Analog pin
 * @return {uint16_t} Values between 0 - 4095
 */
uint16_t adc_readU(pin_t pin);

/**
 * @brief Read the adc normalized value from the pin specified
 * 
 * @param {pin} Analog pin
 * @return {float} Values between 0.00 - 1.00
 */
float adc_readN(pin_t pin);

#endif
