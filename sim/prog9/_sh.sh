#!/bin/bash

files=(tmp/*)

for test in "${files[@]}"
do
    echo riscv64-unknown-elf-objcopy -O binary ${test} --change-addresses 0x80000000 ${test}.bin
    eval riscv64-unknown-elf-objcopy -O binary ${test} --change-addresses 0x80000000 ${test}.bin
done

