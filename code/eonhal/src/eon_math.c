/**
  ******************************************************************************
  * @file    eon_math.c 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   Math Functions
  ******************************************************************************
*/

#include "eon_math.h"

/** 
 ===============================================================================
              ##### FUNCIONES #####
 ===============================================================================
 */

void random_seed(unsigned long seed)
{
  if (seed != 0)
  {
    srand(seed);
  }
}

long random_num(long howsmall, long howbig)
{
  long diff;
  if (howsmall >= howbig)
  {
    return howsmall;
  }
  diff = howbig - howsmall;
  return (rand() % diff) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
