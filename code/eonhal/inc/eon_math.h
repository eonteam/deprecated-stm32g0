/**
  ******************************************************************************
  * @file    eon_math.h 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   Header de Math Library
  ******************************************************************************
*/

#ifndef __EON_MATH_H
#define __EON_MATH_H

#include "stdint.h"
#include <stdlib.h>

/** 
 ===============================================================================
             ##### Functions #####
 ===============================================================================
 */

/**
 * @brief Set the seed to generate random numbers
 * 
 * @param {seed} Seed value 
 */
void random_seed(unsigned long seed);
/**
 * @brief Returns random number
 * 
 * @param {howsmall} Minimum number
 * @param {howbig} Maximum number 
 * @return {long} Random number in range (min, max) 
 */
long random_num(long howsmall, long howbig);

/**
 * @brief Map function execute the slope-intercept equation
 * 
 * @param {x} Variable axis X 
 * @param {in_min} Minimum input 
 * @param {in_max} Maximum input 
 * @param {out_min} Minimum output 
 * @param {out_max} Maximum output 
 * @return {long} Output in terms of Input 
 */
long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif
