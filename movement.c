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

#include "movement.h"
#include "engines.h"

#define MOVEMENT_PI 31415UL
#define MOVEMENT_PI_DIV 10000UL
#define MOVEMENT_WHEEL_R_MM 30UL
#define MOVEMENT_ROTATION_ENC 333UL
#define MOVEMENT_ROTATION_MM ((2UL * MOVEMENT_PI * MOVEMENT_WHEEL_R_MM) / MOVEMENT_PI_DIV)
#define MOVEMENT_ROBOT_R_MM 625UL
#define MOVEMENT_ROBOT_R_DIV 10UL
#define MOVEMENT_ROBOT_ROTATION_MM ((2UL * MOVEMENT_PI * MOVEMENT_ROBOT_R_MM) / MOVEMENT_ROBOT_R_DIV / MOVEMENT_PI_DIV)

inline uint32_t movement_get_encoder_offset(uint32_t mm) {
	return ((MOVEMENT_ROTATION_ENC * mm) / MOVEMENT_ROTATION_MM);
}

/**
 * Configure engine to move forward or backward
 * @param engine ENGINES_LEFT or ENGINES_RIGHT
 * @return encoder stop position
 */
inline uint32_t movement_engine_move(uint8_t engine, uint8_t direction,  uint32_t mm) {
	if (direction == ENGINES_FORWARD) {
		uint32_t offset = 100 + movement_get_encoder_offset(mm);
		engines_set_encoder_value(engine, 100);
		engines_set_direction(engine, ENGINES_FORWARD);
		return offset;
	} else {
		uint32_t offset = (0xFFFF - 100) - movement_get_encoder_offset(mm);
		engines_set_encoder_value(engine, 0xFFFF - 100);
		engines_set_direction(engine, ENGINES_BACKWARD);
		return offset;
	}
}

void movement_forward(uint32_t cm) {
	uint32_t offset = 100 + movement_get_encoder_offset(cm * 10);
	engines_set_encoder_value(ENGINES_ALL, 100);
	engines_set_direction(ENGINES_ALL, ENGINES_FORWARD);
	while (engines_read_position(ENGINES_LEFT) < offset && engines_read_position(ENGINES_RIGHT) < offset);
	engines_stop(ENGINES_ALL);
}

void movement_backward(uint32_t cm) {
	uint32_t offset = (0xFFFF - 100) - movement_get_encoder_offset(cm * 10);
	engines_set_encoder_value(ENGINES_ALL, 0xFFFF - 100);
	engines_set_direction(ENGINES_ALL, ENGINES_BACKWARD);
	while (engines_read_position(ENGINES_LEFT) > offset && engines_read_position(ENGINES_RIGHT) > offset);
	engines_stop(ENGINES_ALL);
}

void movement_rotate(int16_t angle) {
	uint8_t engine_left_direction = (angle < 0 ? ENGINES_BACKWARD : ENGINES_FORWARD);
	angle = (angle < 0 ? (angle * -1) : angle);
	uint32_t wheels_mm = (MOVEMENT_ROBOT_ROTATION_MM);
	uint32_t engine_left_stop = movement_engine_move(ENGINES_LEFT, engine_left_direction, wheels_mm);
	uint32_t engine_right_stop = movement_engine_move(ENGINES_RIGHT, ~(engine_left_direction), wheels_mm);
	while (engines_read_position(ENGINES_LEFT) != engine_left_stop && engines_read_position(ENGINES_RIGHT) != engine_right_stop);
	engines_stop(ENGINES_ALL);
}

inline void movement_stop() {
	engines_stop(ENGINES_ALL);
}
