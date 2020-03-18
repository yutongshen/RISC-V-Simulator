#ifndef __BCD__
#define __BCD__

#include <stdint.h>
#include "fesvr/fedev.h"

class HTIF;

class BCD : public FrontEndDevice
{
public:
    BCD(HTIF *htif);
    ~BCD();
    uint64_t handle_read(uint64_t args);
    uint64_t handle_write(uint64_t args);
};

#endif
