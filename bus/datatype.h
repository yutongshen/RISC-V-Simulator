#ifndef __DATATYPE__
#define __DATATYPE__

typedef uint8_t DataType;

#define DATA_TYPE_BYTE 0b00010
#define DATA_TYPE_BYTE_UNSIGNED 0b00011
#define DATA_TYPE_HWORD 0b00100
#define DATA_TYPE_HWORD_UNSIGNED 0b00101
#define DATA_TYPE_WORD 0b01000
#define DATA_TYPE_WORD_UNSIGNED 0b01001
#define DATA_TYPE_DWORD 0b10000

#define is_signed(type) (!((type) &1))
#define data_size(type)                    \
    (((type) &DATA_TYPE_DWORD)             \
         ? 8                               \
         : ((type) &DATA_TYPE_WORD)        \
               ? 4                         \
               : ((type) &DATA_TYPE_HWORD) \
                     ? 2                   \
                     : ((type) &DATA_TYPE_BYTE) ? 1 : (abort(), -1))
#define half_data(type, signed) (((type) >> 1) + !(signed) )

#define ACCESS_TYPE_FETCH 0
#define ACCESS_TYPE_LOAD 1
#define ACCESS_TYPE_STORE 2

#endif
