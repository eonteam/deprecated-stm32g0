/**
  ******************************************************************************
  * @file    spi.h 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   SPI Library
  ******************************************************************************
*/

#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>
#include "pinmap_hal.h"
#include "stm32g0xx_ll_spi.h"

#define SPI_DATAMODE0 LL_SPI_POLARITY_LOW | LL_SPI_PHASE_1EDGE
#define SPI_DATAMODE1 LL_SPI_POLARITY_LOW | LL_SPI_PHASE_2EDGE
#define SPI_DATAMODE2 LL_SPI_POLARITY_HIGH | LL_SPI_PHASE_1EDGE
#define SPI_DATAMODE3 LL_SPI_POLARITY_HIGH | LL_SPI_PHASE_2EDGE

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

/* Initialization */
void spi_init(SPI_TypeDef *SPIx, uint32_t freq_hz, uint32_t datamode, pin_t sck, pin_t miso, pin_t mosi);

// Data Mode
void spi_setDataMode(SPI_TypeDef *SPIx, uint32_t SPI_DataMode);

// Prescaler Speed
void spi_setPrescaler(SPI_TypeDef *SPIx, uint32_t prescaler);
uint16_t spi_calculatePrescaler(SPI_TypeDef *SPIx, uint32_t freq_hz);
void spi_setFreq(SPI_TypeDef *SPIx, uint32_t freq_hz);

// SPI - 8 bits
uint8_t spi_write8(SPI_TypeDef *SPIx, uint8_t data);
void spi_writeMultiple8(SPI_TypeDef *SPIx, const uint8_t *pTData, uint8_t *pRData, uint8_t pSize);
uint8_t spi_read8(SPI_TypeDef *SPIx);
void spi_readMultiple8(SPI_TypeDef *SPIx, uint8_t *pRData, uint8_t pSize);

// SPI - 16 bits
uint16_t spi_write16(SPI_TypeDef *SPIx, uint16_t data);
void spi_writeMultiple16(SPI_TypeDef *SPIx, const uint16_t *pTData, uint16_t *pRData, uint8_t pSize);
uint16_t spi_read16(SPI_TypeDef *SPIx);
void spi_readMultiple16(SPI_TypeDef *SPIx, uint16_t *pRData, uint8_t pSize);

// SPI Slave - TODO: implementation
void spiSlave_init(SPI_TypeDef *SPIx, pin_t sck, pin_t miso, pin_t mosi, pin_t ssel, uint32_t datamode);
uint8_t spiSlave_available(SPI_TypeDef *SPIx);
uint8_t spiSlave_read8(SPI_TypeDef *SPIx);
uint16_t spiSlave_read16(SPI_TypeDef *SPIx);
void spiSlave_write8(SPI_TypeDef *SPIx, uint8_t val);
void spiSlave_write16(SPI_TypeDef *SPIx, uint16_t val);

#endif
