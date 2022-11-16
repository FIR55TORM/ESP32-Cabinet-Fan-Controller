// #include <Adafruit_Sensor.h>
// #include <DHT.h>
// #include <DHT_U.h>
// #include "includes/DHT11Sensor.h"
 #include "dtos/TemperatureHumidityDto.h"

#define DHTPIN 27
#define DHTTYPE DHT11 // DHT 11

void initDHTSensor();
TemperatureHumidityDto getTemperatureAndHumidity();