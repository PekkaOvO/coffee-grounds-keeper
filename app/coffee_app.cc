#include "coffee_keeper/app.h"

namespace coffee_keeper {
CoffeeStatus JudgeCoffeeStatus(float humidity,
                               const ThresholdConfig& threshold) {
    if (humidity < threshold.dry_humidity) {
        return CoffeeStatus::kTooDry;
    }

    if (humidity > threshold.ferment_humidity) {
        return CoffeeStatus::kTooDamp;
    }
    return CoffeeStatus::kNormal;
}

SystemStatus JudgeSystemStatus(ProcessMode mode, const EnvironmentData& data,
                               const ThresholdConfig& threshold) {
    if (!data.valid) {
        return SystemStatus::kError;
    }

    if (data.humidity < threshold.min_humidity ||
        data.humidity > threshold.max_humidity) {
        return SystemStatus::kError;
    }
    if (mode == ProcessMode::kFerment &&
        data.humidity > threshold.ferment_humidity) {
        return SystemStatus::kFermenting;
        // if process mode is kFerment and system status is not kFermenting,
        // then start pump and system status set to pumping,
        // when coffee status is kTooDamp, then stop pumping
    }

    if (mode == ProcessMode::kDry && data.humidity < threshold.dry_humidity) {
        return SystemStatus::kDrying;
        // if process mode is kDry and system status is not kDring,
        // then start fan and system status set to fanning
        // when coffee status is kTooDry, then stop fanning
    }
    return SystemStatus::kReady;
}

coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
                                            ProcessMode mode,
                                            const AppConfig& config) {
    coffee_keeper::DisplayData display_data;
    display_data.reading = data;
    display_data.mode = mode;
    display_data.coffee_status =
        JudgeCoffeeStatus(data.humidity, config.threshold);
    display_data.state = JudgeSystemStatus(mode, data, config.threshold);
    return display_data;
}
}  // namespace coffee_keeper
