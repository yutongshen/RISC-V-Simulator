ELF_NAME := main

export CROSS_PREFIX ?= riscv64-unknown-elf-
export RISCV_GCC ?= $(CROSS_PREFIX)gcc
export RISCV_OBJDUMP ?= $(CROSS_PREFIX)objdump -xsd
export RISCV_OBJCOPY ?= $(CROSS_PREFIX)objcopy -O binary

LDFILE := link.ld
CFLAGS := -march=$(march) -mabi=$(mabi)
LDFLAGS := -static -nostdlib -nostartfiles -march=$(march) -mabi=$(mabi) -T$(LDFILE) -lgcc


SRC_C   := $(wildcard *.c)
OBJ_C   := $(patsubst %.c,%.o,$(SRC_C))
SRC_S   := $(wildcard *.S)
OBJ_S   := $(patsubst %.S,%.o,$(SRC_S))
BMP     := $(wildcard *.bmp)
OBJ_BMP := $(patsubst %.bmp,%.o,$(BMP))
SRC     := $(SRC_C) $(SRC_S)
OBJ     := $(OBJ_C) $(OBJ_S) $(OBJ_BMP)

.SUFFIXES: .o .S .c

.PHONY: all

all: build_elf build_log build_hex

build_elf: $(OBJ) | $(LDFILE)
	$(RISCV_GCC) $^ $(LDFLAGS) -o $(ELF_NAME)

build_log: $(ELF_NAME)
	$(RISCV_OBJDUMP) $< > $(ELF_NAME).log

build_hex: $(ELF_NAME)
	$(RISCV_OBJCOPY) $< -j .text0 --change-addresses 0 boot.bin
	$(RISCV_OBJCOPY) $< -R .text0 --change-addresses 0x20000000 main.bin

%.o: %.bmp
	$(RISCV_OBJCOPY) -I binary -O elf64-littleriscv -B riscv:rv64 $^ $@

%.o: %.S
	$(RISCV_GCC) -c $(CFLAGS) $^

%.o: %.c
	$(RISCV_GCC) -c $(CFLAGS) $^

.PHONY: clean

clean:
	rm -rf $(ELF_NAME) $(ELF_NAME).log boot.bin main.bin *.o .*.swp .*.swo
