#include <exception>

#include "coffee_keeper/app.h"
#include "coffee_keeper/config.h"
#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"
#include "mock_sensor.h"
#include "terminal_display.h"

int main(int argc, char** argv) {
    coffee_keeper::MockSensor sensor;
    coffee_keeper::TerminalDisplay display;

    const char* config_path = argc > 1 ? argv[1] : "config/default.yaml";

    coffee_keeper::AppConfig config;
    try {
        config = coffee_keeper::LoadConfig(config_path);
    } catch (const std::exception& error) {
        display.ShowError(error.what());
        return 1;
    }

    ProcessMode mode = config.default_mode;

    for (int i = 0; i < 10; ++i) {
        const EnvironmentData data = sensor.Read();
        if (!data.valid) {
            display.ShowError("Sensor read failed");
            continue;
        }

        coffee_keeper::DisplayData display_data =
            coffee_keeper::BuildDisplayData(data, mode, config);
        display.Show(display_data);
    }
    return 0;
}
