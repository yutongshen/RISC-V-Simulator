SHELL         := /bin/bash
root_dir      := $(PWD)
bld_dir       := ./build
src_dir       := ./main ./sys ./dev ./cpu ./bus ./mem ./fesvr ./util
dts_dir       := ./dts
dts_file      := riscv64emu.dts
sim_dir       := ./sim
sim_file      := sim
mmap_dir      := ./mmap
rv_ext        := rv64mi rv64si rv64ui rv64um rv64ua rv64uc rv64uf rv64ud #benchmarks
obj_path       = $(src_dir:=/*.o)
test          := ${bld_dir}/main ./sim/prog9/boot.bin ./sim/prog9/rv64ui/rv64ui-p-add.bin -dump -mem_addr 0x80001000 -mem_len 0x10 -sim_end 0x80001000 -sim_end_code 0x1 -cycle 0x100 -o ./build/dump.out
tmdl_msg_log  := tmdl_msg.log
compile_cfg   := compile.cfg
prog_src_list := common prog${prog}
inc_src_list  := common prog${prog} prog${prog}/$${prog_type}
err_ignore    := 2> /dev/null || :

INIT_MMAP     := ./script/build_mmap -i 
XTEND_MMAP    := ./script/build_mmap -x 

TMDL_PARSE_C  := ./script/tmdl_parse -c
TMDL_PARSE_S  := ./script/tmdl_parse -s

CC      := g++
CFLAGS  := -std=c++11 -pthread

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
	@rm -rf ${bld_dir}/${tmdl_msg_log};
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
	@cp -r ${sim_dir}/prog${prog}/sd_image ${bld_dir}/prog/ ${err_ignore};
	
	@for prog_type in $$(cat ${sim_dir}/prog${prog}/${compile_cfg}); do \
	    mkdir ${bld_dir}/prog/$${prog_type}; \
	    mkdir ${bld_dir}/prog/$${prog_type}/include; \
	    for src_dir in ${prog_src_list}; do \
	        for file in ${sim_dir}/$${src_dir}/$${prog_type}/*; do \
	            if [[ ! "$${file}" == "${sim_dir}/$${src_dir}/$${prog_type}/*" ]]; then \
	                _file=$$(basename $${file}); \
	                if [[ "$${_file#*.}" == "c" ]]; then \
	                    ${TMDL_PARSE_C} $${file} ${bld_dir}/prog/$${prog_type}/$${_file} ${bld_dir}/${tmdl_msg_log}; \
	                elif [[ "$${_file#*.}" == "S" ]] || [[ "$${_file#*.}" == "s" ]]; then \
	                    ${TMDL_PARSE_S} $${file} ${bld_dir}/prog/$${prog_type}/$${_file} ${bld_dir}/${tmdl_msg_log}; \
	                else \
	                    cp $${file} ${bld_dir}/prog/$${prog_type}/ ${err_ignore}; \
	                fi; \
	            fi; \
	        done; \
	    done; \
	    for src_dir in ${inc_src_list}; do \
	        for file in ${sim_dir}/$${src_dir}/include/*; do \
	            if [[ ! "$${file}" == "${sim_dir}/$${src_dir}/include/*" ]]; then \
	                _file=$$(basename $${file}); \
	                if [[ "$${_file#*.}" == "c" ]]; then \
	                    ${TMDL_PARSE_C} $${file} ${bld_dir}/prog/$${prog_type}/include/$${_file} ${bld_dir}/${tmdl_msg_log}; \
	                elif [[ "$${_file#*.}" == "S" ]] || [[ "$${_file#*.}" == "s" ]]; then \
	                    ${TMDL_PARSE_S} $${file} ${bld_dir}/prog/$${prog_type}/include/$${_file} ${bld_dir}/${tmdl_msg_log}; \
	                else \
	                    cp $${file} ${bld_dir}/prog/$${prog_type}/include/ ${err_ignore}; \
	                fi; \
	            fi; \
	        done; \
	    done; \
	    cp ${bld_dir}/mmap_soc.h ${bld_dir}/prog/$${prog_type}/include; \
	done;
	
	# Compile
	@make -C ${sim_dir} debug=${debug};
	@make -C ${bld_dir}/prog || exit 1;
	
	# Simulation start
	${sim_dir}/${sim_file} ${bld_dir} ${bld_dir}/prog ${bld_dir}/${tmdl_msg_log};
	
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
