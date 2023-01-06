.POSIX:

ABI     = arm-none-eabi
CC      = $(ABI)-gcc
AR      = $(ABI)-ar
OBJCOPY = $(ABI)-objcopy

CFLAGS_ARCH  = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS_CMSIS = -DCMSIS_device_header='"stm32f4xx.h"' \
               -DARMCM4_FP -DSTM32F411xE \
               -DHSE_VALUE=8000000L \
               -D__STARTUP_CLEAR_BSS \
               -Icmsis/CMSIS/Core/Include \
               -Icmsis/Device/ARM/ARMCM4/Include \
               -Icmsis-dfp-stm32f4/Include

CFLAGS_RTOS  = -Ifreertos/include \
               -Ifreertos/portable/GCC/ARM_CM4F

CFLAGS       = -std=c99 -Wall -Wextra -g -O0 \
               $(CFLAGS_ARCH) $(CFLAGS_CMSIS) $(CFLAGS_RTOS) \
               -I.

LDFLAGS = -nostdlib -L. \
          -Tnucleof411re.ld \
          -Wl,--print-memory-usage

LDLIBS  = -lkernel

blink.axf blink.bin : blink.c util.o libkernel.a nucleof411re.ld
	$(CC) $(CFLAGS) -pedantic blink.c util.o $(LDFLAGS) -o blink.axf $(LDLIBS)
	$(OBJCOPY) -O binary blink.axf blink.bin

util.o : util.c util.h
	$(CC) $(CFLAGS) -c util.c

.PHONY:
clean :
	rm -f *.[ao]

## libkernel ##########################################################

VPATH = cmsis/Device/ARM/ARMCM4/Source/GCC:cmsis-dfp-stm32f4/Source/Templates:freertos:freertos/portable/GCC/ARM_CM4F

SRCS_CMSIS = startup_ARMCM4.c system_stm32f4xx.c
SRCS_RTOS  = croutine.c event_groups.c list.c \
             queue.c stream_buffer.c tasks.c \
             timers.c port.c systask_static_mem.c

OBJS_CMSIS  = $(SRCS_CMSIS:.c=.o)
OBJS_RTOS   = $(SRCS_RTOS:.c=.o)
OBJS_KERNEL = $(OBJS_CMSIS) $(OBJS_RTOS)

$(OBJS_RTOS) : FreeRTOSConfig.h
$(OBJS_KERNEL) : Makefile

libkernel.a : $(OBJS_KERNEL)
	$(AR) r $@ $?
