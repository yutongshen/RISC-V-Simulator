void boot(void) 
{
    extern int __text_start[];
    extern int __text_end[];
    extern int __rodata_start[];
    extern int __rodata_end[];
    extern int _dram_i_start[];
    extern int __sdata_paddr_start[];
    extern int __sdata_start[];
    extern int __sdata_end[];
    extern int __data_paddr_start[];
    extern int __data_start[];
    extern int __data_end[];
    int *ptr;
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
