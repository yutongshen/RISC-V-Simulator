root_dir := $(PWD)
bld_dir  := ./build
src_dir  := ./main ./sys ./dev ./cpu ./bus ./mem ./disk ./fesvr ./util
sim_dir  := ./sim
sim_file := sim
mmap_dir := ./mmap
rv_ext   := rv64mi rv64si rv64ui rv64um rv64ua rv64uc rv64uf rv64ud #benchmarks
obj_path  = $(src_dir:=/*.o)
test     := ${bld_dir}/main ./sim/prog9/boot.bin ./sim/prog9/rv64ui/rv64ui-p-add.bin -dump -mem_addr 0x80001000 -mem_len 0x10 -sim_end 0x80001000 -sim_end_code 0x1 -cycle 0x100 -o ./build/dump.out

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
	@${INIT_MMAP} ${mmap_dir}/mmap.h ${bld_dir}/mmap_soc.h
	@for file in ${mmap_dir}/*_reg.h; do \
	  ${XTEND_MMAP} ${bld_dir}/mmap_soc.h $${file}; \
	done
	@cp ${bld_dir}/mmap_soc.h ${sim_dir}/prog${prog};

	@if [ "${prog}" == "9" ]; then \
	  for ext in ${rv_ext}; do \
	    sh ${sim_dir}/prog${prog}/script.sh $${ext}; \
	  done \
	else \
	  make -C ${sim_dir} prog=${prog}; \
	  ${sim_dir}/${sim_file} ${bld_dir} ${sim_dir}/prog${prog}; \
	fi

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
	@make -C ${sim_dir} auto-format;

clean:
	rm -rf ./build .*.swo .*.swp .*.bin
	@for dir in ${src_dir}; do \
	    make -C $${dir} clean; \
	done
	@make -C ${sim_dir} clean;
