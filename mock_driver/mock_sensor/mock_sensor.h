#pragma once

#include <random>

#include "../../include/coffee_keeper/sensor.h"

namespace coffee_keeper {

class MockSensor : public Sensor {
public:
    MockSensor();
    EnvironmentData Read() override;

private:
    std::mt19937 rng_;
    std::uniform_real_distribution<float> temperature_dist_;
    std::uniform_real_distribution<float> humidity_dist_;
    std::uniform_int_distribution<int> failure_dist_;
};
}  // namespace coffee_keeper