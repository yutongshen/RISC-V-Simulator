#ifndef __DEVICE__
#define __DEVICE__

#include "bus/bus.h"

class Device {
  static uint8_t num;
  static void _cnt_num();

protected:
  uint8_t d_id;
  virtual void _init() = 0;

public:
  Device();
  ~Device();
  virtual void run() = 0;
};

#endif
