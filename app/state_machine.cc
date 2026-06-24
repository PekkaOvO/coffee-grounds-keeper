#include "coffee_keeper/state_machine.h"

#include <sstream>

#include "coffee_keeper/app.h"
#include "coffee_keeper/config.h"
#include "coffee_keeper/types.h"

namespace coffee_keeper {
SystemStatus StateMachine::state() const { return state_; }
void StateMachine::Reset() { state_ = SystemStatus::kIdle; }
StateResult StateMachine::Check(ProcessMode mode, const EnvironmentData& data,
                                const ThresholdConfig& threshold) {
    state_ = SystemStatus::kChecking;
    StateResult result;
    if (!data.valid) {
        state_ = SystemStatus::kError;
        result.state = state_;
        result.err_message = "Sensor read failed";
        return result;
    }

    if (data.humidity < threshold.min_humidity || data.humidity > threshold.max_humidity) {
        state_ = SystemStatus::kError;
        result.state = state_;
        std::ostringstream message;
        message << "Humidity" << data.humidity << "is outside valid range ["
                << threshold.min_humidity << ", ]" << threshold.max_humidity;
        result.err_message = message.str();
        return result;
    }
    const CoffeeStatus coffee_status = JudgeCoffeeStatus(data.humidity, threshold);
    if (mode == ProcessMode::kDry) {
        state_ =
            coffee_status == CoffeeStatus::kTooDamp ? SystemStatus::kDrying : SystemStatus::kReady;
    } else {
        state_ = coffee_status == CoffeeStatus::kTooDry ? SystemStatus::kFermenting
                                                        : SystemStatus::kReady;
    }
    result.state = state_;
    return result;
}
}  // namespace coffee_keeper