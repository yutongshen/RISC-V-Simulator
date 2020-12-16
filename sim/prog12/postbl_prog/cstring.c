#include <stddef.h>
#include <stdint.h>
#include <string.h>

size_t strlen(const char *str)
{
    int n = 0;
    while (*str++ && ++n) ;
    return n;
}

int strcmp(const char *a, const char *b)
{
    char c1, c2;
    while ((c1 = *a++) == (c2 = *b++) && c1);
    return c1 != c2;

}

void *memset(void *ptr, int val, size_t len)
{
    uint64_t *dword_ptr = ptr;
    uint64_t write_dword = (char) val;
    write_dword |= write_dword << 8;
    write_dword |= write_dword << 16;
    write_dword |= write_dword << 32;

    while (len >= 8) {
        *dword_ptr++ = write_dword;
        len -= 8;
    }

    char *byte_ptr = (char *) dword_ptr;
    while (len--) {
        *byte_ptr++ = val;
    }

    return ptr;
}

void *memcpy(void *ptr, const void *src, size_t len)
{
    uint64_t *dword_ptr = ptr;
    const uint64_t *dword_src = src;

    while (len >= 8) {
        *dword_ptr++ = *dword_src++;
        len -= 8;
    }

    char *byte_ptr = (char *) dword_ptr;
    char *byte_src = (char *) dword_src;
    while (len--) {
        *byte_ptr++ = *byte_src++;
    }

    return ptr;
}
