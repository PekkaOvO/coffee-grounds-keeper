#pragma once

#include <string>

#include "types.h"
namespace coffee_keeper {
struct DisplayData {
    EnvironmentData reading;
    ProcessMode mode;
    SystemStatus state;
};

class Display {
public:
    virtual ~Display() = default;
    virtual void Show(const DisplayData& data) = 0;
    virtual void ShowError(const std::string& message) = 0;
};
}  // namespace coffee_keeper