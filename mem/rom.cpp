#include "mem/rom.h"
#include "util/util.h"
#include <fstream>
#include <iostream>
using namespace std;

void ROM::_init_data(const char *init_file) {
  std::ifstream data_stream(init_file, std::ios::binary | std::ios::in);
  data_stream.read((char *)data, size);
}

ROM::ROM(const char *init_file, uint64_t size)
    : Slave(size), data(new uint64_t[size >> 3]) {
  _init_data(init_file);
}

ROM::ROM(const char *init_file, const char *str_size)
    : Slave(str_size), data(new uint64_t[this->size >> 3]) {
  _init_data(init_file);
}

ROM::~ROM() { delete[] data; }

bool ROM::write(const Addr &addr, const DataType &data_type,
                const uint64_t &wdata) {
  return 0;
}

bool ROM::read(const Addr &addr, const DataType &data_type, uint64_t &rdata) {
  switch (data_type) {
  case DATA_TYPE_DWORD:
    if (!_check_bound(addr, 8))
      return 0;
    rdata = *((uint64_t *)(((uint8_t *)data) + addr));
    return 1;
  case DATA_TYPE_WORD:
    if (!_check_bound(addr, 4))
      return 0;
    rdata = sext((*((uint32_t *)(((uint8_t *)data) + addr))), 32);
    return 1;
  case DATA_TYPE_WORD_UNSIGNED:
    if (!_check_bound(addr, 4))
      return 0;
    rdata = (uint64_t)(*((uint32_t *)(((uint8_t *)data) + addr)));
    return 1;
  case DATA_TYPE_HWORD:
    if (!_check_bound(addr, 2))
      return 0;
    rdata = sext((*((uint16_t *)(((uint8_t *)data) + addr))), 16);
    return 1;
  case DATA_TYPE_HWORD_UNSIGNED:
    if (!_check_bound(addr, 2))
      return 0;
    rdata = (uint64_t)(*((uint16_t *)(((uint8_t *)data) + addr)));
    return 1;
  case DATA_TYPE_BYTE:
    if (!_check_bound(addr, 1))
      return 0;
    rdata = sext(((uint8_t *)data)[addr], 8);
    return 1;
  case DATA_TYPE_BYTE_UNSIGNED:
    if (!_check_bound(addr, 1))
      return 0;
    rdata = (uint64_t)(((uint8_t *)data)[addr]);
    return 1;
  default:
    abort();
  }
}
