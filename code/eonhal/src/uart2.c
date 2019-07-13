/**
  ******************************************************************************
  * @file    uart2.c 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   UART Functions
  ******************************************************************************
*/

#include "uart2.h"
#include "uart_helper.h"

#if (defined(USART2) || defined(UART2))
/** 
 ===============================================================================
              ##### Global Static Variables #####
 ===============================================================================
 */

static UARTRingBuff_t urb;

/** 
 ===============================================================================
              ##### Interrupt #####
 ===============================================================================
 */

void USART2_IRQHandler(void)
{
	uint8_t ch = 0;
	if (UART_GET_IT(USART2, USART_IT_RXNE) != 0)
	{
		ch = (uint8_t)LL_USART_ReceiveData8(USART2);
		uart_rb_insert(&urb, ch);
	}
}

/** 
 ===============================================================================
              ##### Initialization Functions #####
 ===============================================================================
 */

void uart2_init(uint32_t baudrate, pin_t tx, pin_t rx)
{

	LL_USART_InitTypeDef USART_InitStruct;
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

	gpio_modeUART(tx);
	gpio_modeUART(rx);

	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = baurate;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_8;
	LL_USART_Init(USART2, &USART_InitStruct);
	
 	LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(USART2);

	LL_USART_ConfigAsyncMode(USART2);

	LL_USART_Enable(USART2);

	LL_USART_EnableIT_RXNE(USART2);
}

void uart2_off(void)
{
	LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
	NVIC_DisableIRQ(USART2_IRQn);
	LL_USART_Disable(USART2);
	LL_USART_DisableIT_RXNE(USART2);
}

/** 
 ===============================================================================
              ##### Write Functions #####
 ===============================================================================
 */

void uart2_write(unsigned char c)
{
	while (UART_GET_FLAG(USART2, LL_USART_ISR_TXE_TXFNF) == 0)
		;
	LL_USART_TransmitData8(USART2, c);
}

/** 
 ===============================================================================
              ##### Print Functions #####
 ===============================================================================
 */

void uart2_print(const char *s)
{
	unsigned char i = 0;
	while (s[i] != '\0')
	{
		uart2_write(s[i++]);
	}
}

void uart2_printArray(char s[])
{
	unsigned char i = 0;
	while (s[i] != '\0')
	{
		uart2_write(s[i++]);
	}
}

void uart2_println(const char *s)
{
	uart2_print(s);
	uart2_write('\r');
	uart2_write('\n');
}

void uart2_printIntBase(int64_t n, uint8_t base)
{
	unsigned char buf[10];
	uint16_t i = 0;
	if (n == 0)
	{
		uart2_write('0');
	}

	if (n < 0)
	{
		uart2_write('-');
		n = -n;
	}

	while (n > 0)
	{
		buf[i++] = n % base;
		n /= base;
	}

	for (; i > 0; i--)
	{
		uart2_write((char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
	}
}

void uart2_printlnIntBase(int64_t n, uint8_t base)
{
	uart2_printIntBase(n, base);
	uart2_write('\r');
	uart2_write('\n');
}

void uart2_printNum(int64_t n, uint8_t isfloat)
{
	uint32_t int_part;
	uint8_t remainder;

	// Handle negative numbers
	if (n < 0)
	{
		uart2_write('-');
		n = -n;
	}

	if (!isfloat)
	{
		uart2_printIntBase(n, 10);
		return;
	}

	remainder = n % 100;
	int_part = (uint32_t)((n - remainder) / 100);
	uart2_printIntBase(int_part, 10);
	uart2_write('.');
	uart2_printIntBase(remainder, 10);
}

void uart2_printlnNum(int64_t n, uint8_t isfloat)
{
	uart2_printNum(n, isfloat);
	uart2_write('\r');
	uart2_write('\n');
}

/** 
 ===============================================================================
              ##### Read Functions #####
 ===============================================================================
 */

uint8_t uart2_available(void)
{
	return ((uint8_t)(UART_BUFFER_SIZE + urb.head - urb.tail)) % UART_BUFFER_SIZE;
}

int uart2_read(void)
{
	if (urb.head == urb.tail)
	{
		return -1;
	}
	else
	{
		uint8_t c = urb.buffer[urb.tail];
		urb.tail = (uint8_t)(urb.tail + 1) % UART_BUFFER_SIZE;
		return c;
	}
}

int uart2_peek(void)
{
	if (urb.head == urb.tail)
		return -1;
	else
		return urb.buffer[urb.tail];
}

void uart2_readUntil(char buffer[], uint8_t terminator)
{
	uint8_t i = 0;
	char c = 0;
	if (uart2_available())
		c = uart2_read();
	else
		return;
	while (c != terminator)
	{
		if (uart2_available() > 0)
		{
			buffer[i] = c;
			c = uart2_read();
			i++;
		}
	}
}

#endif
