#include "terminal_display.h"

#include <iostream>

#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"

namespace coffee_keeper {
namespace {
const char *ToString(ProcessMode mode) {
    switch (mode) {
        case ProcessMode::kDry:
            return "Dry";
        case ProcessMode::kFerment:
            return "Ferment";
    }
    return "Unknown";
}

const char *ToString(SystemStatus state) {
    switch (state) {
        case SystemStatus::kChecking:
            return "Checking";
        case SystemStatus::kDrying:
            return "Drying";
        case SystemStatus::kError:
            return "Error";
        case SystemStatus::kFermenting:
            return "Fermenting";
        case SystemStatus::kIdle:
            return "Idle";
        case SystemStatus::kReady:
            return "Ready";
    }
    return "Unknown";
}

const char *ToString(CoffeeStatus coffee_status) {
    switch (coffee_status) {
        case CoffeeStatus::kTooDry:
            return "TooDry";
        case CoffeeStatus::kTooDamp:
            return "TooDamp";
        case CoffeeStatus::kNormal:
            return "Normal";
    }
    return "Unknown";
}
}  // namespace
void TerminalDisplay::Show(const DisplayData &data) {
    std::cout << "Temperature: " << data.reading.temperature << " C\n"
              << "Humidity: " << data.reading.humidity << " %\n"
              << "Mode: " << ToString(data.mode) << "\n"
              << "Coffee Status: " << ToString(data.coffee_status) << "\n"
              << "State: " << ToString(data.state) << "\n"
              << std::endl;
}

void TerminalDisplay::ShowError(const std::string &message) {
    std::cerr << "[Error] " << message << std::endl;
}
}  // namespace coffee_keeper
