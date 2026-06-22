#include "mock_sensor.h"

#include <chrono>

#include "../../include/coffee_keeper/types.h"

namespace coffee_keeper {
MockSensor::MockSensor()
    : rng_(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())),
      temperature_dist_(20.0f, 35.0f),
      humidity_dist_(15.0f, 75.0f),
      failure_dist_(1, 100) {}

EnvironmentData MockSensor::Read() {
    EnvironmentData data;

    // 5% are failures
    if (failure_dist_(rng_) <= 5) {
        data.valid = false;
        return data;
    }

    data.temperature = temperature_dist_(rng_);
    data.humidity = humidity_dist_(rng_);
    data.valid = true;
    return data;
}
}  // namespace coffee_keeper
