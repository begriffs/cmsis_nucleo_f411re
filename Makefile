.POSIX:

ABI = arm-none-eabi
CC = $(ABI)-gcc
AR = $(ABI)-ar
OBJCOPY = $(ABI)-objcopy

STMF411 = -mcpu=cortex-m4 -mthumb \
          -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -DARMCM4_FP -DSTM32F411xE \
		  -DHSE_VALUE=8000000L

CFLAGS = -std=c99 -Wall -Wextra -g -Os $(STMF411) \
         -Icmsis/CMSIS/Core/Include \
		 -Icmsis/Device/ARM/ARMCM4/Include \
         -Icmsis-dfp-stm32f4/Include

SRCS = startup_ARMCM4.c system_stm32f4xx.c
OBJS = $(SRCS:.c=.o)

blink.axf blink.bin : blink.c libcmsis_stm32f411xe.a stm32f411ceu6.ld
	$(CC) $(CFLAGS) blink.c $(LDFLAGS) -o blink.axf $(LDLIBS)
	$(OBJCOPY) -O binary blink.axf blink.bin

VPATH = cmsis/Device/ARM/ARMCM4/Source/GCC:cmsis-dfp-stm32f4/Source/Templates

libcmsis_stm32f411xe.a : $(OBJS)
	$(AR) r $@ $?

LDFLAGS = -nostdlib -nostartfiles -L. \
          -Wl,--print-memory-usage \
          -Tstm32f411ceu6.ld

LDLIBS = -lcmsis_stm32f411xe

clean :
	rm -f *.[ao]
