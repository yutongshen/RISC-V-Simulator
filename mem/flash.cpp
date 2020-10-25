#include "mem/flash.h"
#include "util/util.h"
using namespace std;

void Flash::copyfile(const char *name, const char *cp_name)
{
    std::ifstream src(name, std::ios::in | std::ios::binary);
    std::ofstream dst(cp_name, std::ios::out | std::ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();
}

Flash::Flash(const char *file_name, uint64_t size) : Slave(size)
{
    int ptr(0);
    for (; file_name[ptr]; ++ptr)
        ;
    if (!ptr)
        abort();
    for (--ptr; ptr >= 0 && file_name[ptr] != '/'; --ptr)
        ;
    ++ptr;
    std::string tmp_file(file_name + ptr);
    tmp_file += ".";
    copyfile(file_name, tmp_file.c_str());
    data.open(tmp_file, std::ios::binary | std::ios::in | std::ios::out);
}

Flash::Flash(const char *file_name, const char *str_size) : Slave(str_size)
{
    int ptr(0);
    for (; file_name[ptr]; ++ptr)
        ;
    if (!ptr)
        abort();
    for (--ptr; ptr >= 0 && file_name[ptr] != '/'; --ptr)
        ;
    ++ptr;
    std::string tmp_file(file_name + ptr);
    tmp_file = std::string(".") + tmp_file;
    copyfile(file_name, tmp_file.c_str());
    data.open(tmp_file, std::ios::binary | std::ios::in | std::ios::out);
}

Flash::~Flash()
{
    data.close();
}

bool Flash::write(const Addr &addr,
                  const DataType &data_type,
                  const uint64_t &wdata)
{
    uint8_t len(0);

    switch (data_type) {
    case DATA_TYPE_DWORD:
        len = 8;
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        len = 4;
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        len = 2;
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        len = 1;
        break;
    default:
        abort();
    }

    // std::cout << std::hex << "ADDR: " << addr << " DATA: " << wdata <<
    // std::endl;
    if (!_check_bound(addr, len))
        return 0;
    data.seekg(addr);
    data.write((char *) &wdata, len);
    return 1;
}

bool Flash::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    uint8_t len(0);
    bool _is_signed(0);

    switch (data_type) {
    case DATA_TYPE_DWORD:
        len = 8;
        break;
    case DATA_TYPE_WORD:
        _is_signed = 1;
    case DATA_TYPE_WORD_UNSIGNED:
        len = 4;
        break;
    case DATA_TYPE_HWORD:
        _is_signed = 1;
    case DATA_TYPE_HWORD_UNSIGNED:
        len = 2;
        break;
    case DATA_TYPE_BYTE:
        _is_signed = 1;
    case DATA_TYPE_BYTE_UNSIGNED:
        len = 1;
        break;
    default:
        abort();
    }

    if (!_check_bound(addr, len))
        return 0;

    // Solve that the file's data at read address is empty
    data.seekg(0, ios::end);
    if (addr + len > data.tellg()) {
        char buff(0);
        data.seekg(addr + len - 1);
        data.write(&buff, 1);
        rdata = 0;
        return 1;
    }

    data.seekg(addr);
    data.read((char *) &rdata, len);
    rdata = _is_signed ? sext(rdata, len << 3) : zext(rdata, len << 3);
    return 1;
}
