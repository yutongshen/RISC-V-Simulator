src_dir  := ./brom_prog ./ap_prog

.PHONY: all

all: comp copy_elf

comp:
	@for dir in ${src_dir}; do \
	    make -C $${dir}; \
	done

copy_elf:
	cp ./brom_prog/*.bin .;
	cp ./ap_prog/main flash.bin;

auto-format:
	@for dir in ${src_dir}; do \
	    make -C $${dir} auto-format; \
	done
	@if [ "${SRC_H}" != "" ] || [ "${SRC_C}" != "" ] || [ "${SRC_CPP}" != "" ]; then \
	    clang-format -i ${SRC_H} ${SRC_C} ${SRC_CPP}; \
    fi

clean:
	rm -rf *.dtb *.bin .*.swp .*.swo
	@for dir in ${src_dir}; do \
	    make -C $${dir} clean; \
	done
