.POSIX:

ABI = arm-none-eabi
CC = $(ABI)-gcc
AR = $(ABI)-ar
OBJCOPY = $(ABI)-objcopy

STMF411 = -mcpu=cortex-m4 -mthumb \
          -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -DCMSIS_device_header='"stm32f4xx.h"' \
          -D__STARTUP_CLEAR_BSS \
          -DARMCM4_FP -DSTM32F411xE \

NUCLEO = -DHSE_VALUE=8000000L

CFLAGS = -std=c99 -Wall -Wextra -g -O0 $(STMF411) $(NUCLEO) \
         -Icmsis/CMSIS/Core/Include \
         -Icmsis/Device/ARM/ARMCM4/Include \
         -Icmsis-dfp-stm32f4/Include \
         -Ifreertos/include \
         -Ifreertos/portable/GCC/ARM_CM4F \
         -I.

SRCS = startup_ARMCM4.c system_stm32f4xx.c \
       croutine.c event_groups.c list.c \
       queue.c stream_buffer.c tasks.c \
       timers.c port.c systask_static_mem.c
OBJS = $(SRCS:.c=.o)

blink.axf blink.bin : blink.c util.o libcmsis_stm32f411xe.a stm32f411ceu6.ld
	$(CC) $(CFLAGS) blink.c util.o $(LDFLAGS) -o blink.axf $(LDLIBS)
	$(OBJCOPY) -O binary blink.axf blink.bin

util.o : util.c util.h
	$(CC) $(CFLAGS) -c util.c

VPATH = cmsis/Device/ARM/ARMCM4/Source/GCC:cmsis-dfp-stm32f4/Source/Templates:freertos:freertos/portable/GCC/ARM_CM4F

libcmsis_stm32f411xe.a : $(OBJS)
	$(AR) r $@ $?

LDFLAGS = -nostdlib -nostartfiles -L. \
          -Wl,--print-memory-usage \
          -Tstm32f411ceu6.ld

LDLIBS = -lcmsis_stm32f411xe

clean :
	rm -f *.[ao]
