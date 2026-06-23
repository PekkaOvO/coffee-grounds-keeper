#pragma once
#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"
namespace coffee_keeper {
CoffeeStatus JudgeCoffeeStatus(float humidity);
SystemStatus JudgeSystemStatus(ProcessMode mode, const EnvironmentData& data);
coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
                                            ProcessMode mode);
}  // namespace coffee_keeper