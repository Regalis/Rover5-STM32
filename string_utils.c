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

#include <stdint.h>
#include <string.h>
#include "string_utils.h"

void s_ftoa(float f, char * buffer) {
	int32_t base = (int32_t)f;
	int32_t frac = (int32_t)((f - base) * 100);
	if (frac < 0)
		frac *= -1;
	itoa(base, buffer);
	size_t len = strlen(buffer);
	buffer[len] = '.';
	itoa(frac, buffer + (len + 1));
}

void reverse(char *s) {
     int i, j;
     char c;
	 size_t len = strlen(s);
     for (i = 0, j = len-1; i < j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(int n, char *s) {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

