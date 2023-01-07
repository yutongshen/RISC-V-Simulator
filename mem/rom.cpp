#include "mem/rom.h"
#include <fstream>
#include <iostream>
#include "util/util.h"
using namespace std;

void ROM::_init_data(const char *init_file)
{
    std::ifstream data_stream(init_file, std::ios::binary | std::ios::in);
    if (!data_stream)
        abort();
    data_stream.read((char *) data, size);
}

ROM::ROM(const char *init_file, uint64_t size)
    : Slave(size), data(new uint64_t[(size >> 3) + 1])
{
    _init_data(init_file);
}

ROM::ROM(const char *init_file, const char *str_size)
    : Slave(str_size), data(new uint64_t[(this->size >> 3) + 1])
{
    _init_data(init_file);
}

ROM::~ROM()
{
    delete[] data;
}

bool ROM::write(const Addr &addr,
                const DataType &data_type,
                const uint64_t &wdata)
{
    return 0;
}

bool ROM::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = *((uint64_t *) (((uint8_t *) data) + addr));
    return 1;
}
