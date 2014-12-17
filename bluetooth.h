#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__


/**
 * @brief Configure USART3
 * @return none
 *
 * Configure USART6:
 * 	PA12 as AF8 (USART6_RX) mode push/pull high speed
 * 	USART6 receiver enable
 * 	1 bit stop
 * 	no parity check
 * 	baud rate 9600
 */
void bluetooth_init();

void bluetooth_putc(char c);

void bluetooth_puts(char *string);

char bluetooth_getc();

char bluetooth_getc_timeout(uint16_t timeout_ms);

uint8_t bluetooth_check_timeout();

#endif

