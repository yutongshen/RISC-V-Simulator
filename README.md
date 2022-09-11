# RISC-V Simulator

## Environment
- Ubuntu 18.04.1 LTS (GNU/Linux 4.15.0-34-generic x86\_64)

## Prerequisite
- cpp compiler: g++ 7.4.0
- device-tree-compiler: dtc 1.4.5
- [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)

## Processor extension implementation
- RV64IMACFD

## Compile
```shell
$ make
```

## Block diagram
![](./figures/rv64.png)

- Introduce all slave on bus  

|Slave   |Base Address    |Size    |Read&Write|Cacheable|
|:------:|:--------------:|:------:|:--------:|:-------:|
|Boot ROM|0x0\_0000\_0000 |  32 KB | RO       | Y       |
|SRAM0   |0x0\_0001\_0000 |  64 KB | RW       | Y       |
|SRAM1   |0x0\_0002\_0000 |  64 KB | RW       | Y       |
|Cluster |0x0\_0004\_0000 |   4 KB | RW       | N       |
|Bridge  |0x0\_1000\_0000 | 256 MB | RW       | N       |
|UART    |0x0\_1000\_1000 |   4 KB | RW       | N       |
|HTIF    |0x0\_1000\_2000 |   4 KB | RW       | N       |
|TMDL    |0x0\_1000\_3000 |   4 KB | RW       | N       |
|\*CLINT |0x0\_1001\_0000 |  64 KB | RW       | N       |
|\*PLIC  |0x0\_1400\_0000 |  64 MB | RW       | N       |
|DDR0    |0x0\_2000\_0000 |   8 MB | RW       | Y       |
|DDR1    |0x0\_8000\_0000 |   8 MB | RW       | Y       |
|FLASH   |0x1\_0000\_0000 |   1 GB | RW       | Y       |

- \*CLINT: Core-local Interrupt Controller
- \*PLIC: Platform Level Interrupt Controller

- You also can modify `main/main.cpp` to change those configuration
## Usage
- We provide `prog0`~`prog13` program that you can run processor
- You can use the following command to simulate

```=sh
make sim prog=13 # linux simulation
```

- prog13 is contain [bbl](https://github.com/riscv-software-src/riscv-pk), [Linux](https://github.com/riscvarchive/riscv-linux) and [busybox](https://busybox.net/)

- Linux login
  - user name: root
  - password: root
  <br>![](./figures/linux_login.gif)

- CPU and Linux information
  <br>![](./figures/cpuinfo.png)


## Authors
[Yu-Tong Shen](https://github.com/yutongshen/)

###### tags: `RISC-V`
