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

#include "usart.h"

#define USART_TX 2
#define USART_RX 3

inline void usart_init() {
	/* enable USART2 in APB1 */
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	/* enbale GPIOA in AHB1 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* GPIO configuration */

	/* reset PA2 mode */
	GPIOA->MODER &= ~(3 << (USART_TX << 1));
	/* PA2 in Alternate function mode */
	GPIOA->MODER |= (2 << (USART_TX << 1));
	/* push/pull */
	GPIOA->OTYPER &= ~(1 << 2);
	/* high speed */
	GPIOA->OSPEEDR |= (3 << (USART_TX << 1));
	/* no pull-up/pull-down */
	GPIOA->PUPDR &= ~(3 << (USART_TX << 1));
	/* reset PA2 AF mode */
	GPIOA->AFR[0] &= ~(0x00000F00);
	/* set USART2_TX AF (AF7) */
	GPIOA->AFR[0] |= 0x00000700;

	/* reset PA3 mode */
	GPIOA->MODER &= ~(3 << (USART_RX << 1));
	/* PA3 in Alternate function mode */
	GPIOA->MODER |= (2 << (USART_RX << 1));
	/* push/pull */
	GPIOA->OTYPER &= ~(1 << 2);
	/* high speed */
	GPIOA->OSPEEDR |= (3 << (USART_RX << 1));
	/* no pull-up/pull-down */
	GPIOA->PUPDR &= ~(3 << (USART_RX << 1));
	/* reset PA3 AF mode */
	GPIOA->AFR[0] &= ~(0x0000F000);
	/* set USART2_RX AF (AF7) */
	GPIOA->AFR[0] |= 0x00007000;

	/* USART2 configuration */

	/* enable transmitter, receiver */
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
	/* 1 bit stop */
	USART2->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);
	/* USARTDIV = 273.4375 (baud rate 9600) */
	USART2->BRR |= 0x1117;
	/* enable USART2 */
	USART2->CR1 |= USART_CR1_UE;
}

inline void usart_putc(char c) {
	while (!(USART2->SR & USART_SR_TC));
	USART2->DR = (uint8_t)c;
}

void usart_puts(char *buffer) {
	while (*buffer != '\0') {
		usart_putc(*(buffer++));
	}
}

char usart_getc() {
	while (!(USART2->SR & USART_SR_RXNE));
	return (char)USART2->DR;
}
