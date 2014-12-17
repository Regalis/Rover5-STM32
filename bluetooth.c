/*
* 
* Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
*/

#include <stm32f4xx.h>

#include "bluetooth.h"
#include "delay.h"

#define BLUETOOTH_RX 12

static uint8_t timeout;

inline void bluetooth_init() {
	/* enable USART6 in APB2 */
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	/* enbale GPIOA in AHB1 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* GPIO configuration */

	/* reset PA12 mode */
	GPIOA->MODER &= ~(3 << (BLUETOOTH_RX << 1));
	/* PA3 in Alternate function mode */
	GPIOA->MODER |= (2 << (BLUETOOTH_RX << 1));
	/* push/pull */
	GPIOA->OTYPER &= ~(1 << 2);
	/* high speed */
	GPIOA->OSPEEDR |= (3 << (BLUETOOTH_RX << 1));
	/* no pull-up/pull-down */
	GPIOA->PUPDR &= ~(3 << (BLUETOOTH_RX << 1));
	/* reset PA10 AF mode */
	GPIOA->AFR[1] &= ~(0x000F0000);
	/* set USART6_RX AF (AF8) */
	GPIOA->AFR[1] |= 0x00080000;

	/* USART6 configuration */

	/* enable receiver */
	USART6->CR1 |= USART_CR1_RE | USART_CR1_TE;
	/* 1 bit stop */
	USART6->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);
	/* USARTDIV = 546.875 (baud rate 9600) */
	USART6->BRR |= 0x222E;
	/* enable USART6 */
	USART6->CR1 |= USART_CR1_UE;
}

inline void bluetooth_putc(char c) {
	while (!(USART6->SR & USART_SR_TC));
	USART6->DR = (uint8_t)c;
}

void bluetooth_puts(char *buffer) {
	while (*buffer != '\0') {
		bluetooth_putc(*(buffer++));
	}
}

char bluetooth_getc() {
	while (!(USART6->SR & USART_SR_RXNE));
	return (uint8_t)(USART6->DR & (uint8_t)0x00ff);
}

char bluetooth_getc_timeout(uint16_t timeout_ms) {
	timeout = 0x00;
	while (!(USART6->SR & USART_SR_RXNE) && timeout) {
		--timeout_ms;
		_delay_ms(1);
	}
	if (!timeout_ms) {
		timeout = 0xff;
		return 0x00;
	}
	return (uint8_t)(USART6->DR & (uint8_t)0x00ff);

}

uint8_t bluetooth_check_timeout() {
	return timeout;
}
