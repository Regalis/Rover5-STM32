#ifndef __USART_H__
#define __USART_H__


/**
 * @brief Configure USART3
 * @return none
 *
 * Configure USART3:
 * 	PD8 as AF7 (USART3_TX) mode push/pull high speed
 * 	USART3 transmitter, receiver enable
 * 	1 bit stop
 * 	no parity check
 * 	baud rate 9600
 */
void usart_init();

void usart_putc(char c);

void usart_puts(char *string);

char usart_getc();

#endif
