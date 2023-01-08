#ifndef __DATATYPE__
#define __DATATYPE__

#include <cstdint>

typedef uint8_t DataType;

#define DATA_TYPE_BYTE 0b00010
#define DATA_TYPE_BYTE_UNSIGNED 0b00011
#define DATA_TYPE_HWORD 0b00100
#define DATA_TYPE_HWORD_UNSIGNED 0b00101
#define DATA_TYPE_WORD 0b01000
#define DATA_TYPE_WORD_UNSIGNED 0b01001
#define DATA_TYPE_DWORD 0b10000
#define GET_SIGNED(x) (!(x & 0x1))
#define GET_SIZE(x) (x >> 1)
#define HALF_DATA(TYPE, SIGNED) (((TYPE) >> 1) | !(SIGNED))

#define ACCESS_TYPE_FETCH 0
#define ACCESS_TYPE_LOAD 1
#define ACCESS_TYPE_STORE 2

#endif
