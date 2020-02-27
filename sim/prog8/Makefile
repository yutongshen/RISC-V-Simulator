ELF_NAME := main

export CROSS_PREFIX ?= riscv64-unknown-elf-
export RISCV_GCC ?= $(CROSS_PREFIX)gcc
export RISCV_OBJDUMP ?= $(CROSS_PREFIX)objdump -xsd
export RISCV_OBJCOPY ?= $(CROSS_PREFIX)objcopy -O verilog

LDFILE := link.ld
CFLAGS := -march=rv32i -mabi=ilp32
LDFLAGS := -static -nostdlib -nostartfiles -march=rv32i -mabi=ilp32 -T$(LDFILE) -lgcc


SRC_C := $(wildcard *.c)
OBJ_C := $(patsubst %.c,%.o,$(SRC_C))
SRC_S := $(wildcard *.S)
OBJ_S := $(patsubst %.S,%.o,$(SRC_S))
SRC := $(SRC_C) $(SRC_S)
OBJ := $(OBJ_C) $(OBJ_S)

.SUFFIXES: .o .S .c

.PHONY: all

all: build_elf build_log build_hex

build_elf: $(OBJ) | $(LDFILE)
	$(RISCV_GCC) $^ $(LDFLAGS) -o $(ELF_NAME)

build_log: $(ELF_NAME)
	$(RISCV_OBJDUMP) $< > $(ELF_NAME).log

build_hex: $(ELF_NAME)
	$(RISCV_OBJCOPY) $< -i 4 -b 0 -j .text0 --change-addresses 0 rom0.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 1 -j .text0 --change-addresses 0 rom1.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 2 -j .text0 --change-addresses 0 rom2.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 3 -j .text0 --change-addresses 0 rom3.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 0 -R .text0 --change-addresses -0x20000000 dram0.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 1 -R .text0 --change-addresses -0x20000000 dram1.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 2 -R .text0 --change-addresses -0x20000000 dram2.hex
	$(RISCV_OBJCOPY) $< -i 4 -b 3 -R .text0 --change-addresses -0x20000000 dram3.hex

%.o: %.S
	$(RISCV_GCC) -c $(CFLAGS) $^

%.o: %.c
	$(RISCV_GCC) -c $(CFLAGS) $^

.PHONY: clean

clean:
	rm -rf $(ELF_NAME) $(ELF_NAME).log rom*.hex dram*.hex *.o
