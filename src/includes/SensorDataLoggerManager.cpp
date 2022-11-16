#include "Arduino.h"
#include "time.h"
#include "includes/DHT11Sensor.h"
#include "dtos/TemperatureHumidityDto.h"
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <WiFi.h>
// #include <WiFiUdp.h>

//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
const char *dataLogFilename = "data.txt";

RTC_DATA_ATTR int readingID = 0;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
TemperatureHumidityDto temperatureHumidityDto;

// void getTimeStamp()
// {
//     while (!timeClient.update())
//     {
//         timeClient.forceUpdate();
//     }
//     // The formattedDate comes with the following format:
//     // 2018-05-28T16:00:13Z
//     // We need to extract date and time
//     formattedDate = timeClient.getFormattedTime();
//     Serial.println(formattedDate);

//     // Extract date
//     int splitT = formattedDate.indexOf("T");
//     dayStamp = formattedDate.substring(0, splitT);
//     Serial.println(dayStamp);
//     // Extract time
//     timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
//     Serial.println(timeStamp);
// }

// void writeLogFile(fs::FS &fs, const char *path, const char *message)
// {
//     Serial.printf("Writing file: %s\n", path);

//     File file = fs.open(path, FILE_WRITE);
//     if (!file)
//     {
//         Serial.println("Failed to open file for writing");
//         return;
//     }
//     if (file.print(message))
//     {
//         Serial.println("File written");
//     }
//     else
//     {
//         Serial.println("Write failed");
//     }
//     file.close();
// }

// void appendLogFile(fs::FS &fs, const char *path, const char *message)
// {
//     Serial.printf("Appending to file: %s\n", path);

//     File file = fs.open(path, FILE_APPEND);
//     if (!file)
//     {
//         Serial.println("Failed to open file for appending");
//         return;
//     }
//     if (file.print(message))
//     {
//         Serial.println("Message appended");
//     }
//     else
//     {
//         Serial.println("Append failed");
//     }
//     file.close();
// }

// void logSDCard()
// {
//     DynamicJsonDocument dataMessageJson(1024);
//     dataMessageJson["id"] = esp_random();
//     dataMessageJson["date"] = dayStamp;
//     dataMessageJson["time"] = timeStamp;
//     dataMessageJson["temperature"] = temperatureHumidityDto.temperature;
//     dataMessageJson["humidity"] = temperatureHumidityDto.humidity;
//     String dataMessage;
//     serializeJson(dataMessageJson, dataMessage);
//     dataMessage = dataMessage + "\r\n";

//     Serial.print("Save data: ");
//     Serial.println(dataMessage);
//     appendLogFile(SD, dataLogFilename, dataMessage.c_str());
// }

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

bool initSensorDataLogger()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);    
    return true;

    // timeClient.begin();
    // timeClient.setTimeOffset(0); // GMT 0

    // // If the data.txt file doesn't exist
    // // Create a file on the SD card and write the data labels
    // File file = SD.open(dataLogFilename);

    // if (!file)
    // {
    //     Serial.println("File doesn't exist");
    //     Serial.println("Creating data logging file...");
    //     writeLogFile(SD, dataLogFilename, "Reading ID, Date, Hour, Temperature \r\n");
    // }
    // else
    // {
    //     Serial.println("File already exists");
    // }

    // file.close();

    // temperatureHumidityDto = getTemperatureAndHumidity();
    // getTimeStamp();
    // logSDCard();
}