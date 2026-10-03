# Toolchain definition
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# Target MCU Architecture Flags
MCU     = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft

# Compiler Flags
CFLAGS  = $(MCU) -Wall -Wextra -Werror -O2 -g3 -std=c99
LDFLAGS = $(MCU) -Tlinker.ld -nostdlib -Wl,-Map=build/firmware.map,--gc-sections

# Source files
SRCS    = main.c startup.c
OBJS    = $(SRCS:%.c=build/%.o)

TARGET  = build/firmware

all: $(TARGET).bin $(TARGET).hex

build:
	mkdir -p build

build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

clean:
	rm -rf build

flash: $(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(TARGET).bin 0x08000000 verify reset exit"

.PHONY: all clean flash
