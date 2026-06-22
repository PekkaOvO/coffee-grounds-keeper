#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"
#include "mock_sensor.h"
#include "terminal_display.h"

int main(int, char **) {
    coffee_keeper::MockSensor sensor;
    coffee_keeper::TerminalDisplay display;

    for (int i = 0; i < 10; ++i) {
        EnvironmentData data = sensor.Read();
        if (!data.valid) {
            display.ShowError("Sensor read failed");
            continue;
        }

        coffee_keeper::DisplayData display_data;
        display_data.reading = data;
        display_data.mode = ProcessMode::kDry;
        display_data.state = SystemStatus::kChecking;

        display.Show(display_data);
    }
    return 0;
}
