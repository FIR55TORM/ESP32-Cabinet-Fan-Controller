//#pragma once
#include "includes/Config.h"
#include "sensors/TemperatureHumiditySensorManager.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <Bounce2.h>
#include "includes/fanTachometer.h"
#include "dtos/TemperatureHumidityDto.h"
#include "includes/fanPWM.h"
#include "includes/ImageData.h"

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// OLED FeatherWing buttons map to different pins depending on board:
#if defined(ESP8266)
#define BUTTON_A 0
#define BUTTON_B 16
#define BUTTON_C 2
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14
#elif defined(ARDUINO_STM32_FEATHER)
#define BUTTON_A PA15
#define BUTTON_B PC7
#define BUTTON_C PC5
#elif defined(TEENSYDUINO)
#define BUTTON_A 4
#define BUTTON_B 3
#define BUTTON_C 8
#elif defined(ARDUINO_NRF52832_FEATHER)
#define BUTTON_A 31
#define BUTTON_B 30
#define BUTTON_C 27
#else // 32u4, M0, M4, nrf52840, esp32-s2 and 328p
#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5
#endif

const int DEBOUNCE_INTERVAL = 50; // ms

Bounce buttonABounce = Bounce();
Bounce buttonBBounce = Bounce();
Bounce buttonCBounce = Bounce();

enum currently_displayed_menu
{
    connectionDetails,
    tempAndFanSpeed,
    none
};

currently_displayed_menu menu = none;

unsigned long currentTime;
unsigned long previousTime = 0;
const unsigned long screenUpdateInterval = 33; // because 30fps, in theory...

unsigned long currentScreenTimeout;
unsigned long previousScreenTimeout = 0;
const unsigned long screenTimeoutUpdateInterval = 60000; // Turn off screen after 60 seconds
bool isScreenOff = false;

void resetDisplay()
{
    display.clearDisplay();
    display.display();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    display.setCursor(0, 0);
}

void initDisplay()
{
    delay(250); // wait for OLED power up

    display.begin(0x3C, true);
    Serial.println("OLED Display Initialized");

    display.clearDisplay();
    display.setRotation(1);

    // Display Zinkr Logo
    display.drawBitmap(0, 0, epd_bitmap_ZinkrLogo, 128, 64, 1);
    display.display();
    delay(2000);

    buttonABounce.attach(BUTTON_A, INPUT_PULLUP);
    buttonBBounce.attach(BUTTON_B, INPUT_PULLUP);
    buttonCBounce.attach(BUTTON_C, INPUT_PULLUP);

    buttonABounce.interval(DEBOUNCE_INTERVAL);
    buttonBBounce.interval(DEBOUNCE_INTERVAL);
    buttonCBounce.interval(DEBOUNCE_INTERVAL);

    resetDisplay();
}

void displayConnectionDetails()
{
    display.setTextSize(1);
    if (WiFi.status() != WL_CONNECTED)
    {
        display.setCursor(0, 0);
        display.println("No WiFi. AP mode On");

        display.print("IP: ");
        display.println(WiFi.softAPIP());

        display.print("SSID: ");
        display.println(WiFi.softAPSSID());
    }
    else
    {
        display.setCursor(0, 0);
        display.print("IP: ");
        display.println(WiFi.localIP());

        display.print("SSID: ");
        display.println(WiFi.SSID());

        long wifiStrengthDbM = WiFi.RSSI() * -1; // RSSI value is out of 100 according to ESP docs.
        long wifiStrengthPercentage = wifiStrengthDbM * -1 + 100;

        display.setCursor(50, 28);
        display.println("WiFi Strength");
        display.setTextSize(2);
        display.setCursor(50, 38);
        display.print(wifiStrengthPercentage);
        display.print("% ");

        display.fillRect(5, 28, 38, 27, SH110X_BLACK); // blank out wifi symbol area first

        if (wifiStrengthDbM >= 90) // No connection to really weak...
            display.drawBitmap(5, 28, epd_bitmap_allWifiArray[0], 38, 27, SH110X_WHITE);

        if (wifiStrengthDbM >= 75 && wifiStrengthDbM <= 89) // weak...
            display.drawBitmap(5, 28, epd_bitmap_allWifiArray[1], 38, 27, SH110X_WHITE);

        if (wifiStrengthDbM >= 50 && wifiStrengthDbM <= 74) // half...
            display.drawBitmap(5, 28, epd_bitmap_allWifiArray[2], 38, 27, SH110X_WHITE);

        if (wifiStrengthDbM >= 25 && wifiStrengthDbM <= 49) // fullish...
            display.drawBitmap(5, 28, epd_bitmap_allWifiArray[3], 38, 27, SH110X_WHITE);

        if (wifiStrengthDbM >= 0 && wifiStrengthDbM <= 24) // full...
            display.drawBitmap(5, 28, epd_bitmap_allWifiArray[4], 38, 27, SH110X_WHITE);
    }
}

int fanAnimationFrameCounter = -1;
unsigned long previousMillisecondsForWarning = 0;
const unsigned long intervalForWarning = 1000;
bool isBlinking = true;

void clearTachoScreenArea()
{
    if (menu == tempAndFanSpeed)
    {
        display.fillRect(0, 29, 89, 64, SH110X_BLACK); // Clear that area of screen space
    }
}

void displayTempAndFanSpeed()
{
    int fanSpeed = getFanSpeedPercentage();
    TemperatureHumiditySensorManager temperatureHumiditySensorManager = TemperatureHumiditySensorManager();
    TemperatureHumidityDto dto = temperatureHumiditySensorManager.getTemperatureAndHumidity();

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Fan Speed: ");
    display.print("     "); // crudely wipe that part of the screen, this fixes the percentage sign doubling up on the update, when fan at 100% speed
    display.setCursor(78, 0);
    display.print(fanSpeed);
    display.println("%");

    display.setCursor(0, 8);
    display.print("Temperature: ");
    display.print(dto.temperature);
    display.print(" C");
    display.drawCircle(111, 10, 1, SH110X_WHITE); // degrees sign
    display.println();

    display.print("Humidity: ");
    display.setCursor(78, 17);
    display.print(ceilf(dto.humidity));
    display.println("%");

    int tacho = readFanTacho();

    if (tacho == 0)
    {
        long currentMilliseconds = millis();

        if ((currentMilliseconds - previousMillisecondsForWarning) >= intervalForWarning)
        {
            previousMillisecondsForWarning = currentMilliseconds;

            isBlinking = !isBlinking; // toggle every interval

            if (isBlinking)
            {
                display.drawBitmap(5, 36, epd_bitmap_warningSign, 16, 14, SH110X_WHITE);
                display.setCursor(29, 35);
                display.println("Fans Not");
                display.setCursor(29, 44);
                display.println("Running!");
            }
            else
            {
                clearTachoScreenArea();
            }
        }
    }
    else
    {
        clearTachoScreenArea();
        display.setTextSize(2);
        display.setCursor(5, 36);
        display.print(tacho);
        display.println("rpm ");
    }

    // Display Fan Image
    fanAnimationFrameCounter++;
    if (fanAnimationFrameCounter == epd_bitmap_allFanArray_LEN || tacho == 0)
    {
        fanAnimationFrameCounter = 0;
    }

    display.fillRect(90, 26, 38, 38, SH110X_BLACK);
    display.drawBitmap(90, 26, epd_bitmap_allFanArray[fanAnimationFrameCounter], 38, 38, SH110X_WHITE);
}

void updateDisplay()
{
    buttonABounce.update();
    buttonBBounce.update();
    buttonCBounce.update();

    if (buttonABounce.changed() && buttonABounce.read() == HIGH)
    {
        menu = connectionDetails;
        previousScreenTimeout = 0;
        resetDisplay();
    }

    if (buttonBBounce.changed() && buttonBBounce.read() == HIGH)
    {
        menu = tempAndFanSpeed;
        previousScreenTimeout = 0;
        resetDisplay();
    }

    if (buttonCBounce.changed() && buttonCBounce.read() == HIGH)
    {
        previousScreenTimeout = 0;
        menu = none;
    }

    currentTime = millis();

    if (currentTime - previousTime >= screenUpdateInterval)
    {
        previousTime += screenUpdateInterval;
        previousScreenTimeout += screenUpdateInterval;

        if (previousScreenTimeout < screenTimeoutUpdateInterval)
        {
            switch (menu)
            {
            case connectionDetails:
                displayConnectionDetails();
                break;

            case tempAndFanSpeed:
                displayTempAndFanSpeed();
                break;

            default:
                resetDisplay();
                break;
            }
        }
        else
        {
            resetDisplay();
            menu = none;
        }

        yield();
        display.display();
    }
}