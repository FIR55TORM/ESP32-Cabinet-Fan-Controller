#ifndef TemperatureHumidityDto_h
#define TemperatureHumidityDto_h

class TemperatureHumidityDto
{
public:
    float temperature;
    float humidity;
    TemperatureHumidityDto(float _temperature, float _humidity)
    {
        temperature = _temperature;
        humidity = _humidity;
    }
    TemperatureHumidityDto(){};
};
#endif