#ifndef AHT20Sensor_h
#define AHT20Sensor_h

#include "includes/sensors/TemperatureHumiditySensor.h"
#include "dtos/TemperatureHumidityDto.h"
#include <Adafruit_AHTX0.h>

class AHT20Sensor : TemperatureHumiditySensor
{
public:
    Adafruit_AHTX0 aht;

    void initSensor()
    {
        // Initialize device.
        if (aht.begin())
        {
            Serial.println(F("AHT20 Unified Sensor"));

            getTemperatureAndHumidity();
        }
    }

    TemperatureHumidityDto getTemperatureAndHumidity()
    {
        // Get temperature event and print its value.
        sensors_event_t humidityEvent, tempEvent;
        TemperatureHumidityDto temperatureHumidityDto;

        aht.getEvent(&humidityEvent, &tempEvent);

        if (isnan(tempEvent.temperature))
        {
            Serial.println(F("Error reading temperature!"));
            temperatureHumidityDto.temperature = -273; // impossible - absolute zero
        }
        else
        {
            temperatureHumidityDto.temperature = tempEvent.temperature;
        }

        // Get humidity event and print its value.
        if (isnan(humidityEvent.relative_humidity))
        {
            Serial.println(F("Error reading humidity!"));
            temperatureHumidityDto.humidity = 0; // impossible on a world like Earth.
        }
        else
        {
            temperatureHumidityDto.humidity = humidityEvent.relative_humidity;
        }

        return temperatureHumidityDto;
    }
    AHT20Sensor()
    {
        aht = Adafruit_AHTX0();
    }
};

#endif