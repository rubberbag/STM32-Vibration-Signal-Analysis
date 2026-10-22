# project name
TARGET = dsp

# toolchain
CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# MCU Parameters & FPU Hardware Acceleration
CPU   = -mcpu=cortex-m4
THUMB = -mthumb
FPU   = -mfpu=fpv4-sp-d16 -mfloat-abi=hard 

# preprocessor definitions
DEFINES = -DSTM32F407xx 

# include directories
INCLUDES = \
-Iinc \
-IDevice/Include \
-IDrivers/CMSIS/Core/Include \

# compiler flags
CFLAGS = $(CPU)
CFLAGS += $(THUMB)
CFLAGS += $(FPU)   
CFLAGS += -O0
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += $(DEFINES)
CFLAGS += $(INCLUDES)

# source files
SRCS = \
main.c \
$(shell find src -type f -name '*.c') \
Device/Source/system_stm32f4xx.c

# assembly files 
ASMS = Device/Source/startup_stm32f407xx.s

# build directory
OBJ_DIR = build

# object files

OBJS = $(SRCS:%=$(OBJ_DIR)/%.o)
OBJS += $(ASMS:%=$(OBJ_DIR)/%.o)

# linker script
LDSCRIPT = Linker/STM32F407VGTX_FLASH.ld

# linker flags
LDFLAGS = $(CPU)
LDFLAGS += $(THUMB)
LDFLAGS += $(FPU)                          
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(OBJ_DIR)/$(TARGET).map

# output files
ELF = $(OBJ_DIR)/$(TARGET).elf
BIN = $(OBJ_DIR)/$(TARGET).bin
HEX = $(OBJ_DIR)/$(TARGET).hex

# default
all: $(ELF) $(BIN) $(HEX)

# create build directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# compile C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# compile assembly
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(CFLAGS) -c $< -o $@

# link
$(ELF): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

# binary
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# hex
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# renode simulation launch
run:
	renode renode/run.resc
	
# clean workspace
clean:
	rm -rf $(OBJ_DIR)

.PHONY: all clean run
