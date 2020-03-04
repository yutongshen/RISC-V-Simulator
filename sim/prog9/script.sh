#!/bin/bash

prog_dir="./sim/prog9/"
boot_file="boot.bin"
dump_file="./build/dump.out"

if [ "${1}" == "" ]; then
  echo "Please input testbench name"
  exit 1
fi

files=(${prog_dir}${1}/*)

for test in "${files[@]}"
do
  cmd="./build/main ${prog_dir}${boot_file} ${test} -dump -mem_addr 0x80001000 -mem_len 0x10 -sim_end 0x80001000 -sim_end_code 0x1 -cycle 0xffff -o ${dump_file}"
  eval "${cmd} >> ./build/.tmp"
  sim_end=$(sed "2q;d" ./build/dump.out)
  if [ "${sim_end}" != "00000001" ]; then
    # eval "${cmd}"
    echo "There are some error on ${test}"
    echo "Please try command: ${cmd}"
    exit 1
  else
    echo "${test} ..... ok"
  fi
done

