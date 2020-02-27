void boot(void) 
{
    extern unsigned char __text_start[];
    extern unsigned char __text_end[];
    extern unsigned char __rodata_start[];
    extern unsigned char __rodata_end[];
    extern unsigned char _dram_i_start[];
    extern unsigned char __sdata_paddr_start[];
    extern unsigned char __sdata_start[];
    extern unsigned char __sdata_end[];
    extern unsigned char __data_paddr_start[];
    extern unsigned char __data_start[];
    extern unsigned char __data_end[];
    unsigned char *ptr;
    int i;

    // Move instruction to IM
    i = 0;
    ptr = __text_start;
    while (ptr < __text_end)
        *(ptr++) = _dram_i_start[i++];

    ptr = __rodata_start;
    while (ptr < __rodata_end)
        *(ptr++) = _dram_i_start[i++];

    // Move sdata to DM
    i = 0;
    ptr = __sdata_start;
    while (ptr < __sdata_end)
        *(ptr++) = __sdata_paddr_start[i++];

    // Move data to DM
    i = 0;
    ptr = __data_start;
    while (ptr < __data_end)
        *(ptr++) = __data_paddr_start[i++];
}
