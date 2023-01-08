#ifndef __UTIL__
#define __UTIL__

#include <stdint.h>
#include <map>
#include <string>
#include <vector>
#define zext(x, len) ((uint64_t) (x) << 64 - (len) >> 64 - (len))
#define sext(x, len) ((int64_t) (x) << 64 - (len) >> 64 - (len))
#define bits_zext(x, high, low) \
    ((uint64_t) (x) << 63 - (high) >> 63 - (high) + (low))
#define bits_sext(x, high, low) \
    ((int64_t) (x) << 63 - (high) >> 63 - (high) + (low))
#define get_field(x, mask) (((x) & (mask)) / ((mask) & ~((mask) << 1)))
#define set_field(x, mask, value) \
    ((x) & ~(mask)) | ((value) * ((mask) & ~((mask) << 1)) & (mask))

uint64_t str2uint64_t(const char *str);
uint8_t clz(uint64_t x);
uint8_t ctz(uint64_t x);
uint64_t bit(const uint64_t &x, const uint8_t &idx);
uint64_t bit_signed(const uint64_t &x, const uint8_t &idx);

class ArgumentParser
{
    std::string prog_name;
    std::map<std::string, bool> name_attr;
    std::map<std::string, std::string> name_descr;
    std::map<std::string, std::string> name_values;
    std::map<std::string, std::string> flag2name;
    std::vector<std::string> values;
    uint64_t int_parse(const char *str);
    void help();

public:
    ArgumentParser();
    ~ArgumentParser();
    void add_argument(const char *flag,
                      const char *dest = "",
                      const char *help = "",
                      const char *def = "",
                      bool flag_only = 1);
    bool parse(int argc, char **argv);
    std::string get_str(const int &idx);
    std::string get_str(const char *name);
    uint64_t get_int(const int &idx);
    uint64_t get_int(const char *name);
    bool get_bool(const int &idx);
    bool get_bool(const char *name);
};
#endif
