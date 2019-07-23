/**
  ******************************************************************************
  * @file    i2c.h 
  * @author  Pablo Fuentes
	* @version V1.0.0
  * @date    2019
  * @brief   I2C Library
  ******************************************************************************
*/

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include "stm32g0xx_ll_i2c.h"
#include "pinmap_hal.h"

/* Stop Bit */
#define I2C_STOP 0x01
#define I2C_NO_STOP 0x00

/* I2C Modes */
#define I2C_MASTER 0x00
#define I2C_SLAVE 0x01

/* I2C Frequencies */
#define I2C_100KHZ_C2MHZ 0x00100508 // 2 MHZ
#define I2C_100KHZ_C4MHZ 0x00200D13 // 4 MHZ
#define I2C_400KHZ_C4MHZ 0x00100002
#define I2C_100KHZ_C6MHZ 0x0030171B // 6 MHZ
#define I2C_400KHZ_C6MHZ 0x00200105
#define I2C_100KHZ_C8MHZ 0x00501D28 // 8 MHZ
#define I2C_400KHZ_C8MHZ 0x00200208
#define I2C_1MHZ_C8MHZ 0x00000001
#define I2C_100KHZ_C16MHZ 0x00A03D53 // 16 MHZ
#define I2C_400KHZ_C16MHZ 0x00500615
#define I2C_1MHZ_C16MHZ 0x00100105
#define I2C_100KHZ_C32MHZ 0x20602938 // 32 MHZ
#define I2C_400KHZ_C32MHZ 0x00B0122A
#define I2C_1MHZ_C32MHZ 0x0030040E
#define I2C_100KHZ_C64MHZ 0xC0311319 // 64 MHZ
#define I2C_400KHZ_C64MHZ 0x10B1102E
#define I2C_1MHZ_C64MHZ 0x00710B1E

/* I2C Slave Responses */
#define I2C_NoData 0         // the slave has not been addressed
#define I2C_ReadAddressed 1  // the master has requested a read from this slave (slave = transmitter)
#define I2C_WriteGeneral 2   // the master is writing to all slave
#define I2C_WriteAddressed 3 // the master is writing to this slave (slave = receiver)

/* Redefinition of Slave Responses for easy set */
#define I2C_RequestNoData I2C_NoData
#define I2C_RequestReadAddressed I2C_ReadAddressed
#define I2C_RequestWriteGeneral I2C_WriteGeneral
#define I2C_RequestWriteAddressed I2C_WriteAddressed

/** 
 ===============================================================================
              ##### Public functions #####
 ===============================================================================
 */

// Initialization
void i2c_init(I2C_TypeDef *I2Cx, uint32_t freq, pin_t scl, pin_t sda);
void i2c_reset(I2C_TypeDef *I2Cx);
void i2c_setFreq(I2C_TypeDef *I2Cx, uint32_t freq, uint8_t i2c_master_slave);

// Start y Stop
uint8_t i2c_start(I2C_TypeDef *I2Cx);
#define i2c_stop(__I2C__) __I2C__->CR2 |= I2C_CR2_STOP

// Byte Write and Read
int16_t i2c_read8(I2C_TypeDef *I2Cx, int last);
uint8_t i2c_write8(I2C_TypeDef *I2Cx, int data);

/* 
Funciones de escritura y lectura completas con start, stop y delays incluidos 
args:
	I2Cx = Establece el I2C a utilizar
	address = Estable la direcci�n del esclavo
	data = Puntero que establece los datos a enviar o el puntero que recibe los datos
	length = La cantidad de datos que se van a leer o a escribir
	stop = Puede tomar dos valores: I2C_STOP o I2C_NO_STOP. Este argumento especifica si al final
				 de la escritura o lectura se envia el bit de Stop, generalmente se usa I2C_STOP.
*/
int16_t i2c_read(I2C_TypeDef *I2Cx, int address, char *data, int length, int stop);
int16_t i2c_write(I2C_TypeDef *I2Cx, int address, const char *data, int length, int stop);

/** 
 ===============================================================================
              ##### FUNCIONES SLAVE #####
 ===============================================================================
 */

/* Initialization */
void i2cSlave_init(I2C_TypeDef *I2Cx, uint32_t freq, int SlaveAddr, pin_t scl, pin_t sda);
void i2cSlave_initWIrq(I2C_TypeDef *I2Cx, uint32_t freq, int SlaveAddr, pin_t scl, pin_t sda);
void i2cSlave_address(I2C_TypeDef *I2Cx, uint32_t address);
void i2cSlave_mode(I2C_TypeDef *I2Cx, int enable_slave);

/* Receive Master Request */
int i2cSlave_receiveMasterRequest(I2C_TypeDef *I2Cx);

/* Funciones de Escritura y Lectura */
int i2cSlave_read(I2C_TypeDef *I2Cx, char *data, int length);
int i2cSlave_write(I2C_TypeDef *I2Cx, const char *data, int length);
#define i2cSlave_writeByte(__I2C__, __DATA__) i2c_write8(__I2C__, __DATA__)
#define i2cSlave_readByte(__I2C__) i2c_read8(__I2C__, 0)
#define i2cSlave_stop(__I2C__) i2c_stop(__I2C__)

#endif
