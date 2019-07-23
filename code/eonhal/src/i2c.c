/**
  ******************************************************************************
  * @file    i2c.c 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   I2C Functions
  ******************************************************************************
*/

#include "i2c.h"
#include "gpio.h"
#include "stm32g0xx_ll_bus.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

// Timeout
#define FLAG_TIMEOUT ((uint16_t)0x1000)
#define LONG_TIMEOUT ((uint16_t)0x8000)

/** 
 ===============================================================================
              ##### Public functions #####
 ===============================================================================
 */

void i2c_init(I2C_TypeDef *I2Cx, uint32_t freq, pin_t scl, pin_t sda)
{
#ifdef I2C1
	if (I2Cx == I2C1)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
	}
#endif
#if defined(I2C2)
	if (I2Cx == I2C2)
	{
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
	}
#endif

	gpio_modeI2C(scl);
	gpio_modeI2C(sda);
	i2c_reset(I2Cx);
	i2c_setFreq(I2Cx, freq, I2C_MASTER);
}

void i2c_reset(I2C_TypeDef *I2Cx)
{
	uint16_t timeout;

	timeout = LONG_TIMEOUT;
	while ((LL_I2C_IsActiveFlag_BUSY(I2Cx) != RESET) && (timeout-- != 0))
		;
#if defined(I2C1)
	if (I2Cx == I2C1)
	{

		LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);
		LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);
	}
#endif
#if defined(I2C2)
	if (I2Cx == I2C2)
	{
		LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C2);
		LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C2);
	}
#endif
}

void i2c_setFreq(I2C_TypeDef *I2Cx, uint32_t freq, uint8_t i2c_master_slave)
{
	uint16_t timeout;

	timeout = LONG_TIMEOUT;
	while ((LL_I2C_IsActiveFlag_BUSY(I2Cx) != RESET) && (timeout-- != 0))
		;

	LL_I2C_Disable(I2Cx);
	// Standard mode with Rise Time = 400ns and Fall Time = 100ns (100KHz)
	// Fast mode with Rise Time = 250ns and Fall Time = 100ns			(400KHz)
	// Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns	(1MHz)
	LL_I2C_SetTiming(I2Cx, freq);
	LL_I2C_DisableOwnAddress1(I2Cx);
	LL_I2C_DisableOwnAddress2(I2Cx);
	LL_I2C_SetMode(I2Cx, LL_I2C_MODE_I2C);
	LL_I2C_EnableAutoEndMode(I2Cx);
	LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
	LL_I2C_DisableClockStretching(I2Cx);
	LL_I2C_DisableGeneralCall(I2Cx);
	LL_I2C_Enable(I2Cx);
}

uint8_t i2c_start(I2C_TypeDef *I2Cx)
{
	uint16_t timeout;

	LL_I2C_ClearFlag_NACK(I2Cx);

	// Wait the STOP condition has been previously correctly sent
	timeout = FLAG_TIMEOUT;
	while ((I2Cx->CR2 & I2C_CR2_STOP) == I2C_CR2_STOP)
	{
		if ((timeout--) == 0)
			return 1;
	}

	// Generate the START condition
	LL_I2C_GenerateStartCondition(I2Cx);

	// Wait the START condition has been correctly sent
	timeout = FLAG_TIMEOUT;
	while (LL_I2C_IsActiveFlag_BUSY(I2Cx) == 0)
	{
		if ((timeout--) == 0)
		{
			return 1;
		}
	}

	return 0;
}

/** 
 ===============================================================================
              ##### Byte functions #####
 ===============================================================================
 */

int16_t i2c_read8(I2C_TypeDef *I2Cx, int last)
{
	uint16_t timeout;

	// Wait until the byte is received
	timeout = FLAG_TIMEOUT;
	while (LL_I2C_IsActiveFlag_RXNE(I2Cx) == RESET)
	{
		if ((timeout--) == 0)
		{
			return -1; // fail read
		}
	}

	return (uint8_t)LL_I2C_ReceiveData8(I2Cx); // success
}

uint8_t i2c_write8(I2C_TypeDef *I2Cx, int data)
{
	int timeout;

	// Wait until the byte is transmitted
	timeout = FLAG_TIMEOUT;
	while (LL_I2C_IsActiveFlag_TXIS(I2Cx) == RESET)
	{
		if ((timeout--) == 0)
		{
			return 0; // fail write
		}
	}

	LL_I2C_TransmitData8(I2Cx, data);

	return 1; // success
}

/** 
 ===============================================================================
              ##### Write and read functions #####
 ===============================================================================
 */

int16_t i2c_read(I2C_TypeDef *I2Cx, int address, char *data, int length, int stop)
{
	uint16_t timeout;
	int16_t count, value;

	// Handle Transfer
	LL_I2C_HandleTransfer(I2Cx, address, LL_I2C_ADDRSLAVE_7BIT, length & 0xFF, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ);

	// Read all bytes
	for (count = 0; count < length; count++)
	{
		value = i2c_read8(I2Cx, 0);
		data[count] = (char)value;
	}

	// Wait transfer complete
	timeout = FLAG_TIMEOUT;
	while (LL_I2C_IsActiveFlag_TC(I2Cx) == RESET)
	{
		timeout--;
		if (timeout == 0)
		{
			return -1;
		}
	}

	LL_I2C_ClearFlag_TXE(I2Cx); // TODO: test

	// If not repeated start, send stop.
	if (stop)
	{
		i2c_stop(I2Cx);
		/* Wait until STOPF flag is set */
		timeout = FLAG_TIMEOUT;
		while (LL_I2C_IsActiveFlag_STOP(I2Cx) == RESET)
		{
			timeout--;
			if (timeout == 0)
			{
				return -1;
			}
		}
		/* Clear STOP Flag */
		LL_I2C_ClearFlag_STOP(I2Cx);
	}

	return length;
}

int16_t i2c_write(I2C_TypeDef *I2Cx, int address, const char *data, int length, int stop)
{
	uint16_t timeout;
	int16_t count;

	// Handle Transfer
	LL_I2C_HandleTransfer(I2Cx, address, LL_I2C_ADDRSLAVE_7BIT, length & 0xFF, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

	for (count = 0; count < length; count++)
	{
		i2c_write8(I2Cx, data[count]);
	}

	// Wait transfer complete
	timeout = FLAG_TIMEOUT;
	while (LL_I2C_IsActiveFlag_TC(I2Cx) == RESET)
	{
		timeout--;
		if (timeout == 0)
		{
			return -1;
		}
	}

	LL_I2C_ClearFlag_TXE(I2Cx); // TODO: test

	// If not repeated start, send stop.
	if (stop)
	{
		i2c_stop(I2Cx);
		/* Wait until STOPF flag is set */
		timeout = FLAG_TIMEOUT;
		while (LL_I2C_IsActiveFlag_STOP(I2Cx) == RESET)
		{
			timeout--;
			if (timeout == 0)
			{
				return -1;
			}
		}
		/* Clear STOP Flag */
		LL_I2C_ClearFlag_STOP(I2Cx);
	}

	return count;
}

/** 
 ===============================================================================
              ##### Slave functions #####
 ===============================================================================
 */

///* I2C Slave Init */
//void I2C_Slave_Init(I2C_TypeDef* I2Cx, uint32_t freq, int SlaveAddr, pin_t scl, pin_t sda){
//	I2C_Master_Init(I2Cx, freq, scl, sda);
//	I2C_Slave_Mode(I2Cx, 1);
//	I2C_Slave_Address(I2Cx, SlaveAddr);
//}

///* I2C Slave with Interrupt Init */
//void I2C_SlaveWithInterrupt_Init(I2C_TypeDef* I2Cx, uint32_t freq, int SlaveAddr, pin_t scl, pin_t sda){
//	NVIC_InitTypeDef i2c_slave_nvic;
//	uint8_t _i2c_er_irq;
//
//	I2C_Master_Init(I2Cx, freq, scl, sda);
//	I2C_Slave_Mode(I2Cx, 1);
//	I2C_Slave_Address(I2Cx, SlaveAddr);
//
//	/* Configure the Priority Group to 1 bit */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	if(I2Cx == I2C1){
//		_i2c_er_irq = I2C1_ER_IRQn;
//		i2c_slave_nvic.NVIC_IRQChannel = I2C1_EV_IRQn;
//	}else if(I2Cx == I2C2){
//		_i2c_er_irq = I2C2_ER_IRQn;
//		i2c_slave_nvic.NVIC_IRQChannel = I2C2_EV_IRQn;
//	}
//	i2c_slave_nvic.NVIC_IRQChannelPreemptionPriority = 1;
//  i2c_slave_nvic.NVIC_IRQChannelSubPriority = 0;
//  i2c_slave_nvic.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&i2c_slave_nvic);
//
//	i2c_slave_nvic.NVIC_IRQChannel = _i2c_er_irq;
//	NVIC_Init(&i2c_slave_nvic);
//
//	/* Enable Error Interrupt */
//  I2C_ITConfig(I2Cx, (I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
//}

///* I2C Set Slave Address */
//void I2C_Slave_Address(I2C_TypeDef* I2Cx, uint32_t address){
//    uint16_t tmpreg = 0;

//    // Get the old register value
//    tmpreg = I2Cx->OAR1;
//    // Reset address bits
//    tmpreg &= 0xFC00;
//    // Set new address
//    tmpreg |= (uint16_t)((uint16_t)address & (uint16_t)0x00FE); // 7-bits
//    // Store the new register value
//    I2Cx->OAR1 = tmpreg;
//}

///* I2C Set Slave Mode Enable o Disable */
//void I2C_Slave_Mode(I2C_TypeDef* I2Cx, int enable_slave){
//    if (enable_slave) {
//        /* Enable Address Acknowledge */
//        I2Cx->CR1 |= I2C_CR1_ACK;
//    }
//}

///* Slave Receive Master Request */
//int I2C_Slave_ReceiveMasterRequest(I2C_TypeDef* I2Cx){
//    int retValue = I2C_NoData;

//    if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET) {
//        if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) != RESET) {
//            if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TRA) != RESET)
//                retValue = I2C_ReadAddressed;
//            else
//                retValue = I2C_WriteAddressed;

//            I2C_ClearFlag(I2Cx, I2C_FLAG_ADDR);
//        }
//    }

//    return (retValue);
//}

///* Slave Read */
//int I2C_Slave_Read(I2C_TypeDef* I2Cx, char *data, int length){
//    uint32_t Timeout;
//    int size = 0;

//    while (length > 0) {
//        /* Wait until RXNE flag is set */
//        // Wait until the byte is received
//        Timeout = FLAG_TIMEOUT;
//        while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET) {
//            Timeout--;
//            if (Timeout == 0) {
//                return -1;
//            }
//        }

//        /* Read data from DR */
//        (*data++) = I2Cx->DR;
//        length--;
//        size++;

//        if ((I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == SET) && (length != 0)) {
//            /* Read data from DR */
//            (*data++) = I2Cx->DR;
//            length--;
//            size++;
//        }
//    }

//    /* Wait until STOP flag is set */
//    Timeout = FLAG_TIMEOUT;
//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET) {
//        Timeout--;
//        if (Timeout == 0) {
//            return -1;
//        }
//    }

//    /* Clear STOP flag */
//    __I2C_CLEAR_STOPFLAG(I2Cx);

//    /* Wait until BUSY flag is reset */
//    Timeout = FLAG_TIMEOUT;
//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET) {
//        Timeout--;
//        if (Timeout == 0) {
//            return -1;
//        }
//    }

//    return size;
//}

///* Slave Write */
//int I2C_Slave_Write(I2C_TypeDef* I2Cx, const char *data, int length){
//    uint32_t Timeout;
//    int size = 0;

//    while (length > 0) {
//        /* Wait until TXE flag is set */
//        Timeout = FLAG_TIMEOUT;
//        while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) == RESET) {
//            Timeout--;
//            if (Timeout == 0) {
//                return -1;
//            }
//        }

//        /* Write data to DR */
//        I2Cx->DR = (*data++);
//        length--;
//        size++;

//        if ((I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == SET) && (length != 0)) {
//            /* Write data to DR */
//            I2Cx->DR = (*data++);
//            length--;
//            size++;
//        }
//    }

//    /* Wait until AF flag is set */
//    Timeout = FLAG_TIMEOUT;
//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == RESET) {
//        Timeout--;
//        if (Timeout == 0) {
//            return -1;
//        }
//    }

//    /* Clear AF flag */
//    I2C_ClearFlag(I2Cx, I2C_FLAG_AF);

//    /* Wait until BUSY flag is reset */
//    Timeout = FLAG_TIMEOUT;
//    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET) {
//        Timeout--;
//        if (Timeout == 0) {
//            return -1;
//        }
//    }

//    return size;
//}

///**
// ===============================================================================
//              ##### FUNCIONES SLAVE Interrupt Error #####
// ===============================================================================
// */

//void I2C1_ER_IRQHandler(void){
//  /* Read SR1 register to get I2C error */
//  if ((I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0xFF00) != 0x00)
//  {
//    /* Clears erreur flags */
//    I2C1->SR1 &= 0x00FF;
//  }
//}

//void I2C2_ER_IRQHandler(void){
//  /* Read SR1 register to get I2C error */
//  if ((I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0xFF00) != 0x00)
//  {
//    /* Clears erreur flags */
//    I2C2->SR1 &= 0x00FF;
//  }
//}
