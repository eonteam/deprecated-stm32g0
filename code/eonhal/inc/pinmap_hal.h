/**
 ******************************************************************************
 * @file    pinmap_hal.h
 * @version V1.0.1
 * @date    2019
 * @brief   STM32 pin map
 ******************************************************************************
 */

#ifndef __PINMAP_HAL_H
#define __PINMAP_HAL_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32g0xx.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IRQ(__IRQ__) __IRQ__()

  // **** Types
  typedef uint8_t pin_t;
  typedef uint8_t mode_t;
  typedef uint8_t pull_t;
  typedef uint8_t speed_t;

  // **** Function that returns STM32 Pin Map
  typedef struct STM32_Pin_Info STM32_Pin_Info;
  STM32_Pin_Info *HAL_Pin_Map(void);

/***
 * Alternate Functions
 */
#define AF_0 ((uint8_t)0x00)
#define AF_1 ((uint8_t)0x01)
#define AF_2 ((uint8_t)0x02)
#define AF_3 ((uint8_t)0x03)
#define AF_4 ((uint8_t)0x04)
#define AF_5 ((uint8_t)0x05)
#define AF_6 ((uint8_t)0x06)
#define AF_7 ((uint8_t)0x07)
#define AF_NONE ((uint8_t)0xFF)

  /***
 * Pin Definitions
 */

#if defined(STM32G070RB) || defined(STM32G071RB)
// **** Pins
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PA8 8
#define PA9 9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA15 14

#define PB0 15
#define PB1 16
#define PB2 17
#define PB3 18
#define PB4 19
#define PB5 20
#define PB6 21
#define PB7 22
#define PB8 23
#define PB9 24
#define PB10 25
#define PB11 26
#define PB12 27
#define PB13 28
#define PB14 29
#define PB15 30

#define PC0 31
#define PC1 32
#define PC2 33
#define PC3 34
#define PC4 35
#define PC5 36
#define PC6 37
#define PC7 38
#define PC8 39
#define PC9 40
#define PC10 41
#define PC11 42
#define PC12 43
#define PC13 44
#define PC14 45
#define PC15 46

#define PD0 47
#define PD1 48
#define PD2 49
#define PD3 50
#define PD4 51
#define PD5 52
#define PD6 53
#define PD8 54
#define PD9 55

#define PF0 56
#define PF1 57
#endif

#if defined(STM32G070xx)
  // **** Special Pins

#define AN_PA0 PA0
#define AN_PA1 PA1
#define AN_PA2 PA2
#define AN_PA3 PA3
#define AN_PA4 PA4
#define AN_PA5 PA5
#define AN_PA6 PA6
#define AN_PA7 PA7

#define AN_PB0 PB0
#define AN_PB1 PB1
#define AN_PB2 PB2
#define AN_PB10 PB10
#define AN_PB11 PB11
#define AN_PB12 PB12

#define AN_PC4 PC4
#define AN_PC5 PC5

// TIM //
#define TIM15CH1_PA2 PA2
#define TIM15CH2_PA3 PA3
#define TIM14CH1_PA4 PA4
#define TIM16CH1_PA6 PA6
#define TIM17CH1_PA7 PA7
#define TIM1CH1_PA8 PA8
#define TIM1CH2_PA9 PA9
#define TIM1CH3_PA10 PA10
#define TIM1CH4_PA11 PA11

#define TIM3CH3_PB0 PB0
#define TIM3CH4_PB1 PB1
#define TIM1CH2_PB3 PB3
#define TIM3CH1_PB4 PB4
#define TIM3CH2_PB5 PB5
#define TIM1CH3_PB6 PB6
#define TIM16CH1_PB8 PB8
#define TIM17CH1_PB9 PB9
#define TIM15CH1_PB14 PB14
#define TIM15CH2_PB15 PB15

#define TIM15CH1_PC1 PC1
#define TIM15CH2_PC2 PC2
#define TIM3CH1_PC6 PC6
#define TIM3CH2_PC7 PC7
#define TIM1CH1_PC8 PC8
#define TIM1CH2_PC9 PC9
#define TIM1CH3_PC10 PC10
#define TIM1CH4_PC11 PC11
#define TIM14CH1_PC12 PC12

#define TIM16CH1_PD0 PD0
#define TIM17CH1_PD1 PD1

#define TIM14CH1_PF0 PF0

// SPI //
#define SCK2_PA0 PA0
#define SCK1_PA1 PA1
#define MOSI1_PA2 PA2
#define MISO2_PA3 PA3
#define MOSI2_PA4 PA4
#define SCK1_PA5 PA5
#define MISO1_PA6 PA6
#define MOSI1_PA7 PA7
#define CS2_PA8 PA8
#define MISO2_PA9 PA9
#define MOSI2_PA10 PA10
#define MISO1_PA11 PA11
#define MOSI1_PA12 PA12
#define CS1_PA15 PA15

#define CS1_PB0 PB0
#define MISO2_PB2 PB2
#define SCK1_PB3 PB3
#define MISO1_PB4 PB4
#define MOSI1_PB5 PB5
#define MISO2_PB6 PB6
#define MOSI2_PB7 PB7
#define SCK2_PB8 PB8
#define CS2_PB9 PB9
#define SCK2_PB10 PB10
#define MOSI2_PB11 PB11
#define CS2_PB12 PB12
#define SCK2_PB13 PB13
#define MISO2_PB14 PB14
#define MOSI2_PB15 PB15

#define MISO2_PC2 PC2
#define MOSI2_PC3 PC3

#define CS2_PD0 PD0
#define SCK2_PD1 PD1
#define MISO2_PD3 PD3
#define MOSI2_PD4 PD4
#define MISO1_PD5 PD5
#define MOSI1_PD6 PD6
#define SCK1_PD8 PD8
#define CS1_PD9 PD9

// I2C //
#define SCL1_PA9 PA9
#define SDA1_PA10 PA10
#define SCL2_PA11 PA11
#define SDA2_PA12 PA12

#define SCL1_PB6 PB6
#define SDA1_PB7 PB7
#define SCL1_PB8 PB8
#define SDA1_PB9 PB9
#define SCL2_PB10 PB10
#define SDA2_PB11 PB11
#define SCL2_PB13 PB13
#define SDA2_PB14 PB14

// USART //
#define TX4_PA0 PA0
#define RX4_PA1 PA1
#define TX2_PA2 PA2
#define RX2_PA3 PA3
#define TX3_PA5 PA5
#define TX1_PA9 PA9
#define RX1_PA10 PA10
#define RX2_PA15 PA15

#define RX3_PB0 PB0
#define TX3_PB2 PB2
#define TX1_PB6 PB6
#define RX1_PB7 PB7
#define TX3_PB8 PB8
#define RX3_PB9 PB9
#define TX3_PB10 PB10
#define RX3_PB11 PB11

#define TX1_PC4 PC4
#define RX1_PC5 PC5
#define TX3_PC10 PC10
#define RX3_PC11 PC11

#define TX2_PD5 PD5
#define RX2_PD6 PD6
#define TX3_PD8 PD8
#define RX3_PD9 PD9

#endif

#if defined(STM32G071xx)

#define AN_PA0 PA0
#define AN_PA1 PA1
#define AN_PA2 PA2
#define AN_PA3 PA3
#define AN_PA4 PA4
#define AN_PA5 PA5
#define AN_PA6 PA6
#define AN_PA7 PA7

#define AN_PB0 PB0
#define AN_PB1 PB1
#define AN_PB2 PB2
#define AN_PB10 PB10
#define AN_PB11 PB11
#define AN_PB12 PB12

#define AN_PC4 PC4
#define AN_PC5 PC5

// TIM //
#define TIM2CH2_PA1 PA1
#define TIM2CH3_PA2 PA2
#define TIM2CH4_PA3 PA3
#define TIM14CH1_PA4 PA4
#define TIM3CH1_PA6 PA6
#define TIM3CH2_PA7 PA7
#define TIM1CH1_PA8 PA8
#define TIM1CH2_PA9 PA9
#define TIM1CH3_PA10 PA10
#define TIM1CH4_PA11 PA11

#define TIM3CH3_PB0 PB0
#define TIM3CH4_PB1 PB1
#define TIM1CH2_PB3 PB3
#define TIM3CH1_PB4 PB4
#define TIM3CH2_PB5 PB5
#define TIM1CH3_PB6 PB6
#define TIM16CH1_PB8 PB8
#define TIM17CH1_PB9 PB9
#define TIM2CH3_PB10 PB10
#define TIM2CH4_PB11 PB11
#define TIM15CH1_PB14 PB14
#define TIM15CH2_PB15 PB15

#define TIM15CH1_PC1 PC1
#define TIM15CH2_PC2 PC2
#define TIM2CH2_PC5 PC5
#define TIM3CH1_PC6 PC6
#define TIM3CH2_PC7 PC7
#define TIM3CH3_PC8 PC8
#define TIM3CH4_PC9 PC9
#define TIM1CH3_PC10 PC10
#define TIM1CH4_PC11 PC11
#define TIM14CH1_PC12 PC12

#define TIM16CH1_PD0 PD0
#define TIM17CH1_PD1 PD1

#define TIM14CH1_PF0 PF0

// SPI //
#define SCK2_PA0 PA0
#define SCK1_PA1 PA1
#define MOSI1_PA2 PA2
#define MISO2_PA3 PA3
#define MOSI2_PA4 PA4
#define SCK1_PA5 PA5
#define MISO1_PA6 PA6
#define MOSI1_PA7 PA7
#define CS2_PA8 PA8
#define MISO2_PA9 PA9
#define MOSI2_PA10 PA10
#define MISO1_PA11 PA11
#define MOSI1_PA12 PA12
#define CS1_PA15 PA15

#define CS1_PB0 PB0
#define MISO2_PB2 PB2
#define SCK1_PB3 PB3
#define MISO1_PB4 PB4
#define MOSI1_PB5 PB5
#define MISO2_PB6 PB6
#define MOSI2_PB7 PB7
#define SCK2_PB8 PB8
#define CS2_PB9 PB9
#define SCK2_PB10 PB10
#define MOSI2_PB11 PB11
#define CS2_PB12 PB12
#define SCK2_PB13 PB13
#define MISO2_PB14 PB14
#define MOSI2_PB15 PB15

#define MISO2_PC2 PC2
#define MOSI2_PC3 PC3

#define CS2_PD0 PD0
#define SCK2_PD1 PD1
#define MISO2_PD3 PD3
#define MOSI2_PD4 PD4
#define MISO1_PD5 PD5
#define MOSI1_PD6 PD6
#define SCK1_PD8 PD8
#define CS1_PD9 PD9

// I2C //
#define SCL1_PA9 PA9
#define SDA1_PA10 PA10
#define SCL2_PA11 PA11
#define SDA2_PA12 PA12

#define SCL1_PB6 PB6
#define SDA1_PB7 PB7
#define SCL1_PB8 PB8
#define SDA1_PB9 PB9
#define SCL2_PB10 PB10
#define SDA2_PB11 PB11
#define SCL2_PB13 PB13
#define SDA2_PB14 PB14

// USART //
#define TX4_PA0 PA0
#define RX4_PA1 PA1
#define TX2_PA2 PA2
#define RX2_PA3 PA3
#define TX3_PA5 PA5
#define TX1_PA9 PA9
#define RX1_PA10 PA10
#define RX2_PA15 PA15

#define RX3_PB0 PB0
#define TX3_PB2 PB2
#define TX1_PB6 PB6
#define RX1_PB7 PB7
#define TX3_PB8 PB8
#define RX3_PB9 PB9
#define TX3_PB10 PB10
#define RX3_PB11 PB11

#define TX1_PC4 PC4
#define RX1_PC5 PC5
#define TX3_PC10 PC10
#define RX3_PC11 PC11

#define TX2_PD5 PD5
#define RX2_PD6 PD6
#define TX3_PD8 PD8
#define RX3_PD9 PD9

#endif

// **** Other Pin Definitions
#define NOPIN 255
#define UNUSED(__X__) ((void)(__X__))

#ifdef __cplusplus
}
#endif

#endif
