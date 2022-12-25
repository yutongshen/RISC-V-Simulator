#include <string.h>
#include "elf_loader.h"

void *memcpy(void *restrict dst, const void *restrict src, uint64_t len)
{
    uint64_t *dword_dst = (uint64_t *)dst;
    const uint64_t *dword_src = (const uint64_t *)src;

    while (len >= 8) {
        *dword_dst++ = *dword_src++;
        len -= 8;
    }

    char *byte_dst = (char *)dword_dst;
    const char *byte_src = (const char *)dword_src;
    while (len--) {
        *byte_dst++ = *byte_src++;
    }

    return (void *) dst;
}

void *elf_loader(ehdr64_t *elf)
{
    shdr64_t *sec_header =
        (shdr64_t *) ((uint64_t) elf + (uint64_t) elf->e_shoff);
    uint16_t sec_header_num = elf->e_shnum;
    uint32_t i;

    for (i = 0; i < sec_header_num; ++i) {
        if (sec_header[i].sh_type == SHT_PROGBITS && sec_header[i].sh_addr)
            memcpy((void *)sec_header[i].sh_addr,
                   (void *)((size_t)elf + sec_header[i].sh_offset),
                   sec_header[i].sh_size);
    }

    return (void *) elf->e_entry;
}
