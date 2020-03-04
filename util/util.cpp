#include "util/util.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <string>

uint64_t mulhu(const uint64_t &a, const uint64_t &b)
{
    uint64_t a0(a & -1U);
    uint64_t b0(b & -1U);
    uint64_t a1(a >> 32);
    uint64_t b1(b >> 32);
    uint64_t t0, t1, t2, t3, res;

    t0 = a0 * b0;  //   **
    t1 = a1 * b0;  //  **
    t2 = a0 * b1;  //  **
    t3 = a1 * b1;  // **

    res = (t0 >> 32) + (t1 & -1U) + (t2 & -1U);
    res = res >> 32;
    res += (t1 >> 32) + (t2 >> 32) + t3;

    return res;
}

int64_t mulh(const int64_t &a, const int64_t &b)
{
    uint64_t mask((a >> 63) ^ (b >> 63));
    uint64_t abs_a((a ^ (a >> 63)) - (a >> 63));
    uint64_t abs_b((b ^ (b >> 63)) - (b >> 63));
    return (mulhu(abs_a, abs_b) ^ mask) - mask * (a * b == 0);
}

int64_t mulhsu(const int64_t &a, const uint64_t &b)
{
    uint64_t mask(a >> 63);
    uint64_t abs_a((a ^ mask) - mask);
    return (mulhu(abs_a, b) ^ mask) - mask * (a * b == 0);
}

uint64_t str2uint64_t(const char *str)
{
    std::string _str(str);
    uint64_t base(1), res;
    int len(_str.size() - 1);
    if ((_str[len] & 0xDF) == 'B') {
        base <<= 3;
        --len;
    }
    switch (_str[len] | 0x20) {
    case 'k':
        base <<= 10;
        break;
    case 'm':
        base <<= 20;
        break;
    case 'g':
        base <<= 30;
        break;
    case 't':
        base <<= 40;
        break;
    case 'p':
        base <<= 50;
        break;
    case 'e':
        base <<= 60;
        break;
    default:
        break;
    }
    res = base * atoi(_str.substr(0, len).c_str());
    return res >> 3;
}

uint8_t leading_zero(uint64_t x)
{
    uint8_t n(0);
    if (x <= 0x00000000ffffffffUL)
        n += 32, x <<= 32;
    if (x <= 0x0000ffffffffffffUL)
        n += 16, x <<= 16;
    if (x <= 0x00ffffffffffffffUL)
        n += 8, x <<= 8;
    if (x <= 0x0fffffffffffffffUL)
        n += 4, x <<= 4;
    if (x <= 0x3fffffffffffffffUL)
        n += 2, x <<= 2;
    if (x <= 0x7fffffffffffffffUL)
        n += 1;
    return n;
}

uint8_t lsb_leading_zero(uint64_t x)
{
    uint8_t n(0);
    if (!(x << 32))
        n += 32, x >>= 32;
    if (!(x << 48))
        n += 16, x >>= 16;
    if (!(x << 56))
        n += 8, x >>= 8;
    if (!(x << 60))
        n += 4, x >>= 4;
    if (!(x << 62))
        n += 2, x >>= 2;
    if (!(x << 63))
        n += 1;
    return n;
}


uint64_t bit(const uint64_t &x, const uint8_t &idx)
{
    return x & 1L << idx ? 1 : 0;
}

uint64_t bit_signed(const uint64_t &x, const uint8_t &idx)
{
    return x & 1L << idx ? -1L : 0;
}

uint64_t ArgumentParser::int_parse(const char *str)
{
    bool neg(0);
    uint64_t res(0);
    if (*str == '-')
        neg = 1, ++str;
    switch (*(uint16_t *) str) {
    case 0x5830:
    case 0x7830:  // 0x
        str += 2;
        while (1) {
            switch (*(str++) | 0x20) {
            case '0':
                res = res << 4 | 0x0;
                break;
            case '1':
                res = res << 4 | 0x1;
                break;
            case '2':
                res = res << 4 | 0x2;
                break;
            case '3':
                res = res << 4 | 0x3;
                break;
            case '4':
                res = res << 4 | 0x4;
                break;
            case '5':
                res = res << 4 | 0x5;
                break;
            case '6':
                res = res << 4 | 0x6;
                break;
            case '7':
                res = res << 4 | 0x7;
                break;
            case '8':
                res = res << 4 | 0x8;
                break;
            case '9':
                res = res << 4 | 0x9;
                break;
            case 'a':
                res = res << 4 | 0xa;
                break;
            case 'b':
                res = res << 4 | 0xb;
                break;
            case 'c':
                res = res << 4 | 0xc;
                break;
            case 'd':
                res = res << 4 | 0xd;
                break;
            case 'e':
                res = res << 4 | 0xe;
                break;
            case 'f':
                res = res << 4 | 0xf;
                break;
            default:
                return neg ? -res : res;
            }
        }
    case 0x4230:
    case 0x6230:
        str += 2;
        while (1) {
            switch (*(str++)) {
            case '0':
                res = res << 1 | 0x0;
                break;
            case '1':
                res = res << 1 | 0x1;
                break;
            default:
                return neg ? -res : res;
            }
        }
    default:
        while (1) {
            switch (*(str++)) {
            case '0':
                res = res * 10 + 0x0;
                break;
            case '1':
                res = res * 10 + 0x1;
                break;
            case '2':
                res = res * 10 + 0x2;
                break;
            case '3':
                res = res * 10 + 0x3;
                break;
            case '4':
                res = res * 10 + 0x4;
                break;
            case '5':
                res = res * 10 + 0x5;
                break;
            case '6':
                res = res * 10 + 0x6;
                break;
            case '7':
                res = res * 10 + 0x7;
                break;
            case '8':
                res = res * 10 + 0x8;
                break;
            case '9':
                res = res * 10 + 0x9;
                break;
            default:
                return neg ? -res : res;
            }
        }
    }
}

void ArgumentParser::help()
{
    int width(20);
    std::cout << "Usage: " << prog_name << " [-h]";
    for (std::map<std::string, std::string>::iterator it = flag2name.begin();
         it != flag2name.end(); ++it)
        std::cout << " [" << it->first << " " << it->second << "]";
    std::cout << std::endl << std::endl;
    std::cout << "optional arguments:" << std::endl;
    std::cout << "  " << std::left << std::setw(width) << "-h"
              << "  "
              << "show this help message and exit" << std::endl;
    for (std::map<std::string, std::string>::iterator it = flag2name.begin();
         it != flag2name.end(); ++it)
        std::cout << "  " << std::left << std::setw(width)
                  << (it->first + ", " + it->second) << "  "
                  << name_descr[it->second] << std::endl;
}

ArgumentParser::ArgumentParser() {}

ArgumentParser::~ArgumentParser() {}

void ArgumentParser::add_argument(const char *flag,
                                  const char *dest,
                                  const char *help,
                                  const char *def,
                                  bool flag_only)
{
    if (*flag != '-') {
        std::cerr << "Flag \"" << flag << "\""
                  << " is illigal." << std::endl;
        throw 1;
    }
    char name[100];
    if (!*dest) {
        int i(0);
        dest = flag;
        while (*dest == '-')
            dest++;
        while (*dest) {
            if (~*dest & 0x60)
                name[i++] = *dest;
            else
                name[i++] = *dest & 0xdf;
            dest++;
        }
        name[i] = 0;
        dest = name;
    }
    flag2name[std::string(flag)] = dest;
    name_attr[std::string(dest)] = flag_only;
    name_descr[std::string(dest)] = help;
    name_values[std::string(dest)] = flag_only ? "" : def;
}

bool ArgumentParser::parse(int argc, char **argv)
{
    std::string flag("");
    int i(1);
    prog_name = argv[0];
    for (; i < argc; ++i) {
        if (*argv[i] == '-' && !flag[0]) {
            flag = argv[i];
            if (flag == "-h") {
                help();
                return 0;
            }
            if (flag2name.find(flag) == flag2name.end()) {
                std::cerr << "Unknown flag \"" << flag << "\"" << std::endl;
                throw 1;
            }
            if (name_attr[flag2name[flag]]) {
                name_values[flag2name[flag]] = " ";
                flag = "";
            }
        } else {
            if (flag[0]) {
                name_values[flag2name[flag]] = argv[i];
                flag = "";
            } else {
                values.push_back(std::string(argv[i]));
            }
        }
    }
    return 1;
}

std::string ArgumentParser::get_str(const int &idx)
{
    return values[idx];
}

std::string ArgumentParser::get_str(const char *name)
{
    return name_values[std::string(name)];
}

uint64_t ArgumentParser::get_int(const int &idx)
{
    return int_parse(values[idx].c_str());
}

uint64_t ArgumentParser::get_int(const char *name)
{
    return int_parse(name_values[std::string(name)].c_str());
}

bool ArgumentParser::get_bool(const int &idx)
{
    return values[idx][0];
}

bool ArgumentParser::get_bool(const char *name)
{
    return name_values[std::string(name)][0];
}
