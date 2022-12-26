#include "includes/sensors/TemperatureHumiditySensorManager.h"
#include "includes/Config.h"
#include "dtos/TemperatureHumidityDto.h"

#ifdef IS_USING_AHT20
#include "includes/sensors/AHT20Sensor.h"
AHT20Sensor sensor;
#else
#include "includes/sensors/DHT11Sensor.h"
DHT11Sensor sensor;
#endif

TemperatureHumidityDto TemperatureHumiditySensorManager::getTemperatureAndHumidity()
{
    return sensor.getTemperatureAndHumidity();
}

TemperatureHumiditySensorManager::TemperatureHumiditySensorManager()
{
    sensor.initSensor();
}