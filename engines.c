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
#include "engines_config.h"

#define ENCODER_TIM(TIM, GPIO, PIN) TIM
#define ENCODER_GPIO(TIM, GPIO, PIN) GPIO
#define ENCODER_PIN(TIM, GPIO, PIn) PIN

/* helpers (configuration resolver) */
#define __E_OUT_GPIO(GPIO, PIN) GPIO
#define __E_OUT_PIN(GPIO, PIN) PIN
#define __E_PWM_GPIO(TIM, GPIO, PIN) GPIO
#define __E_PWM_PIN(TIM, GPIO, PIN) PIN 

#define E_OUT_GPIO(CONFIG) __E_OUT_GPIO(CONFIG)
#define E_OUT_PIN(CONFIG) __E_OUT_PIN(CONFIG)
#define E_PWM_GPIO(CONFIG) __E_PWM_GPIO(CONFIG)
#define E_PWM_PIN(CONFIG) __E_PWM_PIN(CONFIG)


inline void engines_init() {

	/* 
	 * Initialize encoders
	 * Connection block:
	 * 	PC6 left wheel encoder input A
	 * 	PC7 left wheel encoder input B
	 * 	PB6 right wheel encoder input A
	 * 	PB7 right wheel encoder input B
	 *
	 * 	PA0 engine right PWM output (TIM2_CH1)
	 * 	PA1 engine left PWM output (TIM2_CH2)
	 *
	 * 	PA4 engine right output A
	 * 	PA6 engine right output B
	 * 	PC0 engine left output A
	 * 	PC1 engine left output B
	 */

	/* Enable TIM3 and TIM4 clock */
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;
	/* Enable GPIOA, GPIOB, GPIOC clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN |
					RCC_AHB1ENR_GPIOAEN;

	/* PWN tmp init (always on) */
	E_PWM_GPIO(ENGINES_LEFT_PWM)->MODER |= (1 << (E_PWM_PIN(ENGINES_LEFT_PWM) << 1));
	E_PWM_GPIO(ENGINES_LEFT_PWM)->OSPEEDR |= (3 << (E_PWM_PIN(ENGINES_LEFT_PWM) << 1));
	E_PWM_GPIO(ENGINES_LEFT_PWM)->BSRRL |= (1 << E_PWM_PIN(ENGINES_LEFT_PWM));
	E_PWM_GPIO(ENGINES_RIGHT_PWM)->MODER |= (1 << (E_PWM_PIN(ENGINES_RIGHT_PWM) << 1));
	E_PWM_GPIO(ENGINES_RIGHT_PWM)->OSPEEDR |= (3 << (E_PWM_PIN(ENGINES_RIGHT_PWM) << 1));
	E_PWM_GPIO(ENGINES_RIGHT_PWM)->BSRRL |= (1 << E_PWM_PIN(ENGINES_RIGHT_PWM));

	/* left engine output init */
	E_OUT_GPIO(ENGINES_LEFT_OUTA)->MODER |= (1 << (E_OUT_PIN(ENGINES_LEFT_OUTA) << 1));
	E_OUT_GPIO(ENGINES_LEFT_OUTA)->OSPEEDR |= (3 << (E_OUT_PIN(ENGINES_LEFT_OUTA) << 1));
	E_OUT_GPIO(ENGINES_LEFT_OUTA)->BSRRL |= (1 << E_OUT_PIN(ENGINES_LEFT_OUTA));
	E_OUT_GPIO(ENGINES_LEFT_OUTB)->MODER |= (1 << (E_OUT_PIN(ENGINES_LEFT_OUTB) << 1));
	E_OUT_GPIO(ENGINES_LEFT_OUTB)->OSPEEDR |= (3 << (E_OUT_PIN(ENGINES_LEFT_OUTB) << 1));
	E_OUT_GPIO(ENGINES_LEFT_OUTB)->BSRRL |= (1 << E_OUT_PIN(ENGINES_LEFT_OUTB));
	/* right engine output init */
	E_OUT_GPIO(ENGINES_RIGHT_OUTA)->MODER |= (1 << (E_OUT_PIN(ENGINES_RIGHT_OUTA) << 1));
	E_OUT_GPIO(ENGINES_RIGHT_OUTA)->OSPEEDR |= (3 << (E_OUT_PIN(ENGINES_RIGHT_OUTA) << 1));
	E_OUT_GPIO(ENGINES_RIGHT_OUTA)->BSRRL |= (1 << E_OUT_PIN(ENGINES_RIGHT_OUTA));
	E_OUT_GPIO(ENGINES_RIGHT_OUTB)->MODER |= (1 << (E_OUT_PIN(ENGINES_RIGHT_OUTB) << 1));
	E_OUT_GPIO(ENGINES_RIGHT_OUTB)->OSPEEDR |= (3 << (E_OUT_PIN(ENGINES_RIGHT_OUTB) << 1));
	E_OUT_GPIO(ENGINES_RIGHT_OUTB)->BSRRL |= (1 << E_OUT_PIN(ENGINES_RIGHT_OUTB));

	/* PC6, PC7 as AF2 (TIM3) PP hi-speed */
	GPIOC->MODER |= (2 << (6 << 1)) | (2 << (7 << 1));
	GPIOC->OSPEEDR |= (2 << (6 << 1)) | (2 << (7 << 1));
	GPIOC->AFR[0] |= (0x02 << (6 << 2)) | (0x02 << (7 << 2));
	/* PB6, PB7 as AF2 (TIM4) PP hi-speed */
	GPIOB->MODER |= (2 << (6 << 1)) | (2 << (7 << 1));
	GPIOB->OSPEEDR |= (2 << (6 << 1)) | (2 << (7 << 1));
	GPIOB->AFR[0] |= (0x02 << (6 << 2)) | (0x02 << (7 << 2));

	/* TIM3 in quadrature encoder mode III with 2 inputs */
	TIM3->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
	/* TI1FP1 mapped on TI1, TI2FP2 mapped on TI2 */
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
	/* Disable prescaler (count every tick) */
	TIM3->PSC = 0x0000;
	/* Set maximum auto-reload value */
	TIM3->ARR = 0xFFFF;
	/* Set start position */
	TIM3->CNT = (0xFFFF >> 1);
	/* Enable counter */
	TIM3->CR1 |= TIM_CR1_CEN;

	/* TIM4 in quadrature encoder mode III with 2 inputs */
	TIM4->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
	/* TI1FP1 mapped on TI1, TI2FP2 mapped on TI2 */
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
	/* Disable prescaler (count every tick) */
	TIM4->PSC = 0x0000;
	/* Set maximum auto-reload value */
	TIM4->ARR = 0xFFFF;
	/* Set start position */
	TIM4->CNT = (0xFFFF >> 1);
	/* Enable counter */
	TIM4->CR1 |= TIM_CR1_CEN;
}

inline uint16_t engines_read_position(uint8_t engine) {
	if (engine == ENGINES_LEFT)
		return TIM4->CNT;
	return TIM3->CNT;
}

void engines_set_encoder_value(uint8_t engine, uint16_t value) {
	if (engine & ENGINES_LEFT)
		TIM4->CNT = value;
	if (engine & ENGINES_RIGHT)
		TIM3->CNT = value;
}

void engines_stop(uint8_t engine) {
	if (engine & ENGINES_LEFT) {
		E_OUT_GPIO(ENGINES_LEFT_OUTA)->BSRRL |= (1 << E_OUT_PIN(ENGINES_LEFT_OUTA));
		E_OUT_GPIO(ENGINES_LEFT_OUTB)->BSRRL |= (1 << E_OUT_PIN(ENGINES_LEFT_OUTB));
	}
	if (engine & ENGINES_RIGHT) {
		E_OUT_GPIO(ENGINES_RIGHT_OUTA)->BSRRL |= (1 << E_OUT_PIN(ENGINES_RIGHT_OUTA));
		E_OUT_GPIO(ENGINES_RIGHT_OUTB)->BSRRL |= (1 << E_OUT_PIN(ENGINES_RIGHT_OUTB));
	}
}

void engines_set_direction(uint8_t engine, uint8_t direction) {
	if (engine & ENGINES_RIGHT) {
		engines_stop(ENGINES_RIGHT);
		if (direction == ENGINES_FORWARD) {
			E_OUT_GPIO(ENGINES_RIGHT_OUTA)->BSRRH |= (1 << E_OUT_PIN(ENGINES_RIGHT_OUTA));
		} else {
			E_OUT_GPIO(ENGINES_RIGHT_OUTB)->BSRRH |= (1 << E_OUT_PIN(ENGINES_RIGHT_OUTB));
		}
	}
	if (engine & ENGINES_LEFT) {
		engines_stop(ENGINES_LEFT);
		if (direction == ENGINES_FORWARD) {
			E_OUT_GPIO(ENGINES_LEFT_OUTA)->BSRRH |= (1 << E_OUT_PIN(ENGINES_LEFT_OUTA));
		} else {
			E_OUT_GPIO(ENGINES_LEFT_OUTB)->BSRRH |= (1 << E_OUT_PIN(ENGINES_LEFT_OUTB));
		}
	}
}


