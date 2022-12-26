#include "includes/Config.h"
#include "sensors/TemperatureHumiditySensorManager.h"
#include "dtos/TemperatureHumidityDto.h"
#include "includes/ControllerConfigManager.h"
#include "includes/fanPWM.h"
#include <Arduino.h>

int fanSpeedPercentage = 100; // Initially set to 100%

void setFanSpeedFromTemperature()
{
    TemperatureHumiditySensorManager temperatureHumiditySensorManager = TemperatureHumiditySensorManager();
    TemperatureHumidityDto dto = temperatureHumiditySensorManager.getTemperatureAndHumidity();
    float temperatureDelta = dto.temperature - config.targetTemperature;
    int fanSpeedMinimumPercentage = 15;
    int fanSpeedMaximumPercentage = 85; // for future use

#ifndef IS_USING_AHT20
    // This exists to account for the error prone DHT11 Sensor. Prevents the fans spinning up to 100% randomly. Usually corrects itself.
    if (!isnan(dto.temperature))
    {
        temperatureDelta = 0.5f;
    }
#endif

    if (temperatureDelta <= 0.0)
    {
        // Temperature is below target. Keep to minimum speed
        fanSpeedPercentage = fanSpeedMinimumPercentage;
    }
    else if (temperatureDelta <= 0.5)
    {
        fanSpeedPercentage = 55;
    }
    else if (temperatureDelta <= 1.0)
    {
        fanSpeedPercentage = 63;
    }
    else if (temperatureDelta <= 1.5)
    {
        fanSpeedPercentage = 71;
    }
    else if (temperatureDelta <= 2.0)
    {
        fanSpeedPercentage = 78;
    }
    else if (temperatureDelta <= 2.5)
    {
        fanSpeedPercentage = 86;
    }
    else if (temperatureDelta <= 3.0)
    {
        fanSpeedPercentage = 94;
    }
    else
    {
        // Temperature much too high. Run fan at full speed.
        fanSpeedPercentage = 100;
    }

    setFanPWM(fanSpeedPercentage);
}

int getCurrentFanSpeedPercentage()
{
    return getFanSpeedPercentage();
}
