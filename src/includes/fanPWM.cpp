#include <Arduino.h>
#include <esp32-hal.h>
#include <esp32-hal-ledc.h>
#include <math.h>
#include "includes/ControllerConfigManager.h"

const int pwmPin = 13;
const int pwmFreq = 25000;
const int pwmChannel = 0;
const int pwmResolution = 8;
const int fanMaxRPM = 2500;
int pwmValue = 0;
int pwmMaxValue = 255;

void initFanPWM()
{
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(pwmPin, pwmChannel);
    ledcWrite(pwmChannel, pwmValue);
}

void setFanPWM(int speedInPercent)
{
    if (speedInPercent > 100)
    {
        throw std::invalid_argument("speed is more that 100. Must be percentage");
    }

    pwmValue = (int)ceilf((float)pwmMaxValue / (float)100 * (float)speedInPercent);
    ledcWrite(pwmChannel, pwmValue);

    config.fanSpeedPercentage = speedInPercent;

    saveConfiguration();
}

void setAutomaticMode(int value)
{   
    if (value == 1)
    {
        config.isAutomaticMode = true;
    }
    else
    {
        config.isAutomaticMode = false;
    }

    saveConfiguration();
}

void setTargetTemperature(int value){
    config.targetTemperature = value;

    saveConfiguration();
}

int getFanSpeedPercentage()
{
    return (int)ceilf((float)pwmValue / (float)pwmMaxValue * (float)100);
}