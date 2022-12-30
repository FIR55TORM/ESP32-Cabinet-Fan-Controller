#include "includes/sensors/TemperatureHumiditySensorManager.h"
#include "includes/Config.h"
#include "dtos/TemperatureHumidityDto.h"

#ifdef IS_USING_AHT20
#include "includes/sensors/AHT20Sensor.h"
AHT20Sensor *sensor = nullptr;
#else
#include "includes/sensors/DHT11Sensor.h"
DHT11Sensor *sensor = nullptr;
#endif

TemperatureHumidityDto TemperatureHumiditySensorManager::getTemperatureAndHumidity()
{
    return sensor->getTemperatureAndHumidity();
}

TemperatureHumiditySensorManager::TemperatureHumiditySensorManager()
{
    // No need to reinitialize the sensor if it is already made
    // Resorted to this rather than a singleton as it seems to be considered an anti-pattern 
    // in the C++ world. Knowing when and if it is destroyed seems to be one of the cons. 
    if (sensor != nullptr)
    {
        return;
    }

#ifdef IS_USING_AHT20
    sensor = new AHT20Sensor();
#else
    sensor = new DHT11Sensor();
#endif
    sensor->initSensor();
}