#pragma once

#include "types.h"

namespace coffee_keeper {
class Sensor {
public:
  virtual ~Sensor() = default;
  virtual EnvironmentData Read() = 0;
};
} // namespace coffee_keeper