/**
  ******************************************************************************
  * @file    adc.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.0
  * @date    2019
  * @brief   ADC Functions
  ******************************************************************************
*/

#include "adc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_dma.h"
#include "pinmap_impl.h"

/** 
 ===============================================================================
              ##### DEFINICIONES #####
 ===============================================================================
 */

// Number of samples taken from an ADC Channel, to get an average value
// Note: It isn't the number of channels
#define ADC_BUFFERSIZE 18
#define ADC_SAMPLING_TIME LL_ADC_SAMPLINGTIME_1CYCLE_5
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

/** 
 ===============================================================================
              ##### VARIABLES #####
 ===============================================================================
 */

static uint16_t ADC_ConvertedValues[ADC_BUFFERSIZE];
static uint8_t adcInitFirstTime = true;
static uint32_t adcChannelConfigured = ADC_CHANNEL_NONE;
static uint32_t _adc_sample_time = ADC_SAMPLING_TIME;

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

void adc_setSampleTime(uint8_t ADC_SampleTime)
{
	if (ADC_SampleTime > LL_ADC_SAMPLINGTIME_160CYCLES_5)
	{
		_adc_sample_time = ADC_SAMPLING_TIME;
	}
	else
	{
		_adc_sample_time = ADC_SampleTime;
	}
}

/* Inicializa el ADC */
static void ADC_Init(void)
{
	__IO uint32_t wait_loop_index = 0U;
	LL_ADC_InitTypeDef ADC_InitStruct;
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct;

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

	//ADC
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
	LL_ADC_SetCommonFrequencyMode(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_FREQ_MODE_HIGH);
	LL_ADC_REG_SetSequencerConfigurable(ADC1, LL_ADC_REG_SEQ_CONFIGURABLE);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_1, _adc_sample_time);
	LL_ADC_DisableIT_EOC(ADC1);
	LL_ADC_DisableIT_EOS(ADC1);

	ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);

	LL_ADC_Disable(ADC1);

	LL_ADC_EnableInternalRegulator(ADC1);

	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while (wait_loop_index != 0)
	{
		wait_loop_index--;
	}

	LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);

	LL_ADC_StartCalibration(ADC1);

	while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
	{
	}

	wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
	while (wait_loop_index != 0)
	{
		wait_loop_index--;
	}

	LL_ADC_Enable(ADC1);

	while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
	{
	}
}

uint16_t adc_readU(pin_t pin)
{
	uint8_t i = 0;
	uint32_t ADC_SummatedValue = 0;
	uint16_t ADC_AveragedValue = 0;
	STM32_Pin_Info *PIN_MAP = HAL_Pin_Map();

	if (adcInitFirstTime == true)
	{
		ADC_Init();
		adcInitFirstTime = false;
	}

	if (adcChannelConfigured != PIN_MAP[pin].adcCh)
	{
		LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, PIN_MAP[pin].adcCh);
		LL_ADC_SetChannelSamplingTime(ADC1, PIN_MAP[pin].adcCh, LL_ADC_SAMPLINGTIME_COMMON_1);
		adcChannelConfigured = PIN_MAP[pin].adcCh;
	}

	for (i = 0; i < ADC_BUFFERSIZE; i++)
	{
		ADC_ConvertedValues[i] = 0;

		if ((LL_ADC_IsEnabled(ADC1) == 1) ||
				(LL_ADC_IsDisableOngoing(ADC1) == 0))
		{
			return 0;
		}

		while (LL_ADC_REG_IsConversionOngoing(ADC1) != 0)
		{
		}

		LL_ADC_REG_StartConversion(ADC1);
		// while ((LL_ADC_ReadReg(ADC1, ISR) & LL_ADC_FLAG_EOS) == RESET)
		// {
		// }
		while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0)
		{
		}

		LL_ADC_ClearFlag_EOC(ADC1);
		LL_ADC_ClearFlag_EOS(ADC1);
		LL_ADC_ClearFlag_OVR(ADC1);

		ADC_ConvertedValues[i] = LL_ADC_REG_ReadConversionData12(ADC1);
		ADC_SummatedValue += ADC_ConvertedValues[i];
	}

	ADC_AveragedValue = (uint16_t)(ADC_SummatedValue / ADC_BUFFERSIZE);

	// Return ADC averaged value
	return ADC_AveragedValue;
}

float adc_readN(pin_t pin)
{
	uint16_t value = adc_readU(pin);
	return (float)value * (1.0f / (float)0xFFF); // 12 bits range
}
