#include "mmap_soc.h"

volatile unsigned int sensor_addr[0x100] = {0};

void sensor_handle() {
    extern int _sensor_data_start[];
    int sctr_cnt = 0;
    int data_cnt = 0;
    
    volatile unsigned int *sensor_en  = &(sensor_addr[0x40]);
    volatile unsigned int *sensor_clr = &(sensor_addr[0x80]);
    volatile unsigned int *msip       = (volatile unsigned int *) CLINT_RG_MSIP;

    // Enable Global Interrupt
    // asm("csrsi mstatus, 0x8");  // MIE of mstatus

    // Enable Local Interrupt
    asm("li t6, 0x8");
    asm("csrs mie, t6");  // MSIE of mie

    while (1) {
        if (*sensor_en) {
            if (*sensor_clr) {
                msip[0] = 0;
                sctr_cnt = 0;
            }
            else if (sctr_cnt < 64) {
                sensor_addr[sctr_cnt++] = _sensor_data_start[data_cnt++];
            }
            else {
                msip[0] = 1;
                asm("wfi");
                msip[0] = 0;
                sctr_cnt = 0;
            }
        }
    }
}
