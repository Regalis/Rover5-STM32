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
#include "engines.h"
#include "delay.h"
#include "usart.h"
#include "string_utils.h"
#include "movement.h"

#define LED_ON() GPIOA->BSRRL |= (1 << 5)
#define LED_OFF() GPIOA->BSRRH |= (1 << 5)

int main() {
	delay_init();
	engines_init();
	usart_init();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (1 << (5 << 1));
	GPIOA->OSPEEDR |= (3 << (5 << 1));

	_delay_ms(3000);

	usart_puts("\n\rHello world\n\r");

	char buffer[100];
	uint16_t position;
	//uint8_t i = 0xFF;
	//engines_set_direction(ENGINES_ALL, ENGINES_FORWARD);
	while (1) {
		position = engines_read_position(ENGINES_LEFT);
		itoa(position, buffer);
		usart_puts("LEFT: ");
		usart_puts(buffer);
		usart_puts(" RIGHT: ");
		position = engines_read_position(ENGINES_RIGHT);
		itoa(position, buffer);
		usart_puts(buffer);
		usart_puts("\r");

		_delay_ms(2000);
		movement_forward(10);
		_delay_ms(2000);
		movement_backward(10);
		_delay_ms(2000);
		movement_rotate(90);
		_delay_ms(2000);
		movement_rotate(-90);
		_delay_ms(2000);
		

		_delay_ms(1000);
		LED_ON();
		_delay_ms(1000);
		LED_OFF();
	}

}
