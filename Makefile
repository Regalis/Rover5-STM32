#
# 
# Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# Tested with Arch GNU/Linux @ 3.15.7-1
#
LIBRARY_PATH=../STM32Cube_FW_F4_V1.1.0/

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OC=arm-none-eabi-objcopy
AS=arm-none-eabi-as
AR=arm-none-eabi-ar

MCU_CORE=cortex-m4
MCU=STM32F401xE
OPENOCD_SCRIPT=/usr/share/openocd/scripts/board/st_nucleo_f401re.cfg

CCFLAGS=-Wall -mcpu=$(MCU_CORE) -mlittle-endian -mthumb \
		-ffunction-sections -fdata-sections

CCFLAGS+=$(INCLUDE) $(DEFS) -g -Os -Werror


INCLUDE_PATH=Drivers/CMSIS/Device/ST/STM32F4xx/Include \
		Drivers/CMSIS/Include \
		.

LD_SCRIPT=$(LIBRARY_PATH)Projects/STM32F4xx-Nucleo/Templates/TrueSTUDIO/STM32F4xx-Nucleo/STM32F401CE_FLASH.ld
DEFS=-D$(MCU)
STARTUP_CODE=$(LIBRARY_PATH)Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xe.s
INCLUDE=$(patsubst %, -I$(LIBRARY_PATH)%, $(INCLUDE_PATH))
SOURCES_PATH := .
SOURCES := $(foreach DIR,$(SOURCES_PATH),$(wildcard $(DIR)/*.c $(DIR)/*.cpp))
OBJS := $(patsubst %.c,%.o,$(filter %.c, $(SOURCES)))
OBJS+=$(patsubst %.cpp, %.o, $(filter %.cpp, $(SOURCES)))
OBJS+=startup_stm32f401xe.o

#
# Dependency based section
#

all: main.hex

main.elf: $(OBJS) 
	$(CC) $(CCFLAGS) -T$(LD_SCRIPT) -Wl,--gc-sections $^ -o $@

main.hex: main.elf
	$(OC) -Oihex $< $@

startup_stm32f401xe.o: $(STARTUP_CODE)
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

openocd:
	openocd -f $(OPENOCD_SCRIPT)

cc_cmd:
	@echo $(CC) $(CCFLAGS) -c INPUT -o OUTPUT
	
clean:
	@rm -v $(OBJS) || /bin/true
	@rm -v main.elf main.hex || /bin/true

.PHONY: clean openocd cc_cmd
