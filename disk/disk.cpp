#include "disk/disk.h"
#include "util/util.h"
using namespace std;

void Disk::copyfile(const char *name, const char *cp_name)
{
    std::ifstream src(name, std::ios::in | std::ios::binary);
    std::ofstream dst(cp_name, std::ios::out | std::ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();
}

Disk::Disk(const char *file_name, uint64_t size) : Slave(size)
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

Disk::Disk(const char *file_name, const char *str_size) : Slave(str_size)
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

Disk::~Disk()
{
    data.close();
}

bool Disk::write(const Addr &addr,
                 const DataType &data_type,
                 const uint64_t &wdata)
{
    switch (data_type) {
    case DATA_TYPE_DWORD:
        if (!_check_bound(addr, 8))
            return 0;
        data.seekg(addr);
        data.write((char *) &wdata, 8);
        break;
    case DATA_TYPE_WORD:
    case DATA_TYPE_WORD_UNSIGNED:
        if (!_check_bound(addr, 4))
            return 0;
        data.seekg(addr);
        data.write((char *) &wdata, 4);
        break;
    case DATA_TYPE_HWORD:
    case DATA_TYPE_HWORD_UNSIGNED:
        if (!_check_bound(addr, 2))
            return 0;
        data.seekg(addr);
        data.write((char *) &wdata, 2);
        break;
    case DATA_TYPE_BYTE:
    case DATA_TYPE_BYTE_UNSIGNED:
        if (!_check_bound(addr, 1))
            return 0;
        data.seekg(addr);
        data.write((char *) &wdata, 1);
        break;
    default:
        abort();
    }
    return 1;
}

bool Disk::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    // data.seekg(0, ios::end);
    // std::cout << "END: " << data.tellg() << endl;
    switch (data_type) {
    case DATA_TYPE_DWORD:
        if (!_check_bound(addr, 8))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 8 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 8);
        break;
    case DATA_TYPE_WORD:
        if (!_check_bound(addr, 4))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 4 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 4);
        rdata = sext(rdata, 32);
        break;
    case DATA_TYPE_WORD_UNSIGNED:
        if (!_check_bound(addr, 4))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 4 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 4);
        rdata = zext(rdata, 32);
        break;
    case DATA_TYPE_HWORD:
        if (!_check_bound(addr, 2))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 2 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 2);
        rdata = sext(rdata, 16);
        break;
    case DATA_TYPE_HWORD_UNSIGNED:
        if (!_check_bound(addr, 2))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 2 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 2);
        rdata = zext(rdata, 16);
        break;
    case DATA_TYPE_BYTE:
        if (!_check_bound(addr, 1))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 1 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 1);
        rdata = sext(rdata, 8);
        break;
    case DATA_TYPE_BYTE_UNSIGNED:
        if (!_check_bound(addr, 1))
            return 0;
        data.seekg(0, ios::end);
        if (addr + 1 > data.tellg()) {
            rdata = 0;
            return 1;
        }
        data.seekg(addr);
        data.read((char *) &rdata, 1);
        rdata = zext(rdata, 8);
        break;
    default:
        abort();
    }
    return 1;
}
