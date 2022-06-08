#ifndef __SYSTEM__
#define __SYSTEM__

#include <vector>
#include "dev/device.h"

typedef Device *pDevice;

class System
{
    std::vector<pDevice> devices;

public:
    System();
    ~System();
    void run();
    void stop();
    void add(pDevice dev);
};

#endif
