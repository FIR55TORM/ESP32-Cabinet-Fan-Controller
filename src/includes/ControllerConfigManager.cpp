#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>
#include "dtos/ControllerConfig.h"
#include "includes/fanPWM.h"

const char *configurationFilename = "/config.txt";
ControllerConfig config;

void saveConfiguration(const char *configurationFilename, const ControllerConfig &controllerConfig)
{
    SD.remove(configurationFilename);

    File configFile = SD.open(configurationFilename, FILE_WRITE);

    if (!configFile)
    {
        Serial.println(F("Failed to create file"));
        return;
    }

    StaticJsonDocument<256> doc;

    doc["isAutomaticMode"] = controllerConfig.isAutomaticMode;
    doc["fanSpeedPercentage"] = controllerConfig.fanSpeedPercentage;
    doc["targetTemperature"] = controllerConfig.targetTemperature;

    // Serialize JSON to file
    if (serializeJson(doc, configFile) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }

    // Close the file
    configFile.close();
}

void saveConfiguration()
{
    saveConfiguration(configurationFilename, config);
}

void loadConfiguration(const char *configurationFilename, ControllerConfig &controllerConfig)
{
    File file = SD.open(configurationFilename);

    StaticJsonDocument<512> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));

    // Copy values from the JsonDocument to the Config
    controllerConfig.isAutomaticMode = doc["isAutomaticMode"] | false;
    controllerConfig.fanSpeedPercentage = doc["fanSpeedPercentage"] | 50;
    controllerConfig.targetTemperature = doc["targetTemperature"] | 25;

    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();

    // set Controller values
    setFanPWM(controllerConfig.fanSpeedPercentage);
}

// Prints the content of a file to the Serial
void printFile(const char *configurationFilename)
{
    // Open file for reading
    File file = SD.open(configurationFilename);
    if (!file)
    {
        Serial.println(F("Failed to read file"));
        return;
    }

    // Extract each characters by one by one
    while (file.available())
    {
        Serial.print((char)file.read());
    }
    Serial.println();

    // Close the file
    file.close();
}

void initConfigFile()
{
    // default values
    config.isAutomaticMode = false;
    config.fanSpeedPercentage = 50;
    config.targetTemperature = 25;

    if (!SD.exists(configurationFilename))
    {
        Serial.println("No Config File, Writing New with default values");

        StaticJsonDocument<200> doc;

        doc["isAutomaticMode"] = config.isAutomaticMode;
        doc["fanSpeedPercentage"] = config.fanSpeedPercentage;
        doc["targetTemperature"] = config.targetTemperature;

        saveConfiguration(configurationFilename, config);
    }
    else
    {
        loadConfiguration(configurationFilename, config);
    }
}