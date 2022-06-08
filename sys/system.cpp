#include "system.h"
#include <stdio.h>

System::System() : devices(std::vector<pDevice>()) {}

System::~System()
{
    // int n(devices.size());
    // while (n--)
    //   delete devices[n];
}

void System::run()
{
    int n(devices.size()), i(0);
    while (n--) {
        // devices[i++]->run();
        devices[i++]->single_step();
    }
}

void System::stop()
{
    int n(devices.size()), i(0);
    while (n--) {
        devices[i++]->stop();
    }
}

void System::add(pDevice dev)
{
    devices.push_back(dev);
}
