TARGET = main

# Define the linker script location and chip architecture.
LD_SCRIPT = linker.ld
MCU_SPEC  = cortex-m0

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr
CC = $(TOOLCHAIN)/bin/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/bin/arm-none-eabi-as
LD = $(TOOLCHAIN)/bin/arm-none-eabi-ld
OC = $(TOOLCHAIN)/bin/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/bin/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/bin/arm-none-eabi-size

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall

# C compilation directives.
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -g -fstack-usage
CFLAGS += -I inc/cmsis
CFLAGS += -I inc
CFLAGS += -O0
CFLAGS += -fmessage-length=0
CFLAGS += --specs=nosys.specs

# Linker directives.
LSCRIPT = ./$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += -nostdlib
LFLAGS += -T$(LSCRIPT)

OBJS += $(patsubst %.s, %.o, $(shell ls src/*.s))
OBJS += $(patsubst %.c, %.o, $(shell ls src/*.c))
OBJS += $(patsubst %.c, %.o, $(shell ls src/**/*.c))
OBJS := $(patsubst src/%, bin/%, $(OBJS))

.PHONY: all
all: bin/$(TARGET).bin bin/$(TARGET).elf

bin/%.o: src/%.s
	mkdir -p $(shell dirname $@)
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

bin/%.o: src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

bin/$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

bin/$(TARGET).bin: bin/$(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -rf bin/*