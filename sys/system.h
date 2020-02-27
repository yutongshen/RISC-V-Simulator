#ifndef __SYSTEM__
#define __SYSTEM__

#include "dev/device.h"
#include <vector>

typedef Device *pDevice;

class System {
  std::vector<pDevice> devices;

public:
  System();
  ~System();
  void run();
  void add(pDevice dev);
};

#endif
