#ifndef TemperatureHumiditySensor_h
#define TemperatureHumiditySensor_h

#include "dtos/TemperatureHumidityDto.h"

class TemperatureHumiditySensor {
    public:
    virtual void initSensor() = 0;
    virtual TemperatureHumidityDto getTemperatureAndHumidity();
};

#endif