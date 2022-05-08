#include "mem/ram.h"
#include <fstream>
#include "util/util.h"

void RAM::_init_data(const char *init_file)
{
    std::ifstream data_stream(init_file, std::ios::binary | std::ios::in);
    if (!data_stream)
        abort();
    data_stream.read((char *) data, size);
}

RAM::RAM(uint64_t size) : Slave(size), data(new uint64_t[size >> 3]) {}

RAM::RAM(const char *str_size)
    : Slave(str_size), data(new uint64_t[this->size >> 3])
{
}

RAM::RAM(const char *init_file, const char *str_size)
    : Slave(str_size), data(new uint64_t[this->size >> 3])
{
    _init_data(init_file);
}

RAM::~RAM()
{
    delete[] data;
}

bool RAM::write(const Addr &addr,
                const DataType &data_type,
                const uint64_t &wdata)
{
    switch (data_type) {
    case DATA_TYPE_DWORD:
        if (!_check_bound(addr, 8))
            return 0;
        *((uint64_t *) (((uint8_t *) data) + addr)) = wdata;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        if (!_check_bound(addr, 4))
            return 0;
        *((uint32_t *) (((uint8_t *) data) + addr)) = (uint32_t) wdata;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        if (!_check_bound(addr, 2))
            return 0;
        *((uint16_t *) (((uint8_t *) data) + addr)) = (uint16_t) wdata;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        if (!_check_bound(addr, 1))
            return 0;
        ((uint8_t *) data)[addr] = (uint8_t) wdata;
        break;
    default:
        abort();
    }
    return 1;
}
bool RAM::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    switch (data_type) {
    case DATA_TYPE_DWORD:
        if (!_check_bound(addr, 8))
            return 0;
        rdata = *((uint64_t *) (((uint8_t *) data) + addr));
        return 1;
    case DATA_TYPE_WORD:
        if (!_check_bound(addr, 4))
            return 0;
        rdata = sext((*((uint32_t *) (((uint8_t *) data) + addr))), 32);
        return 1;
    case DATA_TYPE_WORD_UNSIGNED:
        if (!_check_bound(addr, 4))
            return 0;
        rdata = (uint64_t)(*((uint32_t *) (((uint8_t *) data) + addr)));
        return 1;
    case DATA_TYPE_HWORD:
        if (!_check_bound(addr, 2))
            return 0;
        rdata = sext((*((uint16_t *) (((uint8_t *) data) + addr))), 16);
        return 1;
    case DATA_TYPE_HWORD_UNSIGNED:
        if (!_check_bound(addr, 2))
            return 0;
        rdata = (uint64_t)(*((uint16_t *) (((uint8_t *) data) + addr)));
        return 1;
    case DATA_TYPE_BYTE:
        if (!_check_bound(addr, 1))
            return 0;
        rdata = sext(((uint8_t *) data)[addr], 8);
        return 1;
    case DATA_TYPE_BYTE_UNSIGNED:
        if (!_check_bound(addr, 1))
            return 0;
        rdata = (uint64_t)(((uint8_t *) data)[addr]);
        return 1;
    default:
        abort();
    }
}
