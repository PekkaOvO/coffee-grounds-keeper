#include "coffee_keeper/app.h"
#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"
#include "mock_sensor.h"
#include "terminal_display.h"

int main(int, char **) {
    coffee_keeper::MockSensor sensor;
    coffee_keeper::TerminalDisplay display;

    for (int i = 0; i < 10; ++i) {
        const EnvironmentData data = sensor.Read();
        ProcessMode mode = ProcessMode::kDry;
        if (!data.valid) {
            display.ShowError("Sensor read failed");
            continue;
        }

        coffee_keeper::DisplayData display_data =
            coffee_keeper::BuildDisplayData(data, mode);
        display.Show(display_data);
    }
    return 0;
}
