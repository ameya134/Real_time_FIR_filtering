# #################################################
# 
# This makefile contains the variable for all the
# source files present in this project
#
# Author:		Ameya Phadke
# Date created:		28th Nov 2020
# Date modified:	9th Jan 2021
#
# ################################################



sources = src/main.c \
	  src/startup_gcc.c \
	  src/system_tm4c.c \
	  src/FreeRTOS/tasks.c \
	  src/FreeRTOS/list.c \
	  src/FreeRTOS/timers.c \
	  src/FreeRTOS/queue.c \
	  src/FreeRTOS/heap_1.c \
	  src/FreeRTOS/port.c \
	  src/string.c \
	  src/hal/bsp.c \
	  src/hal/uart.c \
	  src/hal/pwm.c \
	  src/hal/timer0.c \
	  src/hal/adc.c


