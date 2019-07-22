/**
  ******************************************************************************
  * @file    spi.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   SPI Functions
  ******************************************************************************
*/

#include "spi.h"
#include "gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_spi.h"

#define SPI_PHASE_MSK SPI_CR1_CPHA_Msk
#define SPI_POL_MSK SPI_CR1_CPOL_Msk

/** 
 ===============================================================================
              ##### FUNCIONES #####
 ===============================================================================
 */

void spi_init(SPI_TypeDef *SPIx, uint32_t freq_hz, uint32_t datamode, pin_t sck, pin_t miso, pin_t mosi)
{

	LL_SPI_InitTypeDef mspi_init;
	uint32_t _spi_src_clk;
	uint16_t _spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV2;
	LL_RCC_ClocksTypeDef clocks;

	LL_RCC_GetSystemClocksFreq(&clocks);

#if defined(SPI1)
	if (SPIx == SPI1)
	{
		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
		_spi_src_clk = clocks.PCLK2_Frequency;
	}
#endif
#if defined(SPI2)
	if (SPIx == SPI2)
	{
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
		_spi_src_clk = clocks.PCLK1_Frequency;
	}
#endif

	gpio_modeSPI(sck);
	gpio_modeSPI(miso);
	gpio_modeSPI(mosi);

	_spi_presc = _spi_src_clk / freq_hz;
	if ((_spi_src_clk / _spi_presc) > freq_hz)
		_spi_presc = _spi_presc + 1;

	if (_spi_presc <= 2)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV2;
	else if (_spi_presc > 2 && _spi_presc <= 4)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV4;
	else if (_spi_presc > 4 && _spi_presc <= 8)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV8;
	else if (_spi_presc > 8 && _spi_presc <= 16)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV16;
	else if (_spi_presc > 16 && _spi_presc <= 32)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV32;
	else if (_spi_presc > 32 && _spi_presc <= 64)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV64;
	else if (_spi_presc > 64 && _spi_presc <= 128)
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV128;
	else
		_spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV256;

	LL_SPI_Disable(SPIx);

	mspi_init.Mode = LL_SPI_MODE_MASTER;
	mspi_init.TransferDirection = LL_SPI_FULL_DUPLEX;
	mspi_init.BaudRate = _spi_presc;
	mspi_init.ClockPhase = (datamode & SPI_PHASE_MSK);
	mspi_init.ClockPolarity = (datamode & SPI_POL_MSK);
	mspi_init.BitOrder = LL_SPI_MSB_FIRST;
	mspi_init.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	mspi_init.NSS = LL_SPI_NSS_SOFT;
	mspi_init.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	mspi_init.CRCPoly = 7;

	LL_SPI_Init(SPIx, &mspi_init);

	LL_SPI_Enable(SPIx);
}

void spi_setPrescaler(SPI_TypeDef *SPIx, uint32_t prescaler)
{
	LL_SPI_SetBaudRatePrescaler(SPIx, prescaler);
}

uint16_t spi_calculatePrescaler(SPI_TypeDef *SPIx, uint32_t freq_hz)
{
	uint32_t _spi_src_clk;
	uint16_t _spi_presc = LL_SPI_BAUDRATEPRESCALER_DIV2;
	LL_RCC_ClocksTypeDef clocks;

	LL_RCC_GetSystemClocksFreq(&clocks);

	_spi_src_clk = clocks.PCLK1_Frequency;

	_spi_presc = _spi_src_clk / freq_hz;
	if ((_spi_src_clk / _spi_presc) > freq_hz)
		_spi_presc = _spi_presc + 1;

	if (_spi_presc <= 2)
		return LL_SPI_BAUDRATEPRESCALER_DIV2;
	else if (_spi_presc > 2 && _spi_presc <= 4)
		return LL_SPI_BAUDRATEPRESCALER_DIV4;
	else if (_spi_presc > 4 && _spi_presc <= 8)
		return LL_SPI_BAUDRATEPRESCALER_DIV8;
	else if (_spi_presc > 8 && _spi_presc <= 16)
		return LL_SPI_BAUDRATEPRESCALER_DIV16;
	else if (_spi_presc > 16 && _spi_presc <= 32)
		return LL_SPI_BAUDRATEPRESCALER_DIV32;
	else if (_spi_presc > 32 && _spi_presc <= 64)
		return LL_SPI_BAUDRATEPRESCALER_DIV64;
	else if (_spi_presc > 64 && _spi_presc <= 128)
		return LL_SPI_BAUDRATEPRESCALER_DIV128;
	else
		return LL_SPI_BAUDRATEPRESCALER_DIV256;
}

void spi_setFreq(SPI_TypeDef *SPIx, uint32_t freq_hz)
{
	uint16_t _baud = spi_calculatePrescaler(SPIx, freq_hz);
	LL_SPI_SetBaudRatePrescaler(SPIx, _baud);
}

void spi_setDataMode(SPI_TypeDef *SPIx, uint32_t SPI_DataMode)
{
	LL_SPI_SetClockPolarity(SPIx, (SPI_DataMode & SPI_POL_MSK));
	LL_SPI_SetClockPhase(SPIx, (SPI_DataMode & SPI_PHASE_MSK));
}

/** 
 ===============================================================================
              ##### 8 Bits #####
 ===============================================================================
 */

/**
  * @brief  SPI write 8 bits.
	* 
	* @param  SPIx: SPI to use.
	* @param  data: Data to transmit.
  */
uint8_t spi_write8(SPI_TypeDef *SPIx, uint8_t data)
{

	/* SPI - 8 bits data */
	LL_SPI_SetDataWidth(SPIx, LL_SPI_DATAWIDTH_8BIT);

	while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
		;
	/* Fill output buffer with data */
	SPIx->DR = (uint8_t)data;
	/* Wait for transmission to complete */
	while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
		;
	/* Return data from buffer */
	return (uint8_t)SPIx->DR;
}

/**
  * @brief  SPI write 8 bits multiple.
	* 
	* @param  SPIx: SPI to use.
	* @param  pTData: Transmit pointer.
	* @param  pRData: Receive pointer.
	* @param	pSize: Number of bytes.
  */
void spi_writeMultiple8(SPI_TypeDef *SPIx, const uint8_t *pTData, uint8_t *pRData, uint8_t pSize)
{
	uint8_t dummy = 0;
	/* SPI - 8 bits data */
	LL_SPI_SetDataWidth(SPIx, LL_SPI_DATAWIDTH_8BIT);

	while (pSize--)
	{
		while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
			;
		/* Fill output buffer with data */
		SPIx->DR = (uint8_t)*pTData++;
		/* Wait for transmission to complete */
		while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
			;
		/* Return data from buffer */
		if (pRData)
			*pRData++ = (uint8_t)SPIx->DR;
		else
			dummy = SPIx->DR;
	}
	UNUSED(dummy);
}

/**
  * @brief  SPI read 8 bits.
	* 
	* @param  SPIx: SPI to use.
  * @retval Byte read.
  */
uint8_t spi_read8(SPI_TypeDef *SPIx)
{
	/* Dummy Byte: 0xFF */
	return (uint8_t)spi_write8(SPIx, 0xFF);
}

/**
  * @brief  SPI write 8 bits multiple.
	* 
	* @param  SPIx: SPI to use.
	* @param  pTData: Transmit pointer.
	* @param  pRData: Receive pointer.
	* @param	pSize: Number of bytes.
  */
void spi_readMultiple8(SPI_TypeDef *SPIx, uint8_t *pRData, uint8_t pSize)
{
	/* SPI - 8 bits data */
	LL_SPI_SetDataWidth(SPIx, LL_SPI_DATAWIDTH_8BIT);

	while (pSize--)
	{
		while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
			;
		/* Fill output buffer with data */
		SPIx->DR = (uint8_t)0xFF;
		/* Wait for transmission to complete */
		while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
			;
		/* Return data from buffer */
		*pRData++ = (uint8_t)SPIx->DR;
	}
}

/** 
 ===============================================================================
              ##### 16 Bits #####
 ===============================================================================
 */

/**
  * @brief  SPI write 16 bits.
	* 
	* @param  SPIx: SPI to use.
	* @param  data: Data to transmit.
  */
uint16_t spi_write16(SPI_TypeDef *SPIx, uint16_t data)
{

	/* SPI - 16 bits data */
	LL_SPI_SetDataWidth(SPIx, LL_SPI_DATAWIDTH_16BIT);

	while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
		;
	/* Fill output buffer with data */
	SPIx->DR = (uint16_t)data;
	/* Wait for transmission to complete */
	while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
		;
	/* Return data from buffer */
	return (uint16_t)SPIx->DR;
}

/**
  * @brief  SPI write 16 bits multiple.
	* 
	* @param  SPIx: SPI to use.
	* @param  pTData: Transmit pointer.
	* @param  pRData: Receive pointer.
	* @param	pSize: Number of bytes.
  */
void spi_writeMultiple16(SPI_TypeDef *SPIx, const uint16_t *pTData, uint16_t *pRData, uint8_t pSize)
{
	uint16_t dummy = 0;
	/* SPI - 16 bits data */
	LL_SPI_SetDataWidth(SPIx, LL_SPI_DATAWIDTH_16BIT);

	while (pSize--)
	{
		while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
			;
		/* Fill output buffer with data */
		SPIx->DR = (uint16_t)*pTData++;
		/* Wait for transmission to complete */
		while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
			;
		/* Return data from buffer */
		if (pRData)
			*pRData++ = (uint16_t)SPIx->DR;
		else
			dummy = (uint16_t)SPIx->DR;
	}
	UNUSED(dummy);
}

/**
  * @brief  SPI read 16 bits.
	* 
	* @param  SPIx: SPI to use.
  * @retval Halfword read.
  */
uint16_t spi_read16(SPI_TypeDef *SPIx)
{
	/* Dummy Byte: 0xFFFF */
	return (uint16_t)spi_write16(SPIx, 0xFFFF);
}

/**
  * @brief  SPI read 16 bits multiple.
	* 
	* @param  SPIx: SPI to use.
	* @param  pRData: Receive pointer.
	* @param	pSize: Number of halfwords.
  */
void spi_readMultiple16(SPI_TypeDef *SPIx, uint16_t *pRData, uint8_t pSize)
{
	/* SPI - 16 bits data */
	LL_SPI_SetDataWidth(SPIx, LL_SPI_DATAWIDTH_16BIT);

	while (pSize--)
	{
		while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
			;
		/* Fill output buffer with data */
		SPIx->DR = (uint16_t)0xFFFF;
		/* Wait for transmission to complete */
		while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
			;
		/* Return data from buffer */
		*pRData++ = (uint16_t)SPIx->DR;
	}
}

/** 
 ===============================================================================
              ##### SPI Slave Functions #####
 ===============================================================================
 */

/*#define SSPI_BUFFER_SIZE 64 // Slave SPI Buffer length
static unsigned char _rx_buffer_sspi[SSPI_BUFFER_SIZE];
volatile static uint8_t _rx_buffer_tail_sspi = 0;
volatile static uint8_t _rx_buffer_head_sspi = 0;

__STATIC_INLINE void sspi_rx_buffer_insert(uint8_t element)
{
	uint8_t i = (uint8_t)(_rx_buffer_head_sspi + 1) % SSPI_BUFFER_SIZE;
	if (i != _rx_buffer_tail_sspi)
	{
		_rx_buffer_sspi[_rx_buffer_head_sspi] = element;
		_rx_buffer_head_sspi = i;
	}
}

void SPISlave_init(SPI_TypeDef *SPIx, pin_t sck, pin_t miso, pin_t mosi, pin_t ssel, uint32_t datamode)
{
	LL_SPI_InitTypeDef sspi_init;
	IRQn_Type irq;

	if (SPIx == SPI1)
	{
		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
		irq = SPI1_IRQn;
	}
#if defined(SPI2)
	if (SPIx == SPI2)
	{
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
		irq = SPI2_IRQn;
	}
#endif

	GPIO_SetAFSpeed(SPEED_LOW);
	GPIO_ModeAF(ssel, AF_PP, AF_0);
	GPIO_ModeAF(sck, AF_PP, AF_0);
	GPIO_ModeAF(miso, AF_PP, AF_0);
	GPIO_ModeAF(mosi, AF_PP, AF_0);

	LL_SPI_Disable(SPIx);

	sspi_init.Mode = LL_SPI_MODE_SLAVE;
	sspi_init.TransferDirection = LL_SPI_FULL_DUPLEX;
	sspi_init.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
	sspi_init.ClockPhase = (datamode & SPI_PHASE_MSK);
	sspi_init.ClockPolarity = (datamode & SPI_POL_MSK);
	sspi_init.BitOrder = LL_SPI_MSB_FIRST;
	sspi_init.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	sspi_init.NSS = LL_SPI_NSS_HARD_OUTPUT;
	sspi_init.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	sspi_init.CRCPoly = 7;

	LL_SPI_Init(SPIx, &sspi_init);

	LL_SPI_Enable(SPIx);

	NVIC_SetPriority(irq, 0);
	NVIC_EnableIRQ(irq);
	LL_SPI_EnableIT_RXNE(SPIx);
}

void SPI1_IRQHandler(void)
{
	uint8_t b = 0;

if (LL_SPI_IsActiveFlag_RXNE(SPI1) != RESET)
{
	b = (uint8_t)SPI1->DR;
	sspi_rx_buffer_insert(b);
}
}

#if defined(SPI2)
void SPI2_IRQHandler(void)
{
	uint8_t b = 0;

if (LL_SPI_IsActiveFlag_RXNE(SPI2) != RESET)
{
	b = (uint8_t)SPI2->DR;
	sspi_rx_buffer_insert(b);
}
}
#endif

uint8_t SPISlave_available(SPI_TypeDef *SPIx)
{
	//if(LL_SPI_IsActiveFlag_RXNE(SPIx) != RESET) return 1;
	//else return 0;
	return ((uint8_t)(SSPI_BUFFER_SIZE + _rx_buffer_head_sspi - _rx_buffer_tail_sspi)) % SSPI_BUFFER_SIZE;
}

uint8_t SPISlave_read8(SPI_TypeDef *SPIx)
{
	//while(LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET);
	//return (uint8_t)SPIx->DR;
if (_rx_buffer_head_sspi == _rx_buffer_tail_sspi)
{
	return 255; // TODO
}
else
{
	uint8_t c = _rx_buffer_sspi[_rx_buffer_tail_sspi];
	_rx_buffer_tail_sspi = (uint8_t)(_rx_buffer_tail_sspi + 1) % SSPI_BUFFER_SIZE;
	return c;
}
}

uint16_t SPISlave_read16(SPI_TypeDef *SPIx)
{
	while (LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET)
		;
	return (uint16_t)SPIx->DR;
}

void SPISlave_write8(SPI_TypeDef *SPIx, uint8_t val)
{
	while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
		;
	SPIx->DR = (uint8_t)val;
}

void SPISlave_write16(SPI_TypeDef *SPIx, uint16_t val)
{
	while (LL_SPI_IsActiveFlag_TXE(SPIx) == RESET)
		;
	SPIx->DR = (uint16_t)val;
}*/
