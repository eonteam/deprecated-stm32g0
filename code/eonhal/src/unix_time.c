/**
  ******************************************************************************
  * @file    unix_time.c 
  * @authors Pablo Fuentes and Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   Unix Time Functions
  ******************************************************************************
*/

#include "unix_time.h"

/** 
 ===============================================================================
              ##### Constants #####
 ===============================================================================
 */

/* Days in a month */
static uint8_t RTC_DaysInMonths[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, /* Not leap year */
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}	/* Leap year */
};

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

#define RTC_LEAP_YEAR(year) ((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define RTC_DAYS_IN_YEAR(x) RTC_LEAP_YEAR(x) ? 366 : 365
#define RTC_OFFSET_YEAR 1970 // Start Year for Unix
#define RTC_SECONDS_PER_DAY 86400
#define RTC_SECONDS_PER_HOUR 3600
#define RTC_SECONDS_PER_MINUTE 60

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

uint32_t time2unix(uint8_t day, uint8_t month, uint8_t year, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint32_t t_days = 0, unix_seconds = 0;
	uint16_t i;
	uint16_t t_year = (uint16_t)(year + 2000);

	if (t_year < RTC_OFFSET_YEAR)
		return 0;

	// Days from 1970 until the specified year
	for (i = RTC_OFFSET_YEAR; i < t_year; i++)
	{
		t_days += RTC_DAYS_IN_YEAR(i);
	}

	// Days that has been passed from the specified year, according to the input month
	for (i = 1; i < month; i++)
	{
		t_days += RTC_DaysInMonths[RTC_LEAP_YEAR(t_year)][i - 1];
	}

	// Day starts with 1
	t_days += day - 1;
	unix_seconds = t_days * RTC_SECONDS_PER_DAY;
	unix_seconds += hours * RTC_SECONDS_PER_HOUR;
	unix_seconds += minutes * RTC_SECONDS_PER_MINUTE;
	unix_seconds += seconds;

	return (uint32_t)unix_seconds;
}

void unix2time(uint32_t unix, LocalTime_t *t)
{
	uint16_t year;

	// Get seconds from unix
	t->seconds = unix % 60;

	// Go to minutes
	unix /= 60;
	// Get minutes
	t->minutes = unix % 60;

	// Go to hours
	unix /= 60;
	// Get hours
	t->hours = unix % 24;

	// Go to days
	unix /= 24;
	// Get week day
	// Monday is day one
	t->weekday = (unix + 3) % 7 + 1;

	// Get year
	year = 1970;
	while (1)
	{
		if (RTC_LEAP_YEAR(year))
		{
			if (unix >= 366)
			{
				unix -= 366;
			}
			else
			{
				break;
			}
		}
		else if (unix >= 365)
		{
			unix -= 365;
		}
		else
		{
			break;
		}
		year++;
	}
	// Get year in xx format
	t->year = (uint8_t)(year - 2000);

	// Get month
	for (t->month = 0; t->month < 12; t->month++)
	{
		if (RTC_LEAP_YEAR(year))
		{
			if (unix >= (uint32_t)RTC_DaysInMonths[1][t->month])
			{
				unix -= RTC_DaysInMonths[1][t->month];
			}
			else
			{
				break;
			}
		}
		else if (unix >= (uint32_t)RTC_DaysInMonths[0][t->month])
		{
			unix -= RTC_DaysInMonths[0][t->month];
		}
		else
		{
			break;
		}
	}
	// Get month
	// Month starts with 1
	t->month++;

	// Get date
	// Day starts with 1
	t->day = unix + 1;
}
