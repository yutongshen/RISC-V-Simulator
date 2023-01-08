#include "system.h"
#include <assert.h>

System::System() : devices(std::vector<pDevice>()) {}

System::~System() {}

void System::run()
{
    for (int i = 0; i < devices.size(); i++)
        for (int times = freqs[i]; times--;)
            devices[i]->single_step();
}

void System::stop()
{
    int n(devices.size()), i(0);
    while (n--) {
        devices[i++]->stop();
    }
}

void System::add(pDevice dev, uint32_t freq)
{
    assert(freq > 0);
    devices.push_back(dev);
    freqs.push_back(freq);
}
