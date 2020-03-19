#!/bin/bash

prog_dir="./sim/prog9/"
boot_file="boot.bin"
dump_file="./build/dump.out"
sim_end_addr="0x80001000"

if [ "${1}" == "" ]; then
  echo "Please input testbench name"
  exit 1
fi

files=(${prog_dir}${1}/*)

for test in "${files[@]}"
do
  if [ "${test}" == "${prog_dir}${1}/rv64uc-p-rvc.bin" ]; then
    sim_end_addr="0x80003000"
    sim_end_code="0x1"
  elif [ "${test}" == "${prog_dir}${1}/rv64ud-p-move.bin" ]; then
    sim_end_addr="0x80002000"
    sim_end_code="0x1"
  else
    sim_end_addr="0x80001040"
    sim_end_code="0x1"
  fi
  cmd="./build/main ${prog_dir}${boot_file} ${test} -dump -mem_addr ${sim_end_addr} -mem_len 0x10 -sim_end ${sim_end_addr} -sim_end_code ${sim_end_code} -cycle 0xffff -o ${dump_file}"
  eval "${cmd} >> ./build/.tmp"
  sim_end=$(sed "2q;d" ./build/dump.out)
  if [ "${sim_end}" != "00000001" ]; then
    # eval "${cmd}"
    echo "${test} ..... error"
    echo "Please try command: ${cmd}"
    exit 1
  else
    echo "${test} ..... ok"
  fi
done

