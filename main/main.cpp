#include <string.h>
#include <iostream>
#include "bus/bus.h"
#include "cpu/cpu.h"
#include "disk/disk.h"
#include "mem/ram.h"
#include "mem/rom.h"
#include "sys/system.h"
#include "util/util.h"
using namespace std;

int main(int argc, char **argv)
{
    // Arguments
    ArgumentParser argparser;
    argparser.add_argument("-dump", "", "Dump the information about memory.");
    argparser.add_argument("-mem_addr", "MEMADDR",
                           "If you dump the information about memory, it will "
                           "be the start address",
                           "0x10000", 0);
    argparser.add_argument(
        "-mem_len", "MEMLEN",
        "If you dump the information about memory, it will be the size",
        "0x100", 0);
    argparser.add_argument("-sim_end", "SIMEND",
                           "The address is used for check terminate", "0x1fffc",
                           0);
    argparser.add_argument("-sim_end_code", "SIMENDCODE",
                           "The value is used for check terminate", "-1", 0);
    argparser.add_argument("-cycle", "CYCLE", "The max simulation cycles",
                           "0x10000", 0);
    argparser.add_argument("-pc", "PC", "Initialize program count (default: 0)",
                           "0x0", 0);
    argparser.add_argument("-o", "OUTPUT", "The output file", "a.out", 0);
    if (!argparser.parse(argc, argv))
        return 0;

    // Main
    const uint32_t SIM_END(argparser.get_int("SIMEND"));
    const uint32_t SIM_END_CODE(argparser.get_int("SIMENDCODE"));
    CPU cpu_0(argparser.get_int("PC"));
    ROM boot_rom(argparser.get_str(0).c_str(), 0x1000);
    RAM sram_0("64kb");
    RAM sram_1("64kb");
    Disk disk_0(argparser.get_str(1).c_str(), "2mb");
    Disk disk_1(argparser.get_str(1).c_str(), "2mb");

    Bus bus_0;
    cpu_0.bus_connect(&bus_0);
    bus_0.s_connect(&boot_rom);
    bus_0.s_connect(0x00010000, &sram_0);
    bus_0.s_connect(0x00020000, &sram_1);
    bus_0.s_connect(0x20000000, &disk_0);
    bus_0.s_connect(0x80000000, &disk_1);

    System sys_0;
    sys_0.add(&cpu_0);

    // Run
    uint64_t cycle(argparser.get_int("CYCLE"));
    uint64_t end_code;
    while ((bus_0.read(SIM_END, DATA_TYPE_WORD, end_code),
            (uint32_t) end_code) != SIM_END_CODE &&
           cycle--) {
        sys_0.run();
    }

    // Dump
    if (argparser.get_bool("DUMP")) {
        const uint64_t dump_start(argparser.get_int("MEMADDR"));
        uint64_t idx(dump_start);
        uint64_t dump_len(argparser.get_int("MEMLEN") << 2);
        FILE *output(fopen(argparser.get_str("OUTPUT").c_str(), "w"));
        fprintf(output, "SIMEND\n");
        fprintf(output, "%08x\n",
                (bus_0.read(SIM_END, DATA_TYPE_WORD, end_code),
                 (uint32_t) end_code));
        fprintf(output, "MEMDUMP\n");
        for (; idx < dump_start + dump_len; idx += 4) {
            bus_0.read(idx, DATA_TYPE_WORD, end_code);
            fprintf(output, "%08x\n", (uint32_t) end_code);
        }
    }
    return 0;
}
