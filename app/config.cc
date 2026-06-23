#include "coffee_keeper/config.h"

#ifdef _WIN32
#include <yaml-cpp/node/parse.h>
#endif
#include <yaml-cpp/yaml.h>

#include "coffee_keeper/types.h"

namespace coffee_keeper {
namespace {
ProcessMode ParseProcessMode(const std::string& mode) {
    if (mode == "ferment") {
        return ProcessMode::kFerment;
    }
    return ProcessMode::kDry;
}
}  // namespace

AppConfig LoadConfig(const std::string& path) {
    YAML::Node root = YAML::LoadFile(path);
    AppConfig config;
    config.sensor_type =
        root["sensor"]["temp_humidity"]["type"].as<std::string>();
    config.display_type = root["display"]["type"].as<std::string>();
    config.refresh_interval_ms =
        root["display"]["refresh_interval_ms"].as<int>();
    config.video_path = root["display"]["video_path"].as<std::string>();
    config.default_mode =
        ParseProcessMode(root["process"]["default_mode"].as<std::string>());

    config.threshold.dry_humidity =
        root["threshold"]["dry_humidity"].as<float>();
    config.threshold.ferment_humidity =
        root["threshold"]["ferment_humidity"].as<float>();
    config.threshold.min_humidity =
        root["threshold"]["min_humidity"].as<float>();
    config.threshold.max_humidity =
        root["threshold"]["max_humidity"].as<float>();

    return config;
}
}  // namespace coffee_keeper
