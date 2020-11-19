root_dir := $(PWD)
bld_dir  := ./build
src_dir  := ./main ./sys ./dev ./cpu ./bus ./mem ./fesvr ./util
dts_dir  := ./dts
dts_file := riscv64emu.dts
sim_dir  := ./sim
sim_file := sim
mmap_dir := ./mmap
rv_ext   := rv64mi rv64si rv64ui rv64um rv64ua rv64uc rv64uf rv64ud #benchmarks
obj_path  = $(src_dir:=/*.o)
test     := ${bld_dir}/main ./sim/prog9/boot.bin ./sim/prog9/rv64ui/rv64ui-p-add.bin -dump -mem_addr 0x80001000 -mem_len 0x10 -sim_end 0x80001000 -sim_end_code 0x1 -cycle 0x100 -o ./build/dump.out
err_ignore := 2> /dev/null || :

INIT_MMAP  := ./script/build_mmap -i 
XTEND_MMAP := ./script/build_mmap -x 

CC      := g++
CFLAGS  := 

ifeq (true,${debug})
CFLAGS  += -g 
endif
ifeq (1,${debug})
CFLAGS  += -g 
endif

OBJ      = $(wildcard ${obj_path})

.PHONY: all

all: build_obj build_elf | ${bld_dir}

build_elf: | ${bld_dir}
	${CC} ${OBJ} ${CFLAGS} -o ${bld_dir}/main

build_obj:
	@for dir in ${src_dir}; do \
	    make -C $${dir} root_dir=${root_dir} debug=${debug}; \
	done

${bld_dir}:
	mkdir -p ${bld_dir}

sim: all
	@if [ -d ${bld_dir}/prog ]; then \
	  rm -rf ${bld_dir}/prog; \
	fi
	@mkdir ${bld_dir}/prog;
	
	@${INIT_MMAP} ${mmap_dir}/mmap.h ${bld_dir}/mmap_soc.h
	@for file in ${mmap_dir}/*_reg.h; do \
	  ${XTEND_MMAP} ${bld_dir}/mmap_soc.h $${file}; \
	done
	@dtc -I dts -O dtb ${dts_dir}/${dts_file} > ${bld_dir}/prog/riscv64emu.dtb;
	
	# Move prog
	@cp ${sim_dir}/common/* ${bld_dir}/prog ${err_ignore};
	@cp ${sim_dir}/prog${prog}/* ${bld_dir}/prog ${err_ignore};
	
	# Move brom_prog
	@mkdir ${bld_dir}/prog/brom_prog;
	@if [ -d ${sim_dir}/common/brom_prog ]; then \
	  cp ${sim_dir}/common/brom_prog/* ${bld_dir}/prog/brom_prog/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/prog${prog}/brom_prog ]; then \
	  cp ${sim_dir}/prog${prog}/brom_prog/* ${bld_dir}/prog/brom_prog/ ${err_ignore}; \
	fi
	@mkdir ${bld_dir}/prog/brom_prog/include;
	@if [ -d ${sim_dir}/common/include ]; then \
	  cp ${sim_dir}/common/include/* ${bld_dir}/prog/brom_prog/include/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/common/brom_prog/include ]; then \
	  cp ${sim_dir}/common/brom_prog/include/* ${bld_dir}/prog/brom_prog/include/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/prog${prog}/include ]; then \
	  cp ${sim_dir}/prog${prog}/include/* ${bld_dir}/prog/brom_prog/include/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/prog${prog}/brom_prog/include ]; then \
	  cp ${sim_dir}/prog${prog}/brom_prog/include/* ${bld_dir}/prog/brom_prog/include/ ${err_ignore}; \
	fi
	@cp ${bld_dir}/mmap_soc.h ${bld_dir}/prog/brom_prog/include;
	
	# Move ap_prog
	@mkdir ${bld_dir}/prog/ap_prog;
	@if [ -d ${sim_dir}/common/ap_prog ]; then \
	  cp ${sim_dir}/common/ap_prog/* ${bld_dir}/prog/ap_prog/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/prog${prog}/ap_prog ]; then \
	  cp ${sim_dir}/prog${prog}/ap_prog/* ${bld_dir}/prog/ap_prog/ ${err_ignore}; \
	fi
	@mkdir ${bld_dir}/prog/ap_prog/include;
	@if [ -d ${sim_dir}/common/include ]; then \
	  cp ${sim_dir}/common/include/* ${bld_dir}/prog/ap_prog/include/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/common/ap_prog/include ]; then \
	  cp ${sim_dir}/common/ap_prog/include/* ${bld_dir}/prog/ap_prog/include/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/prog${prog}/include ]; then \
	  cp ${sim_dir}/prog${prog}/include/* ${bld_dir}/prog/ap_prog/include/ ${err_ignore}; \
	fi
	@if [ -d ${sim_dir}/prog${prog}/ap_prog/include ]; then \
	  cp ${sim_dir}/prog${prog}/ap_prog/include/* ${bld_dir}/prog/ap_prog/include/ ${err_ignore}; \
	fi
	@cp ${bld_dir}/mmap_soc.h ${bld_dir}/prog/ap_prog/include;
	
	# Compile
	@make -C ${sim_dir};
	@make -C ${bld_dir}/prog;
	
	# Simulation start
	${sim_dir}/${sim_file} ${bld_dir} ${bld_dir}/prog;
	
check-leak: all
	valgrind -q --leak-check=full ${test}

link-make:
	@for dir in ${src_dir}; do \
		ln -sf ../.Makefile $${dir}/Makefile; \
	done

link-clang-format:
	@for dir in ${src_dir}; do \
	    ln -sf ../.clang-format $${dir}/.clang-format; \
	done

auto-format:
	@for dir in ${src_dir}; do \
	    make -C $${dir} auto-format; \
	done
	@make -C ${mmap_dir} auto-format;
	@make -C ${sim_dir} auto-format;

clean:
	rm -rf ./build .*.swo .*.swp .*.bin
	@for dir in ${src_dir}; do \
	    make -C $${dir} clean; \
	done
	@make -C ${mmap_dir} clean;
	@make -C ${sim_dir} clean;
