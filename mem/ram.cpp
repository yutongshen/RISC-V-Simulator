#include "mem/ram.h"
#include <fstream>
#include "util/util.h"

void RAM::_init_data(const char *init_file)
{
    std::ifstream data_stream(init_file, std::ios::binary | std::ios::in);
    if (!data_stream)
        return;
    data_stream.read((char *) data, size);
}

RAM::RAM(uint64_t size) : Slave(size), data(new uint64_t[(size >> 3) + 1]) {}

RAM::RAM(const char *str_size)
    : Slave(str_size), data(new uint64_t[(this->size >> 3) + 1]{0})
{
}

RAM::RAM(const char *init_file, const char *str_size)
    : Slave(str_size), data(new uint64_t[(this->size >> 3) + 1]{0})
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
        *((uint64_t *) (((uint8_t *) data) + addr)) = wdata;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        *((uint32_t *) (((uint8_t *) data) + addr)) = (uint32_t) wdata;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        *((uint16_t *) (((uint8_t *) data) + addr)) = (uint16_t) wdata;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        ((uint8_t *) data)[addr] = (uint8_t) wdata;
        break;
    default:
        abort();
    }
    return 1;
}
bool RAM::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = *((uint64_t *) (((uint8_t *) data) + addr));
    return 1;
}
