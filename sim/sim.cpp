#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
using namespace std;

#define TEST_START 0x20000
#define SIM_END 0x2fffc
#define SIM_END_CODE -1

uint64_t hex2int(const char *hex)
{
    uint64_t res(0);
    while (1) {
        switch (*(hex++) | 0x20) {
        case '0':
            res = res << 4 | 0x0;
            break;
        case '1':
            res = res << 4 | 0x1;
            break;
        case '2':
            res = res << 4 | 0x2;
            break;
        case '3':
            res = res << 4 | 0x3;
            break;
        case '4':
            res = res << 4 | 0x4;
            break;
        case '5':
            res = res << 4 | 0x5;
            break;
        case '6':
            res = res << 4 | 0x6;
            break;
        case '7':
            res = res << 4 | 0x7;
            break;
        case '8':
            res = res << 4 | 0x8;
            break;
        case '9':
            res = res << 4 | 0x9;
            break;
        case 'a':
            res = res << 4 | 0xa;
            break;
        case 'b':
            res = res << 4 | 0xb;
            break;
        case 'c':
            res = res << 4 | 0xc;
            break;
        case 'd':
            res = res << 4 | 0xd;
            break;
        case 'e':
            res = res << 4 | 0xe;
            break;
        case 'f':
            res = res << 4 | 0xf;
            break;
        default:
            return res;
        }
    }
};

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Please input path of the simulator and test program");
        return 0;
    }
    const char *dump_file("dump.out");
    char main_cmd[200];
    char dump_path[100];
    char gold_path[100];
    char buff[100];
    char bmp_res_path[100];
    uint32_t output, expect, sim_flag, err(0);
    int state(0), i(TEST_START >> 2);
    bool bmp_flag(0);
    bool done;
    sprintf(main_cmd,
            "%s/main %s/brom.bin %s/flash.bin -dump -mem_addr 0x%08x -mem_len "
            "0x1000 -sim_end "
            "0x%08x -sim_end_code 0x%08x -cycle 0xffffff -o %s/%s",
            argv[1], argv[2], argv[2], TEST_START, SIM_END, SIM_END_CODE,
            argv[1], dump_file);
    printf("%s", main_cmd);
    system(main_cmd);
    sprintf(dump_path, "%s/%s", argv[1], dump_file);
    sprintf(gold_path, "%s/golden.hex", argv[2]);
    sprintf(bmp_res_path, "%s/result.bmp", argv[1]);
    FILE *proc(fopen(dump_path, "r"));
    FILE *golden(fopen(gold_path, "r"));
    FILE *bmp_res;
    if (proc) {
        while (!feof(proc) && !feof(golden)) {
            if (fgets(buff, 100, proc)) {
                // printf("%s state = %d, done = %d\n", buff, state,
                // !strcmp(buff, "MEMDUMP\n"));
                if (!strcmp(buff, "SIMEND\n")) {
                    done = state == 0 ? 0 : done, state = 1;
                    continue;
                } else if (!strcmp(buff, "MEMDUMP\n")) {
                    done = state == 1 ? 0 : done, state = 2;
                    continue;
                }
                switch (state) {
                case 1:
                    if (!done) {
                        sim_flag = hex2int(buff);
                        if (sim_flag == SIM_END_CODE)
                            printf("\nDone\n\n");
                        else
                            printf("\nSIM_END(%08x) = %08x, expect = %08x\n\n",
                                   SIM_END, sim_flag, SIM_END_CODE);
                        done = 1;
                    }
                    break;
                case 2:
                    output = hex2int(buff);
                    if (fgets(buff, 100, golden))
                        expect = hex2int(buff);
                    else
                        break;
                    if ((short) output == (short) 0x4d42 && !bmp_flag)
                        bmp_flag = 1, bmp_res = fopen(bmp_res_path, "wb");
                    if (bmp_flag)
                        fwrite((char *) &output, 1, sizeof(output), bmp_res);
                    if (output != expect)
                        printf("MEM[0x%08x] = 0x%08x, expect = %08x\n", i,
                               output, expect),
                            ++err;
                    else
                        printf("MEM[0x%08x] = 0x%08x, pass\n", i, output);
                    ++i;
                    break;
                }
            }
        }
        if (bmp_flag)
            fclose(bmp_res);
        pclose(proc);
        fclose(golden);
    }
    if (!err) {
        printf("\n");
        printf("\n");
        printf("        ****************************               \n");
        printf("        **                        **       |\\__|| \n");
        printf("        **  Congratulations !!    **      / O.O  | \n");
        printf("        **                        **    /_____   | \n");
        printf("        **  Simulation PASS!!     **   /^ ^ ^ \\  |\n");
        printf("        **                        **  |^ ^ ^ ^ |w| \n");
        printf("        ****************************   \\m___m__|_|\n");
        printf("\n");
    } else {
        printf("\n");
        printf("\n");
        printf("        ****************************               \n");
        printf("        **                        **       |\\__|| \n");
        printf("        **  OOPS!!                **      / X,X  | \n");
        printf("        **                        **    /_____   | \n");
        printf("        **  Simulation Failed!!   **   /^ ^ ^ \\  |\n");
        printf("        **                        **  |^ ^ ^ ^ |w| \n");
        printf("        ****************************   \\m___m__|_|\n");
        printf("         Totally has %d errors                     \n", err);
        printf("\n");
    }
    return 0;
}
