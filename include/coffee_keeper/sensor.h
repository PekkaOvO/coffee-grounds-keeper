#pragma once

#include "types.h"

namespace coffer_keeper {
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual EnvironmentData Read() = 0;
};
}  // namespace coffer_keeper