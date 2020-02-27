CC       := g++
OBJCOPY  := objcopy
ELF_NAME := sim

CFLAGS   := 

PROG_DIR := $(wildcard */)

SRC_H    := $(wildcard *.h)
SRC_C    := $(wildcard *.c)
OBJ_C    := $(patsubst %.c,%.o,$(SRC_C))
SRC_CPP  := $(wildcard *.cpp)
OBJ_CPP  := $(patsubst %.cpp,%.o,$(SRC_CPP))
SRC_S    := $(wildcard *.s)
OBJ_S    := $(patsubst %.s,%.o,$(SRC_S))
SRC_DAT  := $(wildcard *.dat)
OBJ_DAT  := $(patsubst %.dat,%.o,$(SRC_DAT))
OBJ      := ${OBJ_C} ${OBJ_CPP} ${OBJ_S} ${OBJ_DAT}

.SUFFIXES: .o .S .c .cpp

.PHONY: all clean

all: build_elf

build_elf: ${OBJ}
	${CC} ${OBJ} -o ${ELF_NAME}
	# make march=rv32i mabi=ilp32 -C prog${prog}
	make march=rv64i mabi=lp64 -C prog${prog}

%.o: %.c
	${CC} -c ${CFLAGS} $^

%.o: %.cpp
	${CC} -c ${CFLAGS} $^

%.o: %.S
	${CC} -c ${CFLAGS} $^

%.o: %.dat
	${OBJCOPY} -I binary -O elf64-x86-64 -B i386 $^ $@

auto-format:
	@if [ "${SRC_H}" != "" ] || [ "${SRC_C}" != "" ] || [ "${SRC_CPP}" != "" ]; then \
	    clang-format -i ${SRC_H} ${SRC_C} ${SRC_CPP}; \
    fi

clean:
	@for dir in ${PROG_DIR}; do \
	    make -C $${dir} clean; \
	done
	rm -rf *.o .*.swp .*.swo ${ELF_NAME}
