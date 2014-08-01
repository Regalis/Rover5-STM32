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

#ifndef __ENGINES_CONFIG_H__
#define __ENGINES_CONFIG_H__

#define ENGINES_ENCODER_LEFT_CH1 TIM4, GPIOB, 6
#define ENGINES_ENCODER_LEFT_CH2 TIM4, GPIOB, 7
#define ENGINES_ENCODER_RIGHT_CH1 TIM3, GPIOC, 6
#define ENGINES_ENCODER_RIGHT_CH2 TIM3, GPIOC, 7

#define ENGINES_LEFT_PWM TIM2, GPIOA, 1
#define ENGINES_RIGHT_PWM TIM2, GPIOA, 0

#define ENGINES_LEFT_OUTA GPIOC, 0
#define ENGINES_LEFT_OUTB GPIOC, 1
#define ENGINES_RIGHT_OUTA GPIOA, 4
#define ENGINES_RIGHT_OUTB GPIOA, 6

#endif
