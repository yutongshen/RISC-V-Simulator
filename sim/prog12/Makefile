src_dir  := ./brom_prog ./sysram_prog ./kernel_prog

.PHONY: all

all: comp copy_elf

comp:
	@for dir in ${src_dir}; do \
	    make -C $${dir}; \
	done

copy_elf:
	cp ./brom_prog/*.bin .;
	dd if=${RISCV}/riscv64-unknown-elf/bin/bbl of=./bbl.align bs=65536 conv=sync;
	cat ./bbl.align ./riscv64emu.dtb > flash.bin;
	# dd if=./sysram_prog/sysram of=./sysram_prog/sysram.align bs=65536 conv=sync;
	# cat ./sysram_prog/sysram.align ./kernel_prog/kernel > flash.bin;

auto-format:
	@for dir in ${src_dir}; do \
	    make -C $${dir} auto-format; \
	done
	@if [ "${SRC_H}" != "" ] || [ "${SRC_C}" != "" ] || [ "${SRC_CPP}" != "" ]; then \
	    clang-format -i ${SRC_H} ${SRC_C} ${SRC_CPP}; \
    fi

clean:
	rm -rf *.dtb .*.swp .*.swo
	@for dir in ${src_dir}; do \
	    make -C $${dir} clean; \
	done
