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

#ifndef __ENGINES_H__
#define __ENGINES_H__

#define ENGINES_LEFT 0xF0
#define ENGINES_RIGHT 0x0F
#define ENGINES_ALL 0xFF
#define ENGINES_FORWARD 0xF0
#define ENGINES_BACKWARD 0x0F
#define ENGINES_STOP 0xFF

/**
 * @brief Initialize engines and encoders
 */
inline void engines_init();

/**
 * @brief Set engine(s) direction
 */
void engines_set_direction(uint8_t engine, uint8_t direction);

/**
 * @brief stop engine(s)
 */
void engines_stop(uint8_t engine);

/**
 * @brief Set engine(s) speed
 */
void engines_set_speed(uint8_t engines, uint8_t speed);

uint16_t engines_read_position(uint8_t encoder);
void engines_set_encoder_value(uint8_t engine, uint16_t value);

#endif
