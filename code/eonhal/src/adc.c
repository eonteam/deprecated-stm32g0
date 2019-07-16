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

	LL_ADC_InitTypeDef ADC_InitStruct;
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct;

	//ADC
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_AUTOWAIT_AUTOPOWEROFF; // If the ADC clock is less than 2MHz, we can enable LowPowerFrequency
	LL_ADC_Init(ADC1, &ADC_InitStruct);

	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);

	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_1, _adc_sample_time);

	LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);

	LL_ADC_SetCommonFrequencyMode(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_FREQ_MODE_HIGH);

	LL_ADC_EnableIT_EOC(ADC1);

	LL_ADC_DisableIT_EOS(ADC1);

	LL_ADC_StartCalibration(ADC1);

	LL_ADC_Enable(ADC1);
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
		LL_ADC_REG_SetSequencerChannels(ADC1, PIN_MAP[pin].adcCh);
		adcChannelConfigured = PIN_MAP[pin].adcCh;
	}

	for (i = 0; i < ADC_BUFFERSIZE; i++)
	{
		ADC_ConvertedValues[i] = 0;
		LL_ADC_ClearFlag_EOC(ADC1);
		LL_ADC_ClearFlag_EOS(ADC1);
		LL_ADC_ClearFlag_OVR(ADC1);
		LL_ADC_REG_StartConversion(ADC1);
		while ((LL_ADC_ReadReg(ADC1, ISR) & LL_ADC_FLAG_EOS) == RESET)
			;
		if ((LL_ADC_ReadReg(ADC1, ISR) & LL_ADC_FLAG_EOC) == LL_ADC_FLAG_EOC)
		{
			if ((LL_ADC_ReadReg(ADC1, CR) & ADC_CR_ADSTART) == RESET)
			{
				LL_ADC_DisableIT_EOC(ADC1);
				LL_ADC_DisableIT_EOS(ADC1);
			}
		}
		ADC_ConvertedValues[i] = ADC1->DR;
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
