#pragma once

#include "../../include/coffee_keeper/display.h"

namespace coffee_keeper {
class TerminalDisplay : public Display {
public:
    void Show(const DisplayData& data) override;
    void ShowError(const std::string& message) override;
};
}  // namespace coffee_keeper
