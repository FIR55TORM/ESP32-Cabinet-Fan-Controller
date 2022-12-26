//TODO: Likely will need to convert this into a Singleton class.
#ifndef TemperatureHumiditySensorManager_h
#define TemperatureHumiditySensorManager_h

#include "dtos/TemperatureHumidityDto.h"


class TemperatureHumiditySensorManager{
    public:
    TemperatureHumidityDto getTemperatureAndHumidity();
    TemperatureHumiditySensorManager();
};

#endif