/**
  ******************************************************************************
  * @file    system_eeprom.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   System EEPROM Functions
  ******************************************************************************
*/

#include "System.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

// FLASH Timeout
#define FLASH_TIMEOUT_VALUE (50000U) // 50 s
#define FLASH_PAGE_SIZE	0x800u

#define DATA_EEPROM_SIZE 	512 // in bytes
#define DATA_EEPROM_END		(uint32_t)((FLASH_BASE) + (LL_GetFlashSize()*1024) - 1)
#define DATA_EEPROM_BASE	(uint32_t)((DATA_EEPROM_END) - FLASH_PAGE_SIZE + 1)
#define DATA_EEPROM_LIMIT (uint32_t)((DATA_EEPROM_BASE) + DATA_EEPROM_SIZE)

#define FLASH_TYPEPROGRAM_DOUBLEWORD    FLASH_CR_PG     /*!< Program a double-word (64-bit) at a specified address */
#define FLASH_TYPEPROGRAM_FAST          FLASH_CR_FSTPG  /*!< Fast program a 32 row double-word (64-bit) at a specified address */

#define FLASH_KEY1 0x45670123U /*!< Flash key1 */
#define FLASH_KEY2 0xCDEF89ABU /*!< Flash key2: used with FLASH_KEY1 \ \
																		to unlock the FLASH registers access */

#define FLASH_FLAG_EOP                  FLASH_SR_EOP      /*!< FLASH End of operation flag */
#define FLASH_FLAG_OPERR                FLASH_SR_OPERR    /*!< FLASH Operation error flag */
#define FLASH_FLAG_PROGERR              FLASH_SR_PROGERR  /*!< FLASH Programming error flag */
#define FLASH_FLAG_WRPERR               FLASH_SR_WRPERR   /*!< FLASH Write protection error flag */
#define FLASH_FLAG_PGAERR               FLASH_SR_PGAERR   /*!< FLASH Programming alignment error flag */
#define FLASH_FLAG_SIZERR               FLASH_SR_SIZERR   /*!< FLASH Size error flag  */
#define FLASH_FLAG_PGSERR               FLASH_SR_PGSERR   /*!< FLASH Programming sequence error flag */
#define FLASH_FLAG_MISERR               FLASH_SR_MISERR   /*!< FLASH Fast programming data miss error flag */
#define FLASH_FLAG_FASTERR              FLASH_SR_FASTERR  /*!< FLASH Fast programming error flag */
#if defined(FLASH_PCROP_SUPPORT)
#define FLASH_FLAG_RDERR                FLASH_SR_RDERR    /*!< FLASH PCROP read error flag */
#endif /* FLASH_PCROP_SUPPORT */
#define FLASH_FLAG_OPTVERR              FLASH_SR_OPTVERR  /*!< FLASH Option validity error flag */
#define FLASH_FLAG_BSY                  FLASH_SR_BSY1     /*!< FLASH Operation Busy flag */
#define FLASH_FLAG_CFGBSY               FLASH_SR_CFGBSY   /*!< FLASH Configuration Busy flag */
#define FLASH_FLAG_ECCC                 FLASH_ECCR_ECCC   /*!< FLASH ECC correction */
#define FLASH_FLAG_ECCD                 FLASH_ECCR_ECCD   /*!< FLASH ECC detection */
#if defined(FLASH_PCROP_SUPPORT)
#define FLASH_FLAG_SR_ERROR             (FLASH_FLAG_OPERR  | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR |  \
                                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR |   \
                                         FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR | FLASH_FLAG_RDERR |   \
                                         FLASH_FLAG_OPTVERR)     /*!< All SR error flags */
#else
#define FLASH_FLAG_SR_ERROR             (FLASH_FLAG_OPERR  | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR |  \
                                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR |   \
                                         FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR |   \
                                         FLASH_FLAG_OPTVERR)     /*!< All SR error flags */
#endif
#define FLASH_FLAG_BSY                  FLASH_SR_BSY1     /*!< FLASH Operation Busy flag */
#define FLASH_FLAG_CFGBSY               FLASH_SR_CFGBSY   /*!< FLASH Configuration Busy flag */
#define FLASH_FLAG_ECCC                 FLASH_ECCR_ECCC   /*!< FLASH ECC correction */
#define FLASH_FLAG_ECCD                 FLASH_ECCR_ECCD   /*!< FLASH ECC detection */

#define __HAL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)) != 0U) ? \
                                                 (READ_BIT(FLASH->ECCR, (__FLAG__)) == (__FLAG__))  : \
                                                 (READ_BIT(FLASH->SR,   (__FLAG__)) == (__FLAG__)))
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)        do { if(((__FLAG__) & (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)) != 0U) { SET_BIT(FLASH->ECCR, ((__FLAG__) & (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD))); }\
                                                     if(((__FLAG__) & ~(FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)) != 0U) { WRITE_REG(FLASH->SR, ((__FLAG__) & ~(FLASH_FLAG_ECCC | FLASH_FLAG_ECCD))); }\
                                                   } while(0U)

/** 
 ===============================================================================
              ##### Static array #####
 ===============================================================================
 */

static uint8_t _eedata[DATA_EEPROM_SIZE];

typedef union{
	uint8_t u8[8];
	uint64_t u64;
}eedata_t;

/** 
 ===============================================================================
              ##### Private functions #####
 ===============================================================================
 */

static uint8_t _waitForLastOperation(uint32_t timeout)
{
	uint32_t error;
	uint32_t exit_time = millis() + timeout;
	
	while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0x00U)
	{ 
		if (millis() >= exit_time)
			return 0;
	}

	/* check flash errors. Only ECC correction can be checked here as ECCD
      generates NMI */
  error = (FLASH->SR & FLASH_FLAG_SR_ERROR);
  error |= (FLASH->ECCR & FLASH_FLAG_ECCC);

  /* clear error flags */
  __HAL_FLASH_CLEAR_FLAG(error);

	exit_time = millis() + timeout;

  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY) != 0x00U)
  {
    if (millis() >= exit_time)
    {
      return 0;
    }
  }

	return 1;
}

static void _program_doubleWord(uint32_t Address, uint64_t Data)
{
  /* Set PG bit */
  SET_BIT(FLASH->CR, FLASH_CR_PG);

  /* Program first word */
  *(uint32_t *)Address = (uint32_t)Data;

  /* Barrier to ensure programming is performed in 2 steps, in right order
    (independently of compiler optimization behavior) */
  __ISB();

  /* Program second word */
  *(uint32_t *)(Address + 4U) = (uint32_t)(Data >> 32U);
}

static void _eeprom_unlock(void)
{
	if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00U)
	{
		/* Authorize the FLASH Registers access */
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);
	}
}

static void _eeprom_lock(void)
{
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

static uint8_t _eeprom_massErase(void){
	uint8_t status = 0;
	uint32_t tmp;
	uint32_t ee_page = (uint32_t)(((LL_GetFlashSize() * 1024)/FLASH_PAGE_SIZE) - 1);
	status = _waitForLastOperation(FLASH_TIMEOUT_VALUE);
	if(status == 0) { return 0; }
	tmp = (FLASH->CR & ~FLASH_CR_PNB);
  FLASH->CR = (tmp | (FLASH_CR_STRT | (ee_page <<  FLASH_CR_PNB_Pos) | FLASH_CR_PER));
	status = _waitForLastOperation(FLASH_TIMEOUT_VALUE);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	return status;
}

static uint8_t _eeprom_writeDWord(uint32_t address, uint64_t data){
	uint8_t status = 0;
	address = DATA_EEPROM_BASE + address;
	if(address > DATA_EEPROM_END) { return 0; }
	status = _waitForLastOperation(FLASH_TIMEOUT_VALUE);
	if(status == 0) { return 0; }
	_program_doubleWord(address, data);
	status = _waitForLastOperation(FLASH_TIMEOUT_VALUE);
	CLEAR_BIT(FLASH->CR, FLASH_TYPEPROGRAM_DOUBLEWORD);
	return status;
}

/** 
 ===============================================================================
              ##### Public functions #####
 ===============================================================================
 */

void veeprom_init(void){
	uint16_t index = 0;
	__IO uint32_t data32;
	uint32_t address = DATA_EEPROM_BASE;
	uint32_t endaddr = DATA_EEPROM_LIMIT;

	for(index = 0; index < DATA_EEPROM_SIZE; index++){
		_eedata[index] = 255;
	}

	while (address < endaddr)
  {
		index = address - DATA_EEPROM_BASE;
    data32 = *(__IO uint32_t *)address;
		_eedata[index] = (uint8_t)data32;
		_eedata[index+1] = (uint8_t)(data32 >> 8U);
		_eedata[index+2] = (uint8_t)(data32 >> 16U);
		_eedata[index+3] = (uint8_t)(data32 >> 24U);
    address = address + 4;
  }
}

uint8_t veeprom_writeByte(uint16_t address, uint8_t data)
{
	_eedata[address] = (uint8_t)data;
	return 1;
}

uint8_t veeprom_writeHalfWord(uint16_t address, uint16_t data)
{
	_eedata[address] = (uint8_t)data;
	_eedata[address+1] = (uint8_t)(data >> 8U) ;
	return 1;
}

uint8_t veeprom_writeWord(uint16_t address, uint32_t data)
{
	_eedata[address] = (uint8_t)data;
	_eedata[address+1] = (uint8_t)(data >> 8U) ;
	_eedata[address+2] = (uint8_t)(data >> 16U) ;
	_eedata[address+3] = (uint8_t)(data >> 24U) ;
	return 1;
}

uint8_t veeprom_writeFloat(uint16_t address, float *data)
{
	uint8_t *p = (uint8_t *)data;
	for(uint8_t i = 0; i < 4; i++){
		_eedata[address + i] = *p++;
	}
	return 1;
}

uint8_t veeprom_readByte(uint16_t address)
{
	return (uint8_t)_eedata[address];
}

uint16_t veeprom_readHalfWord(uint16_t address)
{
	uint16_t r;
	r = _eedata[address];
	r += ((uint16_t)_eedata[address+1] << 8);
	return r;
}

uint32_t veeprom_readWord(uint16_t address)
{
	uint32_t r;
	r = _eedata[address];
	r += ((uint32_t)_eedata[address+1] << 8);
	r += ((uint32_t)_eedata[address+2] << 16);
	r += ((uint32_t)_eedata[address+3] << 24);
	return r;
}

void veeprom_readFloat(uint16_t address, float *rdata)
{
	uint8_t* p = (uint8_t*)rdata;
	*p++ = _eedata[address];
	*p++ = _eedata[address+1];
	*p++ = _eedata[address+2];
	*p++ = _eedata[address+3];
}

void veeprom_commit(void){
	eedata_t d;
	_eeprom_unlock();
	_eeprom_massErase();
	for(uint16_t i = 0; i < DATA_EEPROM_SIZE; i+=8 ){
		for(uint8_t j = 0; j < 8; j++){
			d.u8[j] = _eedata[i+j];
		}
		_eeprom_writeDWord(i, d.u64);
	}
	_eeprom_lock();
}

void veeprom_clear(void){
	uint16_t i = 0;
	_eeprom_unlock();
	_eeprom_massErase();
	_eeprom_lock();
	for(i = 0; i < DATA_EEPROM_SIZE; i++){
		_eedata[i] = 255;
	}
}