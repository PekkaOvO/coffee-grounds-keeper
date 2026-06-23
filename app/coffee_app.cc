#include"coffee_keeper/app.h"
#include "coffee_keeper/display.h"
#include "coffee_keeper/types.h"

namespace coffee_keeper{
CoffeeStatus JudgeCoffeeStatus(float humidity){
    if(humidity < 20.0f){
        return CoffeeStatus::kTooDry;
    }
    
    if(humidity > 70.0f){
        return CoffeeStatus::kTooDamp;
    }
    return CoffeeStatus::kNormal;
}

SystemStatus JudgeSystemStatus(ProcessMode mode, const EnvironmentData& data){
    if(!data.valid){
        return SystemStatus::kError;
    }

    if(data.humidity<0.0f||data.humidity>100.0f){
            return SystemStatus::kError;
    }
    if(mode == ProcessMode::kFerment && data.humidity < 70.0f){
           return SystemStatus::kFermenting; 
    }

    if(mode == ProcessMode::kDry && data.humidity > 20.0f){
           return SystemStatus::kDrying; 
    }
        return SystemStatus::kReady;
}

coffee_keeper::DisplayData BuildDisplayData(const EnvironmentData& data,
ProcessMode mode){
    coffee_keeper::DisplayData display_data;
    display_data.reading = data;
    display_data.mode = mode;
    display_data.coffee_status = JudgeCoffeeStatus(data.humidity);
    display_data.state = JudgeSystemStatus(mode,data);
    return display_data;
}
}