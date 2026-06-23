#pragma once
#include "coffee_keeper/config.h"
#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"
namespace coffee_keeper {
CoffeeStatus JudgeCoffeeStatus(float humidity,
                               const ThresholdConfig& threshold);
SystemStatus JudgeSystemStatus(ProcessMode mode, const EnvironmentData& data,
                               const ThresholdConfig& threshold);
coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
                                            ProcessMode mode,
                                            const AppConfig& config);
}  // namespace coffee_keeper
