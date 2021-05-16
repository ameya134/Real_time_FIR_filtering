# ######################################################
# 
# This is the makefile for TM4C129ENCDT test project
#
# Author:		Ameya Phadke
# Date created:		28th Nov 2020
# Date modified:	9th Jan 2021
#
# ######################################################

include sources.mk
include includes.mk

SRCS = $(sources)
INCDIRS = $(includes)

TARGET = tm4c

CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump

CCFLAGS = -nostdlib --specs=nosys.specs -fno-builtin -Wall -g -Og -mfloat-abi=hard -mcpu=cortex-m4 -mthumb $(verbose)
LDFLAGS = -Wl,-Map=$(TARGET).map -T project.ld



OBJS = $(SRCS:.c=.o)

%.o : %.c
	$(CC) $(INCDIRS) $(CCFLAGS) -c $< -o $@



.PHONY: all
all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(CCFLAGS) $^ -o $(TARGET).elf
	$(OBJDUMP) -S -d $(TARGET).elf > $(TARGET).s

.PHONY: clean
clean:
	rm -f src/*.o src/hal/*.o src/FreeRTOS/*.o test/*.o $(TARGET).elf $(TARGET).map $(TARGET).s

.PHONY: load
load:
	openocd	-f /usr/share/openocd/scripts/board/ek-tm4c1294xl.cfg

