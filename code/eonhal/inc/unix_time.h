/**
  ******************************************************************************
  * @file    unix_time.h 
  * @authors Pablo Fuentes and Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   Header de Unix Time Library
  ******************************************************************************
*/

#ifndef __UNIX_TIME_H
#define __UNIX_TIME_H

#include <stdint.h>

/** 
 ===============================================================================
              ##### Structures #####
 ===============================================================================
 */

typedef struct
{
  uint8_t weekday;
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} LocalTime_t;

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

void unix2time(uint32_t unix, LocalTime_t *t);
uint32_t time2unix(uint8_t day, uint8_t month, uint8_t year, uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif
