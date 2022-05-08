compile_cfg := compile.cfg
bin_file    := brom ddr flash
funct_list  := funct.list

.PHONY: all

all: comp copy_elf

comp:
	@for dir in $$(cat ${compile_cfg}); do \
	    make -C $${dir}; \
	done

copy_elf:
	@rm -rf *.bin *.list;
	@for bin in ${bin_file}; do \
		echo -n -e \\x00 > $${bin}.bin; \
	    IFS=''; \
	    while read -r items; do \
			IFS=' '; read -ra items <<< $${items}; \
			[[ $$(($${items[1]})) -eq 0 ]] \
			    && touch $${bin}.align \
	            || dd if=$${bin}.bin of=$${bin}.align bs=$$(($${items[1]})) conv=sync count=1 2> /dev/null; \
	        cat $${bin}.align $${items[0]} > $${bin}.bin || exit 1; \
            if [ -f $${items[0]%.*}.list ]; then \
                cat $${items[0]%.*}.list >> ${funct_list}; \
            fi \
	    done < $${bin}.cfg; \
	done

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
	    if [ -f "$${dir}/Makefile" ]; then \
	        make -C $${dir} clean; \
	    fi \
	done
