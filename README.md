# RISC-V Simulator

## Environment
- Ubuntu 18.04.1 LTS (GNU/Linux 4.15.0-34-generic x86\_64)

## Prerequisite
- g++ 7.4.0

## Processor extension implementation
- RV64IMACF

## Compile
```shell
$ make
```

## Block diagram
![](./figures/rv64.png)

- Introduce all slave linked on bus  

|Slave   |Base Address|Size    |Read&Write|Cacheable|
|:------:|:----------:|:------:|:--------:|:-------:|
|Boot ROM|0x00000000  |4KBytes |Read-Only |Y        |
|SRAM0   |0x00010000  |64KBytes|Read-Write|Y        |
|SRAM1   |0x00010000  |64KBytes|Read-Write|Y        |
|DRAM0   |0x20000000  |2MBytes |Read-Write|Y        |
|DRAM1   |0x80000000  |2MBytes |Read-Write|Y        |

- Also you can modify those configuration throgh rewrite `main/main.cpp`
## Usage
- We provide `prog0`~`prog9` test program that you can check processor is correct or not
- Especially `prog9` is referenced to https://github.com/riscv/riscv-tests
  - There are include all extension version testbench
  - In addition, those testbench can be exacuted with virtual address

- You can use makefile to exacute simulator. e.g.

```=sh
make sim prog=0
```


## Authors
[Yu-Tong Shen](https://github.com/yutongshen/)

###### tags: `RISC-V`
