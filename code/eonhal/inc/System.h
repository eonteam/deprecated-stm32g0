/**
 ******************************************************************************
 * @file    System.h
 * @version V1.0.1
 * @date    2019
 * @brief  	STM32 System
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_H
#define __SYSTEM_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32g0xx.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_rtc.h"
#include "stm32g0xx_ll_exti.h"
#include "unix_time.h"
#include "pinmap_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/* Typedef Void Function **********************/
	typedef void (*voidFuncPtr)(void);

/* Redefinitions ******************************/
#define delay LL_mDelay
#define delay_ms LL_mDelay
#define system_softReset NVIC_SystemReset

	/* Millis *************************************/
	uint32_t millis(void);

	/* Clock Functions ***************************/
	void CLOCK_HSI_64MHZ(void);
	void CLOCK_HSI_32MHZ(void);
	void CLOCK_HSI_16MHZ(void);
	void CLOCK_HSI_8MHZ(void);
	void CLOCK_HSI_4MHZ(void);
	void CLOCK_HSI_2MHZ(void);
	void clock_init(void (*clockFunc)(void)); // Function defined in "system_lowpower_l0.c"

/* System RTC Functions **********************/
//Definitions
#define MIN_TO_SEC(__x__) ((__x__)*60)
#define HOUR_TO_SEC(__x__) ((__x__)*60 * 60)
#define RTC_MONDAY LL_RTC_WEEKDAY_MONDAY
#define RTC_TUESDAY LL_RTC_WEEKDAY_TUESDAY
#define RTC_WEDNESDAY LL_RTC_WEEKDAY_WEDNESDAY
#define RTC_THURSDAY LL_RTC_WEEKDAY_THURSDAY
#define RTC_FRIDAY LL_RTC_WEEKDAY_FRIDAY
#define RTC_SATURDAY LL_RTC_WEEKDAY_SATURDAY
#define RTC_SUNDAY LL_RTC_WEEKDAY_SUNDAY
#define RTC_ALARM_TYPE_DATE 0
#define RTC_ALARM_TYPE_WEEKDAY 1
// RTC Tamper definitions
#define RTC_TAMPER_RISING ((uint32_t)0x00000000U)
#define RTC_TAMPER_FALLING ((uint32_t)0x00000002U)
#define RTC_TAMPER_ERASEBKP_EN ((uint32_t)0x00000000U)
#define RTC_TAMPER_ERASEBKP_DIS ((uint32_t)0x00020000U)
#define RTC_TAMPER_AUTOCLRFLG_EN ((uint32_t)0x00040000U)
#define RTC_TAMPER_AUTOCLRFLG_DIS ((uint32_t)0x00000000U)
#define RTC_TAMPER_SAMPLINGFREQ_DIV32768 ((uint32_t)0x00000000U) //  frequency at which each of the RTC_TAMPx inputs are sampled
#define RTC_TAMPER_SAMPLINGFREQ_DIV16384 RTC_TAMPCR_TAMPFREQ_0
#define RTC_TAMPER_SAMPLINGFREQ_DIV8192 RTC_TAMPCR_TAMPFREQ_1
#define RTC_TAMPER_SAMPLINGFREQ_DIV4096 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_0 | RTC_TAMPCR_TAMPFREQ_1))
#define RTC_TAMPER_SAMPLINGFREQ_DIV2048 RTC_TAMPCR_TAMPFREQ_2
#define RTC_TAMPER_SAMPLINGFREQ_DIV1024 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_0 | RTC_TAMPCR_TAMPFREQ_2))
#define RTC_TAMPER_SAMPLINGFREQ_DIV512 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_1 | RTC_TAMPCR_TAMPFREQ_2))
#define RTC_TAMPER_SAMPLINGFREQ_DIV256 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_0 | RTC_TAMPCR_TAMPFREQ_1 | RTC_TAMPCR_TAMPFREQ_2))
	typedef struct
	{
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
	} RTCTime_t;
	typedef struct
	{
		uint8_t weekday;
		uint8_t day;
		uint8_t month;
		uint8_t year;
	} RTCDate_t;
	typedef struct
	{
		uint32_t Mode;
		uint32_t EraseBKP;
		uint32_t AutoClearFlag;
		uint32_t SamplingFreq;
	} RTCTamper_t;
	// Functions defined in "system_rtc_l0.c"
	void rtc_initLsi(bool resetBKP);
	void rtc_initLse(bool resetBKP);
	void rtc_setTime(uint8_t hour, uint8_t min, uint8_t sec);
	void rtc_setDate(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year);
	void rtc_setUnix(uint32_t unix);
	void rtc_getTime(RTCTime_t *rtc_time);
	void rtc_getDate(RTCDate_t *rtc_date);
	uint32_t rtc_getUnix(void);
	// RTC Interrupts
	void rtc_setWKUPMillis(uint16_t milliseconds); //put 0 to disable
	void rtc_setWKUPSeconds(uint16_t seconds);		 //put 0 to disable
	void rtc_setAlarmAAfter(uint32_t seconds);		 //put 0 to disable
	void rtc_setAlarmBAfter(uint32_t seconds);		 //put 0 to disable
	// Just one tamper per application
	void rtc_setTamper1IT(RTCTamper_t *tamper);
	void rtc_setTamper2IT(RTCTamper_t *tamper);
	void rtc_setTamper3IT(RTCTamper_t *tamper);
	void rtc_disableTamper1IT(void);
	void rtc_disableTamper2IT(void);
	void rtc_disableTamper3IT(void);
	void rtc_setAlarmA(uint8_t RTC_ALARM_TYPE_X, uint8_t day_weekday, uint8_t hours, uint8_t minutes, uint8_t seconds);
	void rtc_setAlarmB(uint8_t RTC_ALARM_TYPE_X, uint8_t day_weekday, uint8_t hours, uint8_t minutes, uint8_t seconds);
// Handler INT Functions
#define IRQ_RTC_WKUP() void __Handler_RTC_WKUP(void)
#define IRQ_RTC_ALARMA() void __Handler_RTC_ALARMA(void)
#define IRQ_RTC_ALARMB() void __Handler_RTC_ALARMB(void)
#define IRQ_RTC_TAMPER1() void __Handler_RTC_TAMPER1(void)
#define IRQ_RTC_TAMPER2() void __Handler_RTC_TAMPER2(void)
#define IRQ_RTC_TAMPER3() void __Handler_RTC_TAMPER3(void)
	IRQ_RTC_WKUP();
	IRQ_RTC_ALARMA();
	IRQ_RTC_ALARMB();
	IRQ_RTC_TAMPER1();
	IRQ_RTC_TAMPER2();
	IRQ_RTC_TAMPER3();

/* System Backup (BKP) Registers Functions **********************/
// Definitions
#define BKP_DR0 LL_RTC_BKP_DR0
#define BKP_DR1 LL_RTC_BKP_DR1
#define BKP_DR2 LL_RTC_BKP_DR2
#define BKP_DR3 LL_RTC_BKP_DR3
#define BKP_DR4 LL_RTC_BKP_DR4
	// Functions defined in "system_rtc_l0.c"
	// This functions required System_RTC_initLSI first
	void system_BKPWrite(uint32_t BKP_DRx, uint32_t data);
	uint32_t system_BKPRead(uint32_t BKP_DRx);

/* System Low Power Modes Functions **********************/
// Definitions
#if defined(PA0)
#define WAKEUP_PIN_PA0 LL_PWR_WAKEUP_PIN1
#endif
#if defined(PC13)
#define WAKEUP_PIN_PC13 LL_PWR_WAKEUP_PIN2
#elif defined(PA4)
#define WAKEUP_PIN_PA4 LL_PWR_WAKEUP_PIN2
#endif
#if defined(PA2)
#define WAKEUP_PIN_PA2 LL_PWR_WAKEUP_PIN4
#endif
#if defined(PC5)
#define WAKEUP_PIN_PC5 LL_PWR_WAKEUP_PIN5
#endif
#if defined(PB5)
#define WAKEUP_PIN_PB5 LL_PWR_WAKEUP_PIN6
#endif
	// Functions defined in "system_lowpower_l0.c"
	void system_turnOffAllGPIOs(void);
	// This functions required rtc_initLsi first
	void system_sleepSeconds(uint32_t seconds);
	void system_sleepMillis(uint32_t milliseconds);
	void system_sleepLPSeconds(uint32_t seconds);
	void system_sleepLPMillis(uint32_t milliseconds);
	void system_stop0Seconds(uint32_t seconds);
	void system_stop0Millis(uint32_t milliseconds);
	void system_stop0UntilInterrupt(void); // This function doesn't required rtc
	void system_stop1Seconds(uint32_t seconds);
	void system_stop1Millis(uint32_t milliseconds);
	void system_stop1UntilInterrupt(void); // This function doesn't required rtc
	void system_standby(void);
	void system_standbySeconds(uint32_t seconds);
	void system_standbyUntilWakeUpPin(uint32_t WAKEUP_PIN_x, uint8_t polarity); // This function doesn't required rtc
#if defined(PWR_CR3_RRS)
	void system_standbySRAM(void);
	void system_standbySRAMSeconds(uint32_t seconds);
	void system_standbySRAMUntilWakeUpPin(uint32_t WAKEUP_PIN_x, uint8_t polarity); // This function doesn't required rtc
#endif
#if defined(PWR_CR1_LPMS_2)
	void system_shutdown(void);
	void system_shutdownSeconds(uint32_t seconds);
	void system_shutdownUntilWakeUpPin(uint32_t WAKEUP_PIN_x, uint8_t polarity); // This function doesn't required rtc
#endif

	/* Virtual EEPROM Functions *********************************/
	void veeprom_init(void);
	uint8_t veeprom_writeByte(uint16_t address, uint8_t data);
	uint8_t veeprom_writeHalfWord(uint16_t address, uint16_t data);
	uint8_t veeprom_writeWord(uint16_t address, uint32_t data);
	uint8_t veeprom_writeFloat(uint16_t address, float *data);
	uint8_t veeprom_readByte(uint16_t address);
	uint16_t veeprom_readHalfWord(uint16_t address);
	uint32_t veeprom_readWord(uint16_t address);
	void veeprom_readFloat(uint16_t address, float *rdata);
	void veeprom_commit(void);
	void veeprom_clear(void);

#ifdef __cplusplus
}
#endif

#endif
