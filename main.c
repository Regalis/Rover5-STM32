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
#include "bluetooth.h"

#define LED_ON() GPIOA->BSRRL |= (1 << 5)
#define LED_OFF() GPIOA->BSRRH |= (1 << 5)

int main() {
	delay_init();
	engines_init();
	usart_init();
	bluetooth_init();

	/* LED init */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (1 << (5 << 1));
	GPIOA->OSPEEDR |= (3 << (5 << 1));

	_delay_ms(3000);

	usart_puts("\n\rHello world\n\r");

	while (1) {

		char command = bluetooth_getc_timeout(10000);

		if (bluetooth_check_timeout())
			command = 's';

		if (command == 'K' || command == 'f') {
			engines_set_direction(ENGINES_ALL, ENGINES_FORWARD);
		} else if (command == 'J' || command == 'b') {
			engines_set_direction(ENGINES_ALL, ENGINES_BACKWARD);
		} else if (command == 'L' || command == 'r') {
			engines_set_direction(ENGINES_RIGHT, ENGINES_BACKWARD);
			engines_set_direction(ENGINES_LEFT, ENGINES_FORWARD);
		} else if (command == 'H' || command == 'l') {
			engines_set_direction(ENGINES_RIGHT, ENGINES_FORWARD);
			engines_set_direction(ENGINES_LEFT, ENGINES_BACKWARD);
		} else {
			engines_stop(ENGINES_ALL);	
		}
	}

}
