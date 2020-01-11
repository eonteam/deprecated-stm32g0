/**
 ******************************************************************************
 * @file    uart_helper.h
 * @author  Pablo Fuentes
 * @version V1.0.1
 * @date    2019
 * @brief   Uart Helper
 ******************************************************************************
 */

#ifndef __UART_HELPER_H_
#define __UART_HELPER_H_

#include "gpio.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_lpuart.h"
#include "stm32g0xx_ll_usart.h"
#include <stdint.h>

#define USART_IT_PE                                                            \
  0x0028U /*!< USART parity error interruption                 */
#define USART_IT_TXE                                                           \
  0x0727U /*!< USART transmit data register empty interruption */
#define USART_IT_TXFNF                                                         \
  0x0727U /*!< USART TX FIFO not full interruption             */
#define USART_IT_TC                                                            \
  0x0626U /*!< USART transmission complete interruption        */
#define USART_IT_RXNE                                                          \
  0x0525U /*!< USART read data register not empty interruption */
#define USART_IT_RXFNE                                                         \
  0x0525U /*!< USART RXFIFO not empty interruption             */
#define USART_IT_IDLE                                                          \
  0x0424U /*!< USART idle interruption                         */
#define USART_IT_ERR                                                           \
  0x0060U /*!< USART error interruption                        */
#define USART_IT_ORE                                                           \
  0x0300U /*!< USART overrun error interruption                */
#define USART_IT_NE                                                            \
  0x0200U /*!< USART noise error interruption                  */
#define USART_IT_FE                                                            \
  0x0100U /*!< USART frame error interruption                  */
#define USART_IT_RXFF                                                          \
  0x183FU /*!< USART RXFIFO full interruption                  */
#define USART_IT_TXFE                                                          \
  0x173EU /*!< USART TXFIFO empty interruption                 */
#define USART_IT_RXFT                                                          \
  0x1A7CU /*!< USART RXFIFO threshold reached interruption     */
#define USART_IT_TXFT                                                          \
  0x1B77U /*!< USART TXFIFO threshold reached interruption     */

#define USART_ISR_MASK 0x1F00U /*!< USART ISR register mask         */
#define USART_ISR_POS 8U       /*!< USART ISR register position     */

#define UART_GET_IT(__UARTX__, __INTERRUPT__)                                  \
  ((((__UARTX__)->ISR &                                                        \
     ((uint32_t)0x01U << (((__INTERRUPT__)&USART_ISR_MASK) >>                  \
                          USART_ISR_POS))) != 0U)                              \
       ? SET                                                                   \
       : RESET)
#define UART_GET_FLAG(__UARTX__, __FLAG__)                                     \
  (((__UARTX__)->ISR & (__FLAG__)) == (__FLAG__))

#define UART_BUFFER_SIZE 256

typedef struct {
  uint8_t buffer[UART_BUFFER_SIZE];
  volatile uint8_t head;
  volatile uint8_t tail;
} UARTRingBuff_t;

__STATIC_INLINE void uart_rb_insert(UARTRingBuff_t *rb, uint8_t b) {
  uint8_t i = (uint8_t)(rb->head + 1) % UART_BUFFER_SIZE;
  if (i != rb->tail) {
    rb->buffer[rb->head] = b;
    rb->head = i;
  }
}

#endif