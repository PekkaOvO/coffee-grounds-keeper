# Phase 3 State Machine and Error Handling Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Complete phase 3 by making system status flow explicit and by reporting sensor, config, and display errors without crashing silently.

**Architecture:** Keep humidity classification in `app/coffee_app.cc`. Add a small `StateMachine` in `app/state_machine.cc` to turn mode plus sensor data into `SystemStatus` and optional error text. In phase 3, `kDrying` means the system should perform the drying action, and `kFermenting` means the system should perform the water-addition or fermentation action.

**Tech Stack:** C++17-style project with CMake, yaml-cpp, terminal mock display, mock sensor.

---

## File Map

- Modify `include/coffee_keeper/types.h`: keep existing enums; no new enum values needed.
- Create `include/coffee_keeper/state_machine.h`: declare `StateResult` and `StateMachine`.
- Modify `app/state_machine.cc`: implement state transition rules.
- Modify `app/coffee_app.cc`: remove actuator comments and make `JudgeSystemStatus` consistent with the state machine.
- Modify `include/coffee_keeper/app.h`: add a `BuildDisplayData` overload that accepts an already-decided `SystemStatus`.
- Modify `mock_driver/terminal_display/terminal_display.cc`: confirm `kDrying` and `kFermenting` are displayed clearly.
- Modify `app/config.cc`: validate threshold order and throw clear config errors.
- Modify `main.cc`: use `StateMachine`, handle errors, and catch display exceptions.
- Modify `CMakeLists.txt`: add `app/state_machine.cc` to the executable.

---

### Task 1: Confirm Terminal State Display

**Files:**
- Modify: `mock_driver/terminal_display/terminal_display.cc`

- [ ] **Step 1: Confirm `SystemStatus` string cases**

In `ToString(SystemStatus state)`, keep these cases:

```cpp
case SystemStatus::kDrying:
    return "Drying";
case SystemStatus::kFermenting:
    return "Fermenting";
```

- [ ] **Step 2: Build**

Run:

```bash
cmake --build build
```

Expected: build exits with code `0`.

---

### Task 2: Define the State Machine Interface

**Files:**
- Create: `include/coffee_keeper/state_machine.h`

- [ ] **Step 1: Create the header**

```cpp
#pragma once

#include <string>

#include "coffee_keeper/config.h"
#include "coffee_keeper/types.h"

namespace coffee_keeper {

struct StateResult {
    SystemStatus state = SystemStatus::kIdle;
    std::string error_message;
};

class StateMachine {
public:
    SystemStatus state() const;
    void Reset();
    StateResult Check(ProcessMode mode, const EnvironmentData& data,
                      const ThresholdConfig& threshold);

private:
    SystemStatus state_ = SystemStatus::kIdle;
};

}  // namespace coffee_keeper
```

- [ ] **Step 2: Add the implementation file to CMake**

In `CMakeLists.txt`, add `app/state_machine.cc` to `add_executable(...)`.

- [ ] **Step 3: Build to confirm the missing implementation errors**

Run:

```bash
cmake -S . -B build
cmake --build build
```

Expected: compile or link fails because `StateMachine` methods are declared but not implemented.

---

### Task 3: Implement State Transitions

**Files:**
- Modify: `app/state_machine.cc`

- [ ] **Step 1: Implement the state machine**

```cpp
#include "coffee_keeper/state_machine.h"

#include <sstream>

#include "coffee_keeper/app.h"

namespace coffee_keeper {

SystemStatus StateMachine::state() const {
    return state_;
}

void StateMachine::Reset() {
    state_ = SystemStatus::kIdle;
}

StateResult StateMachine::Check(ProcessMode mode, const EnvironmentData& data,
                                const ThresholdConfig& threshold) {
    state_ = SystemStatus::kChecking;

    StateResult result;
    if (!data.valid) {
        state_ = SystemStatus::kError;
        result.state = state_;
        result.error_message = "Sensor read failed";
        return result;
    }

    if (data.humidity < threshold.min_humidity ||
        data.humidity > threshold.max_humidity) {
        state_ = SystemStatus::kError;
        result.state = state_;

        std::ostringstream message;
        message << "Humidity " << data.humidity
                << " is outside valid range [" << threshold.min_humidity
                << ", " << threshold.max_humidity << "]";
        result.error_message = message.str();
        return result;
    }

    const CoffeeStatus coffee_status = JudgeCoffeeStatus(data.humidity, threshold);
    if (mode == ProcessMode::kDry) {
        state_ = coffee_status == CoffeeStatus::kTooDamp ? SystemStatus::kDrying
                                                         : SystemStatus::kReady;
    } else {
        state_ = coffee_status == CoffeeStatus::kTooDry ? SystemStatus::kFermenting
                                                        : SystemStatus::kReady;
    }

    result.state = state_;
    return result;
}

}  // namespace coffee_keeper
```

- [ ] **Step 2: Build**

Run:

```bash
cmake --build build
```

Expected: build exits with code `0`.

---

### Task 4: Make `coffee_app` Use the Same State Rules

**Files:**
- Modify: `app/coffee_app.cc`
- Modify: `include/coffee_keeper/app.h`

- [ ] **Step 1: Replace the old `JudgeSystemStatus` body**

Use the same mapping:

```cpp
if (!data.valid) {
    return SystemStatus::kError;
}

if (data.humidity < threshold.min_humidity ||
    data.humidity > threshold.max_humidity) {
    return SystemStatus::kError;
}

const CoffeeStatus coffee_status = JudgeCoffeeStatus(data.humidity, threshold);
if (mode == ProcessMode::kDry) {
    return coffee_status == CoffeeStatus::kTooDamp ? SystemStatus::kDrying
                                                   : SystemStatus::kReady;
}

if (mode == ProcessMode::kFerment) {
    return coffee_status == CoffeeStatus::kTooDry ? SystemStatus::kFermenting
                                                  : SystemStatus::kReady;
}

return SystemStatus::kReady;
```

- [ ] **Step 2: Add a `BuildDisplayData` overload**

In `include/coffee_keeper/app.h`:

```cpp
coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
                                            ProcessMode mode,
                                            const AppConfig& config,
                                            SystemStatus state);
```

In `app/coffee_app.cc`:

```cpp
coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
                                            ProcessMode mode,
                                            const AppConfig& config,
                                            SystemStatus state) {
    coffee_keeper::DisplayData display_data;
    display_data.reading = data;
    display_data.mode = mode;
    display_data.coffee_status =
        JudgeCoffeeStatus(data.humidity, config.threshold);
    display_data.state = state;
    return display_data;
}
```

- [ ] **Step 3: Keep the old overload as a convenience wrapper**

```cpp
coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
                                            ProcessMode mode,
                                            const AppConfig& config) {
    return BuildDisplayData(
        data, mode, config,
        JudgeSystemStatus(mode, data, config.threshold));
}
```

- [ ] **Step 4: Build**

Run:

```bash
cmake --build build
```

Expected: build exits with code `0`.

---

### Task 5: Validate Config Thresholds

**Files:**
- Modify: `app/config.cc`

- [ ] **Step 1: Include `<stdexcept>`**

```cpp
#include <stdexcept>
```

- [ ] **Step 2: Add validation after reading threshold values**

```cpp
if (config.threshold.min_humidity > config.threshold.max_humidity) {
    throw std::runtime_error("Invalid config: min_humidity is greater than max_humidity");
}

if (config.threshold.dry_humidity < config.threshold.min_humidity ||
    config.threshold.dry_humidity > config.threshold.max_humidity) {
    throw std::runtime_error("Invalid config: dry_humidity is outside min/max range");
}

if (config.threshold.ferment_humidity < config.threshold.min_humidity ||
    config.threshold.ferment_humidity > config.threshold.max_humidity) {
    throw std::runtime_error("Invalid config: ferment_humidity is outside min/max range");
}

if (config.threshold.dry_humidity >= config.threshold.ferment_humidity) {
    throw std::runtime_error("Invalid config: dry_humidity must be less than ferment_humidity");
}
```

- [ ] **Step 3: Build**

Run:

```bash
cmake --build build
```

Expected: build exits with code `0`.

---

### Task 6: Use the State Machine in `main`

**Files:**
- Modify: `main.cc`

- [ ] **Step 1: Include the state machine header**

```cpp
#include "coffee_keeper/state_machine.h"
```

- [ ] **Step 2: Create the state machine after display**

```cpp
coffee_keeper::StateMachine state_machine;
```

- [ ] **Step 3: Prefix config errors clearly**

Change the config catch block to:

```cpp
} catch (const std::exception& error) {
    display.ShowError(std::string("Config load failed: ") + error.what());
    return 1;
}
```

Also add:

```cpp
#include <string>
```

- [ ] **Step 4: Replace the loop body with state-machine flow**

```cpp
const EnvironmentData data = sensor.Read();
const coffee_keeper::StateResult state_result =
    state_machine.Check(mode, data, config.threshold);

if (state_result.state == SystemStatus::kError) {
    display.ShowError(state_result.error_message);
    continue;
}

coffee_keeper::DisplayData display_data =
    coffee_keeper::BuildDisplayData(data, mode, config, state_result.state);
display.Show(display_data);
```

- [ ] **Step 5: Build**

Run:

```bash
cmake --build build
```

Expected: build exits with code `0`.

---

### Task 7: Catch Display Exceptions

**Files:**
- Modify: `main.cc`

- [ ] **Step 1: Wrap display output in `try/catch`**

Inside the loop:

```cpp
try {
    if (state_result.state == SystemStatus::kError) {
        display.ShowError(state_result.error_message);
        continue;
    }

    coffee_keeper::DisplayData display_data =
        coffee_keeper::BuildDisplayData(data, mode, config, state_result.state);
    display.Show(display_data);
} catch (const std::exception& error) {
    display.ShowError(std::string("Display failed: ") + error.what());
    return 1;
}
```

- [ ] **Step 2: Build**

Run:

```bash
cmake --build build
```

Expected: build exits with code `0`.

---

### Task 8: Manual Verification

**Files:**
- No file changes.

- [ ] **Step 1: Configure and build**

Run:

```bash
cmake -S . -B build
cmake --build build
```

Expected: both commands exit with code `0`.

- [ ] **Step 2: Run with the default config**

Run:

```bash
./build/coffee-grounds-keeper config/default.yaml
```

Expected: terminal prints temperature, humidity, mode, coffee status, and state. State should now include `Ready`, `Drying`, or `Fermenting`.

- [ ] **Step 3: Run with a missing config**

Run:

```bash
./build/coffee-grounds-keeper config/not-exist.yaml
```

Expected: terminal prints an error beginning with:

```text
[Error] Config load failed:
```

- [ ] **Step 4: Temporarily test invalid config**

Temporarily change `config/default.yaml` so `dry_humidity` is greater than `ferment_humidity`, then run:

```bash
./build/coffee-grounds-keeper config/default.yaml
```

Expected: terminal prints:

```text
[Error] Config load failed: Invalid config: dry_humidity must be less than ferment_humidity
```

Restore `config/default.yaml` after this check.

---

## State Rules

Use this table as the source of truth for phase 3:

| Mode | Coffee Status | System Status | Meaning |
| --- | --- | --- | --- |
| Dry | TooDamp | Drying | Fan/drying action is needed |
| Dry | Normal | Ready | No action needed |
| Dry | TooDry | Ready | Dry enough for this phase |
| Ferment | TooDry | Fermenting | Water-addition/fermentation action is needed |
| Ferment | Normal | Ready | No action needed |
| Ferment | TooDamp | Ready | Stop adding water |
| Any | Sensor invalid | Error | Sensor read failed |
| Any | Humidity outside min/max | Error | Sensor/config value is suspicious |

`Idle` means the system has not started checking yet. `Checking` is an internal transition before deciding the next visible state. `Ready` means the latest check is valid and no action is needed.
