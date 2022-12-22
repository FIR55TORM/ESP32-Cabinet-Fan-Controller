#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "includes/DHT11Sensor.h"
#include "dtos/TemperatureHumidityDto.h"

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void initDHTSensor()
{
    // Initialize device.
    dht.begin();
    Serial.println(F("DHT11 Unified Sensor"));

    getTemperatureAndHumidity();
}

TemperatureHumidityDto getTemperatureAndHumidity()
{
    // Get temperature event and print its value.
    sensors_event_t event;
    TemperatureHumidityDto temperatureHumidityDto;

    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
        temperatureHumidityDto.temperature = -273; //impossible - absolute zero
    }
    else
    {
        temperatureHumidityDto.temperature = event.temperature;
    }

    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
        temperatureHumidityDto.humidity = 0; //impossible on a world like Earth.
    }
    else
    {
        temperatureHumidityDto.humidity = event.relative_humidity;
    }

    return temperatureHumidityDto;
}