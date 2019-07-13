/**
  ******************************************************************************
  * @file    system_rtc.c 
  * @authors Pablo Fuentes and Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   System RTC Functions
  ******************************************************************************
*/

#include "System.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

/*------ NVIC global Priority set ------*/
#ifndef RTC_PRIORITY
#define RTC_PRIORITY 0x04
#endif

/*--- Sub priority for wakeup trigger --*/
#ifndef RTC_WAKEUP_SUBPRIORITY
#define RTC_WAKEUP_SUBPRIORITY 0x00
#endif

/*--- Sub priority for alarm trigger --*/
#ifndef RTC_ALARM_SUBPRIORITY
#define RTC_ALARM_SUBPRIORITY 0x01
#endif

#ifndef IGNORE
#define IGNORE 255
#endif

// RTC EXTI Lines
#define RTC_EXTI_LINE LL_EXTI_LINE_19
#define RTC_EXTI_LINE_TAMPER LL_EXTI_LINE_21

// Definiciones CHAR2NUM y CHARISNUM
#define RTC_CHAR2NUM(x) ((x) - '0')
#define RTC_CHARISNUM(x) ((x) >= '0' && (x) <= '9')

/** 
 ===============================================================================
							##### Private Functions #####
 ===============================================================================
 */

__STATIC_INLINE void LSI_init(bool resetBKP)
{
	LL_RCC_LSI_Enable();

	// Wait till LSI is ready
	while (LL_RCC_LSI_IsReady() != 1)
	{
	}

	LL_PWR_EnableBkUpAccess();

	if (resetBKP)
	{
		LL_RCC_ForceBackupDomainReset();
		LL_RCC_ReleaseBackupDomainReset();
	}

	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

	LL_RCC_EnableRTC();
}

__STATIC_INLINE void RTC_setWAKEUPTIMER(uint32_t wakeupCounter, uint32_t wakeupClock)
{
	LL_RTC_DisableWriteProtection(RTC); //ok

	if (LL_RTC_WAKEUP_IsEnabled(RTC) != RESET)
	{ //ok
		while (LL_RTC_IsActiveFlag_WUTW(RTC) == SET)
		{
		} //ok
	}

	LL_RTC_WAKEUP_Disable(RTC); //ok

	while (LL_RTC_IsActiveFlag_WUTW(RTC) == RESET)
	{
	} //ok

	LL_RTC_WAKEUP_SetAutoReload(RTC, (uint32_t)wakeupCounter); //ok

	LL_RTC_WAKEUP_SetClock(RTC, (uint32_t)wakeupClock); //ok

	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE); //ok

	LL_RTC_EnableIT_WUT(RTC); //ok

	LL_RTC_WAKEUP_Enable(RTC); //ok

	LL_RTC_EnableWriteProtection(RTC); //ok
}

/** 
 ===============================================================================
							##### Public Functions #####
 ===============================================================================
 */

void rtc_initLsi(bool resetBKP)
{
	LL_RTC_InitTypeDef RTC_InitStruct;

	LSI_init(resetBKP);

	/* Peripheral clock enable */
	LL_RCC_EnableRTC();

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);

	/*Initialize RTC and set the Time and Date */
	RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
	LL_RTC_Init(RTC, &RTC_InitStruct);
}

void rtc_setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	LL_RTC_TimeTypeDef RTC_TimeStruct;

	RTC_TimeStruct.Hours = hours;
	RTC_TimeStruct.Minutes = minutes;
	RTC_TimeStruct.Seconds = seconds;
	LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStruct);
}

void rtc_setDate(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year)
{
	LL_RTC_DateTypeDef RTC_DateStruct;

	RTC_DateStruct.WeekDay = weekday;
	RTC_DateStruct.Day = day;
	RTC_DateStruct.Month = month;
	RTC_DateStruct.Year = year;
	LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_DateStruct);
}

void rtc_setUnix(uint32_t unix)
{
	LL_RTC_TimeTypeDef RTC_TimeStruct;
	LL_RTC_DateTypeDef RTC_DateStruct;
	LocalTime_t t;
	unix2time(unix, &t);

	RTC_TimeStruct.Hours = t.hours;
	RTC_TimeStruct.Minutes = t.minutes;
	RTC_TimeStruct.Seconds = t.seconds;
	LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStruct);

	RTC_DateStruct.WeekDay = t.weekday;
	RTC_DateStruct.Day = t.day;
	RTC_DateStruct.Month = t.month;
	RTC_DateStruct.Year = t.year;
	LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_DateStruct);
}

void rtc_getTime(RTCTime_t *rtc_time)
{
	rtc_time->hours = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
	rtc_time->minutes = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
	rtc_time->seconds = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
}

void rtc_getDate(RTCDate_t *rtc_date)
{
	rtc_date->weekday = LL_RTC_DATE_GetWeekDay(RTC);
	rtc_date->day = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
	rtc_date->month = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
	rtc_date->year = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
}

uint32_t rtc_getUnix(void)
{
	RTCTime_t rtc_time;
	RTCDate_t rtc_date;
	rtc_getTime(&rtc_time);
	rtc_getDate(&rtc_date);
	return time2unix(rtc_date.day, rtc_date.month, rtc_date.year, rtc_time.hours, rtc_time.minutes, rtc_time.seconds);
}

void rtc_setWKUPMillis(uint16_t milliseconds)
{
	uint16_t ms_fix = 0;

	LL_RTC_ClearFlag_WUT(RTC);												 //added ok
	LL_RTC_WAKEUP_Disable(RTC);												 //ok
	LL_RTC_DisableIT_WUT(RTC);

	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_DisableIRQ(RTC_TAMP_IRQn);

	LL_EXTI_DisableIT_0_31(RTC_EXTI_LINE);

	if (milliseconds != 0)
	{
		NVIC_EnableIRQ(RTC_TAMP_IRQn);
		ms_fix = (uint16_t)(((float)2.048 * milliseconds) - 1); // 2048 = 32768 / 16
		RTC_setWAKEUPTIMER(ms_fix, LL_RTC_WAKEUPCLOCK_DIV_16);
	}
}

void rtc_setWKUPSeconds(uint16_t seconds)
{
	LL_RTC_ClearFlag_WUT(RTC);												 //added ok
	LL_RTC_WAKEUP_Disable(RTC);												 //ok
	LL_RTC_DisableIT_WUT(RTC);

	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_DisableIRQ(RTC_TAMP_IRQn);

	LL_EXTI_DisableIT_0_31(RTC_EXTI_LINE);

	if (seconds != 0)
	{
		NVIC_EnableIRQ(RTC_TAMP_IRQn);
		RTC_setWAKEUPTIMER((seconds - 1), LL_RTC_WAKEUPCLOCK_CKSPRE);
	}
}

void rtc_setAlarmA(uint8_t RTC_ALARM_TYPE_X, uint8_t day_weekday, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	LL_RTC_TimeTypeDef rtc_time;
	LL_RTC_AlarmTypeDef alarm;

	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMA_Disable(RTC);
	LL_RTC_DisableIT_ALRA(RTC);
	LL_RTC_ClearFlag_ALRA(RTC);
	LL_RTC_EnableWriteProtection(RTC);

	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);

	rtc_time.Hours = hours;
	rtc_time.Minutes = minutes;
	rtc_time.Seconds = seconds;
	rtc_time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	alarm.AlarmTime = rtc_time;
	if (RTC_ALARM_TYPE_X == RTC_ALARM_TYPE_DATE)
		alarm.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
	else
		alarm.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_WEEKDAY;
	alarm.AlarmDateWeekDay = day_weekday;
	alarm.AlarmMask = LL_RTC_ALMA_MASK_NONE;
	if (day_weekday == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMA_MASK_DATEWEEKDAY;
	if (hours == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMA_MASK_HOURS;
	if (minutes == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMA_MASK_MINUTES;
	if (seconds == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMA_MASK_SECONDS;

	LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &alarm);

	LL_EXTI_DisableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMA_Enable(RTC);
	LL_RTC_EnableIT_ALRA(RTC);
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_EnableWriteProtection(RTC);
}

void rtc_setAlarmB(uint8_t RTC_ALARM_TYPE_X, uint8_t day_weekday, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	LL_RTC_TimeTypeDef rtc_time;
	LL_RTC_AlarmTypeDef alarm;

	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMB_Disable(RTC);
	LL_RTC_DisableIT_ALRB(RTC);
	LL_RTC_ClearFlag_ALRB(RTC);
	LL_RTC_EnableWriteProtection(RTC);

	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);

	rtc_time.Hours = hours;
	rtc_time.Minutes = minutes;
	rtc_time.Seconds = seconds;
	rtc_time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	alarm.AlarmTime = rtc_time;
	if (RTC_ALARM_TYPE_X == RTC_ALARM_TYPE_DATE)
		alarm.AlarmDateWeekDaySel = LL_RTC_ALMB_DATEWEEKDAYSEL_DATE;
	else
		alarm.AlarmDateWeekDaySel = LL_RTC_ALMB_DATEWEEKDAYSEL_WEEKDAY;
	alarm.AlarmDateWeekDay = day_weekday;
	alarm.AlarmMask = LL_RTC_ALMB_MASK_NONE;
	if (day_weekday == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMB_MASK_DATEWEEKDAY;
	if (hours == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMB_MASK_HOURS;
	if (minutes == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMB_MASK_MINUTES;
	if (seconds == IGNORE)
		alarm.AlarmMask |= LL_RTC_ALMB_MASK_SECONDS;

	LL_RTC_ALMB_Init(RTC, LL_RTC_FORMAT_BIN, &alarm);

	LL_EXTI_DisableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMB_Enable(RTC);
	LL_RTC_EnableIT_ALRB(RTC);
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_EnableWriteProtection(RTC);
}

void rtc_setAlarmAAfter(uint32_t seconds)
{
	LocalTime_t localtime;
	LL_RTC_TimeTypeDef rtc_time;
	LL_RTC_AlarmTypeDef alarm;

	uint32_t alarm_unix = rtc_getUnix() + seconds;
	unix2time(alarm_unix, &localtime);

	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMA_Disable(RTC);
	LL_RTC_DisableIT_ALRA(RTC);
	LL_RTC_ClearFlag_ALRA(RTC);
	LL_RTC_EnableWriteProtection(RTC);

	if (seconds == 0)
		return;

	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);

	rtc_time.Hours = localtime.hours;
	rtc_time.Minutes = localtime.minutes;
	rtc_time.Seconds = localtime.seconds;
	rtc_time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	alarm.AlarmTime = rtc_time;
	alarm.AlarmDateWeekDaySel = LL_RTC_ALMA_DATEWEEKDAYSEL_DATE;
	alarm.AlarmDateWeekDay = 1;
	alarm.AlarmMask = LL_RTC_ALMA_MASK_DATEWEEKDAY;

	LL_RTC_ALMA_Init(RTC, LL_RTC_FORMAT_BIN, &alarm);

	LL_EXTI_DisableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMA_Enable(RTC);
	LL_RTC_EnableIT_ALRA(RTC);
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_EnableWriteProtection(RTC);
}

void rtc_setAlarmBAfter(uint32_t seconds)
{
	LocalTime_t localtime;
	LL_RTC_TimeTypeDef rtc_time;
	LL_RTC_AlarmTypeDef alarm;

	uint32_t alarm_unix = rtc_getUnix() + seconds;
	unix2time(alarm_unix, &localtime);

	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMB_Disable(RTC);
	LL_RTC_DisableIT_ALRB(RTC);
	LL_RTC_ClearFlag_ALRB(RTC);
	LL_RTC_EnableWriteProtection(RTC);

	if (seconds == 0)
		return;

	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);

	rtc_time.Hours = localtime.hours;
	rtc_time.Minutes = localtime.minutes;
	rtc_time.Seconds = localtime.seconds;
	rtc_time.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
	alarm.AlarmTime = rtc_time;
	alarm.AlarmDateWeekDaySel = LL_RTC_ALMB_DATEWEEKDAYSEL_DATE;
	alarm.AlarmDateWeekDay = 1;
	alarm.AlarmMask = LL_RTC_ALMB_MASK_DATEWEEKDAY;

	LL_RTC_ALMB_Init(RTC, LL_RTC_FORMAT_BIN, &alarm);

	LL_EXTI_DisableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMB_Enable(RTC);
	LL_RTC_EnableIT_ALRB(RTC);
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE);
	LL_RTC_EnableWriteProtection(RTC);
}

void system_BKPWrite(uint32_t BKP_DRx, uint32_t data)
{
	LL_RTC_BKP_SetRegister(TAMP, BKP_DRx, (uint32_t)data);
}

uint32_t system_BKPRead(uint32_t BKP_DRx)
{
	return (uint32_t)LL_RTC_BKP_GetRegister(TAMP, BKP_DRx);
}

/*void rtc_setTamper1IT(RTCTamper_t *tamper)
{
	uint32_t tmpreg = 0U;
	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);
	if (tamper->Mode != RTC_TAMPER_RISING)
		tamper->Mode = (uint32_t)(RTC_TAMPCR_TAMP1E << 1U);
	if (tamper->EraseBKP != RTC_TAMPER_ERASEBKP_EN)
	{
		tamper->EraseBKP = 0U;
		tamper->EraseBKP |= RTC_TAMPCR_TAMP1NOERASE;
	}
	if (tamper->AutoClearFlag != RTC_TAMPER_AUTOCLRFLG_DIS)
	{
		tamper->AutoClearFlag = 0U;
		tamper->AutoClearFlag |= RTC_TAMPCR_TAMP1MF;
	}
	tmpreg = (uint32_t)((uint32_t)RTC_TAMPCR_TAMP1E | (uint32_t)RTC_TAMPCR_TAMP1IE | (uint32_t)tamper->Mode | (uint32_t)tamper->EraseBKP |
											(uint32_t)tamper->AutoClearFlag | (uint32_t)tamper->SamplingFreq | (uint32_t)RTC_TAMPCR_TAMPPUDIS);
	RTC->TAMPCR = (uint32_t)0x00000000; // clear tamper config register
	RTC->TAMPCR |= tmpreg;
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE_TAMPER);
	LL_EXTI_EnableRisingTrig_0_31(RTC_EXTI_LINE_TAMPER);
	LL_RTC_ClearFlag_TAMP1(RTC);
}

void rtc_setTamper2IT(RTCTamper_t *tamper)
{
	uint32_t tmpreg = 0U;
	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);
	if (tamper->Mode != RTC_TAMPER_RISING)
		tamper->Mode = (uint32_t)(RTC_TAMPCR_TAMP2E << 1U);
	if (tamper->EraseBKP != RTC_TAMPER_ERASEBKP_EN)
	{
		tamper->EraseBKP = 0U;
		tamper->EraseBKP |= RTC_TAMPCR_TAMP2NOERASE;
	}
	if (tamper->AutoClearFlag != RTC_TAMPER_AUTOCLRFLG_DIS)
	{
		tamper->AutoClearFlag = 0U;
		tamper->AutoClearFlag |= RTC_TAMPCR_TAMP2MF;
	}
	tmpreg = (uint32_t)((uint32_t)RTC_TAMPCR_TAMP2E | (uint32_t)RTC_TAMPCR_TAMP2IE | (uint32_t)tamper->Mode | (uint32_t)tamper->EraseBKP |
											(uint32_t)tamper->AutoClearFlag | (uint32_t)tamper->SamplingFreq | (uint32_t)RTC_TAMPCR_TAMPPUDIS);
	RTC->TAMPCR = (uint32_t)0x00000000; // clear tamper config register
	RTC->TAMPCR |= tmpreg;
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE_TAMPER);
	LL_EXTI_EnableRisingTrig_0_31(RTC_EXTI_LINE_TAMPER);
	LL_RTC_ClearFlag_TAMP2(RTC);
}

#if defined(RTC_TAMPCR_TAMP3E)
void rtc_setTamper3IT(RTCTamper_t *tamper)
{
	uint32_t tmpreg = 0U;
	NVIC_SetPriority(RTC_TAMP_IRQn, RTC_PRIORITY);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);
	if (tamper->Mode != RTC_TAMPER_RISING)
		tamper->Mode = (uint32_t)(RTC_TAMPCR_TAMP3E << 1U);
	if (tamper->EraseBKP != RTC_TAMPER_ERASEBKP_EN)
	{
		tamper->EraseBKP = 0U;
		tamper->EraseBKP |= RTC_TAMPCR_TAMP3NOERASE;
	}
	if (tamper->AutoClearFlag != RTC_TAMPER_AUTOCLRFLG_DIS)
	{
		tamper->AutoClearFlag = 0U;
		tamper->AutoClearFlag |= RTC_TAMPCR_TAMP3MF;
	}
	tmpreg = (uint32_t)((uint32_t)RTC_TAMPCR_TAMP3E | (uint32_t)RTC_TAMPCR_TAMP3IE | (uint32_t)tamper->Mode | (uint32_t)tamper->EraseBKP |
											(uint32_t)tamper->AutoClearFlag | (uint32_t)tamper->SamplingFreq | (uint32_t)RTC_TAMPCR_TAMPPUDIS);
	RTC->TAMPCR = (uint32_t)0x00000000; // clear tamper config register
	RTC->TAMPCR |= tmpreg;
	LL_EXTI_EnableIT_0_31(RTC_EXTI_LINE_TAMPER);
	LL_EXTI_EnableRisingTrig_0_31(RTC_EXTI_LINE_TAMPER);
	LL_RTC_ClearFlag_TAMP3(RTC);
}
#endif

void rtc_disableTamper1IT(void)
{
	RTC->TAMPCR &= ((uint32_t)~RTC_TAMPCR_TAMP1E);
	RTC->TAMPCR &= ((uint32_t) ~(RTC_TAMPCR_TAMPIE | RTC_TAMPCR_TAMP1IE));
}

void rtc_disableTamper2IT(void)
{
	RTC->TAMPCR &= ((uint32_t)~RTC_TAMPCR_TAMP2E);
	RTC->TAMPCR &= ((uint32_t) ~(RTC_TAMPCR_TAMPIE | RTC_TAMPCR_TAMP2IE));
}

#if defined(RTC_TAMPCR_TAMP3E)
void rtc_disableTamper3IT(void)
{
	RTC->TAMPCR &= ((uint32_t)~RTC_TAMPCR_TAMP3E);
	RTC->TAMPCR &= ((uint32_t) ~(RTC_TAMPCR_TAMPIE | RTC_TAMPCR_TAMP3IE));
}
#endif*/

/** 
 ===============================================================================
							##### Interrupts #####
 ===============================================================================
 */

#if defined(__CC_ARM)
__weak void __Handler_RTC_WKUP(void)
{
}
#elif defined(__GNUC__)
void __Handler_RTC_WKUP(void) __attribute__((weak));
#endif

#if defined(__CC_ARM)
__weak void __Handler_RTC_ALARMA(void)
{
}
#elif defined(__GNUC__)
void __Handler_RTC_ALARMA(void) __attribute__((weak));
#endif

#if defined(__CC_ARM)
__weak void __Handler_RTC_ALARMB(void)
{
}
#elif defined(__GNUC__)
void __Handler_RTC_ALARMB(void) __attribute__((weak));
#endif

#if defined(__CC_ARM)
__weak void __Handler_RTC_TAMPER1(void)
{
}
#elif defined(__GNUC__)
void __Handler_RTC_TAMPER1(void) __attribute__((weak));
#endif

#if defined(__CC_ARM)
__weak void __Handler_RTC_TAMPER2(void)
{
}
#elif defined(__GNUC__)
void __Handler_RTC_TAMPER2(void) __attribute__((weak));
#endif

#if defined(__CC_ARM)
__weak void __Handler_RTC_TAMPER3(void)
{
}
#elif defined(__GNUC__)
void __Handler_RTC_TAMPER3(void) __attribute__((weak));
#endif

void RTC_TAMP_IRQHandler(void)
{

	if (LL_RTC_IsActiveFlag_WUT(RTC) != RESET)
	{
		__Handler_RTC_WKUP();
		LL_RTC_ClearFlag_WUT(RTC); // Clear the WAKEUPTIMER interrupt pending bit
	}

	if (LL_RTC_IsActiveFlag_ALRA(RTC) != RESET)
	{
		__Handler_RTC_ALARMA();
		LL_RTC_ClearFlag_ALRA(RTC); // Clear the ALARMA interrupt pending bit
	}

	if (LL_RTC_IsActiveFlag_ALRB(RTC) != RESET)
	{
		__Handler_RTC_ALARMB();
		LL_RTC_ClearFlag_ALRB(RTC); // Clear the ALARMB interrupt pending bit
	}

/*#if defined(STM32L063xx) || defined(STM32L062xx) || defined(STM32L061xx) || \
		defined(STM32L053xx) || defined(STM32L052xx) || defined(STM32L051xx) || \
		defined(STM32L083xx) || defined(STM32L082xx) || defined(STM32L081xx) || \
		defined(STM32L073xx) || defined(STM32L072xx) || defined(STM32L071xx) || \
		defined(STM32L031xx) || defined(STM32L041xx)
	if (LL_RTC_IsEnabledIT_TAMP1(RTC) != RESET)
	{
		if (LL_RTC_IsActiveFlag_TAMP1(RTC) != RESET)
		{
			__Handler_RTC_TAMPER1();
			LL_RTC_ClearFlag_TAMP1(RTC);
		}
	}
#endif

	if (LL_RTC_IsEnabledIT_TAMP2(RTC) != RESET)
	{
		if (LL_RTC_IsActiveFlag_TAMP2(RTC) != RESET)
		{
			__Handler_RTC_TAMPER2();
			LL_RTC_ClearFlag_TAMP2(RTC);
		}
	}

#if defined(STM32L083xx) || defined(STM32L082xx) || defined(STM32L081xx) || \
		defined(STM32L073xx) || defined(STM32L072xx) || defined(STM32L071xx) || \
		defined(STM32L031xx) || defined(STM32L041xx) || defined(STM32L011xx) || defined(STM32L021xx)
	if (LL_RTC_IsEnabledIT_TAMP3(RTC) != RESET)
	{
		if (LL_RTC_IsActiveFlag_TAMP3(RTC) != RESET)
		{
			__Handler_RTC_TAMPER3();
			LL_RTC_ClearFlag_TAMP3(RTC);
		}
	}
#endif*/
}
