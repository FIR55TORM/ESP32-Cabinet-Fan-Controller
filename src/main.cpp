#define CORS_DEBUG true

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "SPIFFS.h"
#include "AsyncJson.h"
#include <ArduinoJson.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <AsyncMqttClient.h>
#include "includes/DHT11Sensor.h"
#include "includes/fanPWM.h"
#include "dtos/TemperatureHumidityDto.h"
#include "includes/ControllerConfigManager.h"
#include "includes/TemperatureControlManager.h"
#include "includes/fanTachometer.h"
#include "includes/SensorDataLoggerManager.h"
#include "includes/AdafruitOLED64x128.h"

// ajax parameters in HTTP POST request for Wifi
const char *PARAM_INPUT_SSID = "ssid";
const char *PARAM_INPUT_PASSKEY = "passkey";
const char *PARAM_INPUT_IP = "deviceIp";
const char *PARAM_INPUT_GATEWAY = "gatewayIp";

// ajax parameters in HTTP POST request for MQTT
const char *PARAM_INPUT_IS_USING_MQTT = "isUsingMQTT";
const char *PARAM_INPUT_CLIENT_NAME = "clientName";
const char *PARAM_INPUT_BROKER_IP = "brokerIp";
const char *PARAM_INPUT_BROKER_PORT = "brokerPort";
const char *PARAM_INPUT_BROKER_USERNAME = "brokerUsername";
const char *PARAM_INPUT_BROKER_PASSWORD = "brokerPassword";

// Wifi variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;

// MQTT variables to save values from HTML form
bool useMQTT = false;
String mqttServerIp;
String mqttServerPort;
String mqttUsername;
String mqttPassword;
String mqttClientName;

// wifi & MQTT config file
const char *configPath = "/config.json";

IPAddress localIP;
// Set your Gateway IP address
IPAddress localGateway;
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMilliseconds = 0;
unsigned long intervalOneSecond = 1000;
unsigned long previousMilliseconds5000Cycle = 0;
unsigned long intervalFiveSeconds = 5000;
unsigned long previousMilliseconds10000Cycle = 0;
unsigned long intervalTenSeconds = 10000;

const long intervalWifi = 10000; // interval to wait for Wi-Fi connection (milliseconds)

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create Event source for live data send back to client browser
AsyncEventSource events("/events");

// MQTT Config
AsyncMqttClient mqttClient;

// MQTT
void onMqttConnect(bool sessionPresent)
{
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  mqttClient.publish("test/lol", 0, true, "test 1");
  Serial.println("Publishing at QoS 0");
  uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
  Serial.print("Publishing at QoS 1, packetId: ");
  Serial.println(packetIdPub1);
  uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
  Serial.print("Publishing at QoS 2, packetId: ");
  Serial.println(packetIdPub2);
}

void initMQTT()
{
  if (!useMQTT)
  {
    return;
  }

  mqttClient.onConnect(onMqttConnect);
  mqttClient.setServer("192.168.1.110", 1883);
  mqttClient.setCredentials("homeassistant", "ieX4shaimieveiShe4quaiW7ea2vienaeV0Ii8hae9ietheePieTu0iepeeNg8fe");
  mqttClient.setClientId("Cabinet Fan Controller");
}

// Initialize SPIFFS
void initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Read File from SPIFFS
String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
}

// Initialize SD
void initSDCard()
{
  if (!SD.begin(4))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

// Initialize WiFi
bool initWiFi()
{
  if (ssid == "" || ip == "")
  {
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  // Esp_wifi_set_ps (WIFI_PS_NONE); //stop power saving mode
  WiFi.setSleep(false);
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());

  if (!WiFi.config(localIP, localGateway, subnet))
  {
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMilliseconds = currentMillis;
  int reconnectionCounter = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    if (currentMillis - previousMilliseconds >= intervalWifi)
    {
      Serial.println("Failed to connect.");

      if (reconnectionCounter == 3)
      {
        return false;
      }

      reconnectionCounter++;
    }
  }

  // previousMilliseconds = 0; // reset for use in loop later
  Serial.println(WiFi.localIP());
  return true;
}

void initWifiAndMQTTDetails()
{
  DynamicJsonDocument json(1024);
  String jsonString = readFile(SPIFFS, configPath);

  Serial.println(jsonString);

  if (DeserializationError::Ok == deserializeJson(json, jsonString.c_str()))
  {
    JsonObject obj = json.as<JsonObject>();

    // Wifi
    ssid = obj[PARAM_INPUT_SSID].as<String>().c_str();
    pass = obj[PARAM_INPUT_PASSKEY].as<String>().c_str();
    ip = obj[PARAM_INPUT_IP].as<String>().c_str();
    gateway = obj[PARAM_INPUT_GATEWAY].as<String>().c_str();

    // MQTT
    useMQTT = obj[PARAM_INPUT_IS_USING_MQTT].as<bool>();
    mqttServerIp = obj[PARAM_INPUT_BROKER_IP].as<String>().c_str();
    mqttServerPort = obj[PARAM_INPUT_BROKER_PORT].as<String>().c_str();
    mqttUsername = obj[PARAM_INPUT_BROKER_USERNAME].as<String>().c_str();
    mqttPassword = obj[PARAM_INPUT_BROKER_PASSWORD].as<String>().c_str();
    mqttClientName = obj[PARAM_INPUT_CLIENT_NAME].as<String>().c_str();
  }
}

void initEventSources()
{
  events.onConnect([](AsyncEventSourceClient *client)
                   {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID: %u\n", client->lastId());
    }
    //send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000); });

  server.addHandler(&events);
}

AsyncWebHandler handleStaticAssets(AsyncWebServerRequest *request, String path)
{
}

void setup()
{
  Serial.begin(115200);

  initDisplay();
  initSPIFFS();
  initDHTSensor();
  initFanPWM();
  initTacho();
  initWifiAndMQTTDetails();
  initSDCard();
  initConfigFile();
  // initSensorDataLogger();

  if (initWiFi())
  {
    initEventSources();
    initMQTT();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html", false); });
    server.serveStatic("/", SPIFFS, "/");

    // Handle Web Assets
    // server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
    //           {
    //             AsyncWebServerResponse *response  = request->beginResponse(SPIFFS, "assets/index.js.gz", "text/javascript", false);
    //             response->addHeader("Content-Encoding", "gzip");
    //             request->send(response);
    //             Serial.println("beep");
    //             });

    // Route to get Sensor Data
    server.on("/api/get-sensors", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response = request->beginResponseStream("application/json");
                DynamicJsonDocument json(1024);
                TemperatureHumidityDto temps = getTemperatureAndHumidity();
                json["humidity"] = temps.humidity;
                json["temperature"] = temps.temperature;
                serializeJson(json, *response);
                request->send(response); });

    // Route to set fan speed
    AsyncCallbackJsonWebHandler *setFanSpeedHandler = new AsyncCallbackJsonWebHandler(
        "/api/set-fan-speed",
        [](AsyncWebServerRequest *request, JsonVariant json)
        {
          JsonObject jsonObj = json.as<JsonObject>();

          if (jsonObj.containsKey("speed"))
          {
            setFanPWM(jsonObj["speed"].as<int>());
            request->send(200);
          }
          else
          {
            request->send(404);
          }
        });

    server.addHandler(setFanSpeedHandler);

    // Route to set automatic mode
    AsyncCallbackJsonWebHandler *setAutomaticModeHandler = new AsyncCallbackJsonWebHandler(
        "/api/set-automatic-mode",
        [](AsyncWebServerRequest *request, JsonVariant json)
        {
          JsonObject jsonObj = json.as<JsonObject>();

          if (jsonObj.containsKey("isAutomaticMode"))
          {
            setAutomaticMode(jsonObj["isAutomaticMode"].as<int>());
            request->send(200);
          }
          else
          {
            request->send(404);
          }
        });

    server.addHandler(setAutomaticModeHandler);

    // Route to set target temperature
    AsyncCallbackJsonWebHandler *setTargetTemperatureHandler = new AsyncCallbackJsonWebHandler(
        "/api/set-target-temperature",
        [](AsyncWebServerRequest *request, JsonVariant json)
        {
          JsonObject jsonObj = json.as<JsonObject>();

          if (jsonObj.containsKey("targetTemperature"))
          {
            setTargetTemperature(jsonObj["targetTemperature"].as<int>());
            request->send(200);
          }
          else
          {
            request->send(404);
          }
        });

    server.addHandler(setTargetTemperatureHandler);

    // Route to get configuration settings
    server.on("/api/get-config", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response = request->beginResponseStream("application/json");
                DynamicJsonDocument json(1024); 
                json["fanSpeedPercentage"] = getFanSpeedPercentage();
                json["isAutomaticMode"] = config.isAutomaticMode;
                json["targetTemperature"] = config.targetTemperature;
                serializeJson(json, *response);
                request->send(response); });

    // Route to get configuration settings
    server.on("/api/read-fan-speed", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response = request->beginResponseStream("application/json");
                DynamicJsonDocument json(1024); 
                json["fanTacho"] = readFanTacho();                
                json["currentFanSpeedPercentage"] = getCurrentFanSpeedPercentage();                
                serializeJson(json, *response);
                request->send(response); });

    // Route to print configuration settings to serial
    server.on("/api/print-config", HTTP_GET, [](AsyncWebServerRequest *request)
              {                
                 printFile("/config.txt");
                request->send(200); });

#ifdef CORS_DEBUG == true
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
#endif

    server.begin();
  }
  else
  {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("CABINET-FAN-WIFI-MANAGER", "Qwerty123");

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });

    server.serveStatic("/", SPIFFS, "/");

    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                         { 
                            if(request->url() == "/WifiDetails" && request->method() == HTTP_POST)
                            {
                              DynamicJsonDocument json(1024); 

                              if (DeserializationError::Ok == deserializeJson(json, (const char*)data))
                              {
                                JsonObject obj = json.as<JsonObject>();

                                String jsonString = json.as<String>();
                                writeFile(SPIFFS, configPath, jsonString.c_str());

                                request->send(200, "text/plain", "Done. FAN CONTROL will restart, connect to your router and go to IP address: " + ip);
                                delay(3000);
                                ESP.restart();
                              }
                            } });

    server.begin();
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    updateTacho();
    updateDisplay();

    unsigned long currentMilliseconds = millis();

    // functions called every 1000 ms
    if ((currentMilliseconds - previousMilliseconds) >= intervalOneSecond)
    {
      previousMilliseconds = currentMilliseconds;

      if (config.isAutomaticMode)
      {
        setFanSpeedFromTemperature();
      }
    }

    // functions called every 10000 ms
    if ((currentMilliseconds - previousMilliseconds10000Cycle) >= intervalTenSeconds)
    {
    }

    // functions called every 5000 ms
    if ((currentMilliseconds - previousMilliseconds5000Cycle) >= intervalFiveSeconds)
    {
      previousMilliseconds5000Cycle = currentMilliseconds;

      // Temps
      DynamicJsonDocument tempsJson(1024);
      TemperatureHumidityDto temps = getTemperatureAndHumidity();
      tempsJson["humidity"] = temps.humidity;
      tempsJson["temperature"] = temps.temperature;
      String stringifiedTempsJson;
      serializeJson(tempsJson, stringifiedTempsJson);

      events.send(stringifiedTempsJson.c_str(), "onTemperatureHumidityReading", millis());

      // Tacho
      DynamicJsonDocument tachoJson(1024);
      tachoJson["fanTacho"] = readFanTacho();
      tachoJson["currentFanSpeedPercentage"] = getCurrentFanSpeedPercentage();
      String stringifiedTachoJson;
      serializeJson(tachoJson, stringifiedTachoJson);

      events.send(stringifiedTachoJson.c_str(), "onFanTachoReading", millis());

      // Data Logging
      // printLocalTime();
    }
  }
}