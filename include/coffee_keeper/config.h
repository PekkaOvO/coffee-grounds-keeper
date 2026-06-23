#pragma once

#include <string>

#include "coffee_keeper/types.h"

namespace coffee_keeper {
struct ThresholdConfig {
    float dry_humidity = 20.0f;
    float ferment_humidity = 70.0f;
    float min_humidity = 0.0f;
    float max_humidity = 100.0f;
};

struct AppConfig {
    std::string sensor_type = "mock";
    std::string display_type = "terminal";
    int refresh_interval_ms = 1000;
    std::string video_path = "./media/videos";
    ProcessMode default_mode = ProcessMode::kDry;
    ThresholdConfig threshold;
};

AppConfig LoadConfig(const std::string& path);

}  // namespace coffee_keeper
