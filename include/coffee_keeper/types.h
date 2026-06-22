#pragma once

#include <cstdint>
enum class ProcessMode {
    kDry,
    kFerment,
};

enum class CoffeeStatus {
    kTooDry,
    kTooDamp,
    kNormal,
};

enum class SystemStatus : std::uint8_t {
    kIdle,
    kChecking,
    kDrying,
    kFermenting,
    kReady,
    kError,
};

// temperature and humidity
struct EnvironmentData {
    float temperature_ = 0.0f;
    float humidity_ = 0.0f;
    bool valid = false;
};
