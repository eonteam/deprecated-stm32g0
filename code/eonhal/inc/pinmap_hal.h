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

// LQF32 Package
#if defined(STM32G070KB)

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

#define PC6 25
#define PC14 26
#define PC15 27

#endif

// LQFP48 Package
#if defined(STM32G070CB)
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

#define PC6 31
#define PC7 32
#define PC13 33
#define PC14 34
#define PC15 35

#define PD0 36
#define PD1 37
#define PD2 38
#define PD3 39

#define PF0 40
#define PF1 41
#endif

// LQF64 Package
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

#ifdef PA0
#define AN_PA0 PA0
#define SCK2_PA0 PA0
#define TX4_PA0 PA0
#endif

#ifdef PA1
#define AN_PA1 PA1
#define SCK1_PA1 PA1
#define RX4_PA1 PA1
#endif

#ifdef PA2
#define AN_PA2 PA2
#define TIM15CH1_PA2 PA2
#define MOSI1_PA2 PA2
#define TX2_PA2 PA2
#endif

#ifdef PA3
#define AN_PA3 PA3
#define TIM15CH2_PA3 PA3
#define MISO2_PA3 PA3
#define RX2_PA3 PA3
#endif

#ifdef PA4
#define AN_PA4 PA4
#define TIM14CH1_PA4 PA4
#define MOSI2_PA4 PA4
#endif

#ifdef PA5
#define AN_PA5 PA5
#define SCK1_PA5 PA5
#define TX3_PA5 PA5
#endif

#ifdef PA6
#define AN_PA6 PA6
#define TIM16CH1_PA6 PA6
#define MISO1_PA6 PA6
#endif

#ifdef PA7
#define AN_PA7 PA7
#define TIM17CH1_PA7 PA7
#define MOSI1_PA7 PA7
#endif

#ifdef PA8
#define TIM1CH1_PA8 PA8
#define CS2_PA8 PA8
#endif

#ifdef PA9
#define TIM1CH2_PA9 PA9
#define MISO2_PA9 PA9
#define SCL1_PA9 PA9
#define TX1_PA9 PA9
#endif

#ifdef PA10
#define TIM1CH3_PA10 PA10
#define MOSI2_PA10 PA10
#define SDA1_PA10 PA10
#define RX1_PA10 PA10
#endif

#ifdef PA11
#define TIM1CH4_PA11 PA11
#define MISO1_PA11 PA11
#define SCL2_PA11 PA11
#endif

#ifdef PA12
#define MOSI1_PA12 PA12
#define SDA2_PA12 PA12
#define RS485DE1_PA12 PA12
#endif

#ifdef PA15
#define CS1_PA15 PA15
#define RX2_PA15 PA15
#endif

#ifdef PB0
#define AN_PB0 PB0
#define TIM3CH3_PB0 PB0
#define CS1_PB0 PB0
#define RX3_PB0 PB0
#endif

#ifdef PB1
#define AN_PB1 PB1
#define TIM3CH4_PB1 PB1
#define RS485DE3_PB1 PB1
#endif

#ifdef PB2
#define AN_PB2 PB2
#define MISO2_PB2 PB2
#define TX3_PB2 PB2
#endif

#ifdef PB3
#define TIM1CH2_PB3 PB3
#define SCK1_PB3 PB3
#define RS485DE1_PB3 PB3
#endif

#ifdef PB4
#define TIM3CH1_PB4 PB4
#define MISO1_PB4 PB4
#endif

#ifdef PB5
#define TIM3CH2_PB5 PB5
#define MOSI1_PB5 PB5
#endif

#ifdef PB6
#define TIM1CH3_PB6 PB6
#define MISO2_PB6 PB6
#define SCL1_PB6 PB6
#define TX1_PB6 PB6
#endif

#ifdef PB7
#define MOSI2_PB7 PB7
#define SDA1_PB7 PB7
#define RX1_PB7 PB7
#endif

#ifdef PB8
#define TIM16CH1_PB8 PB8
#define SCK2_PB8 PB8
#define SCL1_PB8 PB8
#define TX3_PB8 PB8
#endif

#ifdef PB9
#define TIM17CH1_PB9 PB9
#define CS2_PB9 PB9
#define SDA1_PB9 PB9
#define RX3_PB9 PB9
#endif

#ifdef PB10
#define AN_PB10 PB10
#define SCK2_PB10 PB10
#define SCL2_PB10 PB10
#define TX3_PB10 PB10
#endif

#ifdef PB11
#define AN_PB11 PB11
#define MOSI2_PB11 PB11
#define SDA2_PB11 PB11
#define RX3_PB11 PB11
#endif

#ifdef PB12
#define AN_PB12 PB12
#define CS2_PB12 PB12
#endif

#ifdef PB13
#define SCK2_PB13 PB13
#define SCL2_PB13 PB13
#endif

#ifdef PB14
#define TIM15CH1_PB14 PB14
#define MISO2_PB14 PB14
#define SDA2_PB14 PB14
#define RS485DE3_PB14 PB14
#endif

#ifdef PB15
#define TIM15CH2_PB15 PB15
#define MOSI2_PB15 PB15
#endif

#ifdef PC1
#define TIM15CH1_PC1 PC1
#endif

#ifdef PC2
#define TIM15CH2_PC2 PC2
#define MISO2_PC2 PC2
#endif

#ifdef PC3
#define MOSI2_PC3 PC3
#endif

#ifdef PC4
#define AN_PC4 PC4
#define TX1_PC4 PC4
#endif

#ifdef PC5
#define AN_PC5 PC5
#define RX1_PC5 PC5
#endif

#ifdef PC6
#define TIM3CH1_PC6 PC6
#endif

#ifdef PC7
#define TIM3CH2_PC7 PC7
#endif

#ifdef PC8
#define TIM1CH1_PC8 PC8
#endif

#ifdef PC9
#define TIM1CH2_PC9 PC9
#endif

#ifdef PC10
#define TIM1CH3_PC10 PC10
#define TX3_PC10 PC10
#endif

#ifdef PC11
#define TIM1CH4_PC11 PC11
#define RX3_PC11 PC11
#endif

#ifdef PC12
#define TIM14CH1_PC12 PC12
#endif

#ifdef PD0
#define TIM16CH1_PD0 PD0
#define CS2_PD0 PD0
#endif

#ifdef PD1
#define TIM17CH1_PD1 PD1
#define SCK2_PD1 PD1
#endif

#ifdef PD2
#define RS485DE3_PD2 PD2
#endif

#ifdef PD3
#define MISO2_PD3 PD3
#endif

#ifdef PD4
#define MOSI2_PD4 PD4
#define RS485DE2_PD4 PD4
#endif

#ifdef PD5
#define MISO1_PD5 PD5
#define TX2_PD5 PD5
#endif

#ifdef PD6
#define MOSI1_PD6 PD6
#define RX2_PD6 PD6
#endif

#ifdef PD8
#define SCK1_PD8 PD8
#define TX3_PD8 PD8
#endif

#ifdef PD9
#define CS1_PD9 PD9
#define RX3_PD9 PD9
#endif

#ifdef PF0
#define TIM14CH1_PF0 PF0
#endif

#endif

#if defined(STM32G071xx)

#ifdef PA0
#define AN_PA0 PA0
#define SCK2_PA0 PA0
#define TX4_PA0 PA0
#endif

#ifdef PA1
#define AN_PA1 PA1
#define SCK1_PA1 PA1
#define TIM2CH2_PA1 PA1
#define RX4_PA1 PA1
#endif

#ifdef PA2
#define AN_PA2 PA2
#define TIM2CH3_PA2 PA2
#define MOSI1_PA2 PA2
#define TX2_PA2 PA2
#endif

#ifdef PA3
#define AN_PA3 PA3
#define TIM2CH4_PA3 PA3
#define MISO2_PA3 PA3
#define RX2_PA3 PA3
#endif

#ifdef PA4
#define AN_PA4 PA4
#define TIM14CH1_PA4 PA4
#define MOSI2_PA4 PA4
#endif

#ifdef PA5
#define AN_PA5 PA5
#define SCK1_PA5 PA5
#define TX3_PA5 PA5
#endif

#ifdef PA6
#define AN_PA6 PA6
#define TIM3CH1_PA6 PA6
#define MISO1_PA6 PA6
#endif

#ifdef PA7
#define AN_PA7 PA7
#define TIM3CH2_PA7 PA7
#define MOSI1_PA7 PA7
#endif

#ifdef PA8
#define TIM1CH1_PA8 PA8
#define CS2_PA8 PA8
#endif

#ifdef PA9
#define TIM1CH2_PA9 PA9
#define MISO2_PA9 PA9
#define SCL1_PA9 PA9
#define TX1_PA9 PA9
#endif

#ifdef PA10
#define TIM1CH3_PA10 PA10
#define MOSI2_PA10 PA10
#define SDA1_PA10 PA10
#define RX1_PA10 PA10
#endif

#ifdef PA11
#define TIM1CH4_PA11 PA11
#define MISO1_PA11 PA11
#define SCL2_PA11 PA11
#endif

#ifdef PA12
#define MOSI1_PA12 PA12
#define SDA2_PA12 PA12
#define RS485DE1_PA12 PA12
#endif

#ifdef PA15
#define CS1_PA15 PA15
#define RX2_PA15 PA15
#endif

#ifdef PB0
#define AN_PB0 PB0
#define TIM3CH3_PB0 PB0
#define CS1_PB0 PB0
#define RX3_PB0 PB0
#endif

#ifdef PB1
#define AN_PB1 PB1
#define TIM3CH4_PB1 PB1
#define RS485DE3_PB1 PB1
#endif

#ifdef PB2
#define AN_PB2 PB2
#define MISO2_PB2 PB2
#define TX3_PB2 PB2
#endif

#ifdef PB3
#define TIM1CH2_PB3 PB3
#define SCK1_PB3 PB3
#define RS485DE1_PB3 PB3
#endif

#ifdef PB4
#define TIM3CH1_PB4 PB4
#define MISO1_PB4 PB4
#endif

#ifdef PB5
#define TIM3CH2_PB5 PB5
#define MOSI1_PB5 PB5
#endif

#ifdef PB6
#define TIM1CH3_PB6 PB6
#define MISO2_PB6 PB6
#define SCL1_PB6 PB6
#define TX1_PB6 PB6
#endif

#ifdef PB7
#define MOSI2_PB7 PB7
#define SDA1_PB7 PB7
#define RX1_PB7 PB7
#endif

#ifdef PB8
#define TIM16CH1_PB8 PB8
#define SCK2_PB8 PB8
#define SCL1_PB8 PB8
#define TX3_PB8 PB8
#endif

#ifdef PB9
#define TIM17CH1_PB9 PB9
#define CS2_PB9 PB9
#define SDA1_PB9 PB9
#define RX3_PB9 PB9
#endif

#ifdef PB10
#define AN_PB10 PB10
#define SCK2_PB10 PB10
#define SCL2_PB10 PB10
#define TX3_PB10 PB10
#define TIM2CH3_PB10 PB10
#endif

#ifdef PB11
#define AN_PB11 PB11
#define MOSI2_PB11 PB11
#define SDA2_PB11 PB11
#define RX3_PB11 PB11
#define TIM2CH4_PB11 PB11
#endif

#ifdef PB12
#define AN_PB12 PB12
#define CS2_PB12 PB12
#endif

#ifdef PB13
#define SCK2_PB13 PB13
#define SCL2_PB13 PB13
#endif

#ifdef PB14
#define TIM15CH1_PB14 PB14
#define MISO2_PB14 PB14
#define SDA2_PB14 PB14
#define RS485DE3_PB14 PB14
#endif

#ifdef PB15
#define TIM15CH2_PB15 PB15
#define MOSI2_PB15 PB15
#endif

#ifdef PC1
#define TIM15CH1_PC1 PC1
#endif

#ifdef PC2
#define TIM15CH2_PC2 PC2
#define MISO2_PC2 PC2
#endif

#ifdef PC3
#define MOSI2_PC3 PC3
#endif

#ifdef PC4
#define AN_PC4 PC4
#define TX1_PC4 PC4
#endif

#ifdef PC5
#define AN_PC5 PC5
#define RX1_PC5 PC5
#define TIM2CH2_PC5 PC5
#endif

#ifdef PC6
#define TIM3CH1_PC6 PC6
#endif

#ifdef PC7
#define TIM3CH2_PC7 PC7
#endif

#ifdef PC8
#define TIM1CH1_PC8 PC8
#endif

#ifdef PC9
#define TIM1CH2_PC9 PC9
#endif

#ifdef PC10
#define TIM1CH3_PC10 PC10
#define TX3_PC10 PC10
#endif

#ifdef PC11
#define TIM1CH4_PC11 PC11
#define RX3_PC11 PC11
#endif

#ifdef PC12
#define TIM14CH1_PC12 PC12
#endif

#ifdef PD0
#define TIM16CH1_PD0 PD0
#define CS2_PD0 PD0
#endif

#ifdef PD1
#define TIM17CH1_PD1 PD1
#define SCK2_PD1 PD1
#endif

#ifdef PD2
#define RS485DE3_PD2 PD2
#endif

#ifdef PD3
#define MISO2_PD3 PD3
#endif

#ifdef PD4
#define MOSI2_PD4 PD4
#define RS485DE2_PD4 PD4
#endif

#ifdef PD5
#define MISO1_PD5 PD5
#define TX2_PD5 PD5
#endif

#ifdef PD6
#define MOSI1_PD6 PD6
#define RX2_PD6 PD6
#endif

#ifdef PD8
#define SCK1_PD8 PD8
#define TX3_PD8 PD8
#endif

#ifdef PD9
#define CS1_PD9 PD9
#define RX3_PD9 PD9
#endif

#ifdef PF0
#define TIM14CH1_PF0 PF0
#endif

#endif

// **** Other Pin Definitions
#define NOPIN 255
#define UNUSED(__X__) ((void)(__X__))

#ifdef __cplusplus
}
#endif

#endif
