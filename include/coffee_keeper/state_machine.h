#pragma once
#include <string>

#include "coffee_keeper/config.h"
#include "coffee_keeper/types.h"

namespace coffee_keeper {
struct StateResult {
    SystemStatus state = SystemStatus::kIdle;
    std::string err_message;
};
class StateMachine {
public:
    SystemStatus state() const;
    void Reset();
    StateResult Check(ProcessMode mode, const EnvironmentData& data, const ThresholdConfig& config);

private:
    SystemStatus state_ = SystemStatus::kIdle;
};
}  // namespace coffee_keeper