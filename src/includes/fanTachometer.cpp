
#include <Arduino.h>
#include <esp32-hal.h>
#include <pins_arduino.h>

const int tachoPin                             = 33;
const int tachoUpdateCycle                     = 1000; // how often tacho speed shall be determined, in milliseconds
const int numberOfInterruptsInOneSingleRotation = 2;    // Number of interrupts ESP32 sees on tacho signal on a single fan rotation. All the fans I've seen trigger two interrups.

static volatile int counter_rpm = 0;
int last_rpm = 0;
unsigned long millisecondsLastTachoMeasurement = 0;

// Interrupt counting every rotation of the fan
// https://desire.giesecke.tk/index.php/2018/01/30/change-global-variables-from-isr/
void IRAM_ATTR rpm_fan() {
  counter_rpm++;
}

void initTacho() {
  pinMode(tachoPin, INPUT);
  digitalWrite(tachoPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(tachoPin), rpm_fan, FALLING);
  Serial.printf("  Fan tacho detection successfully initialized.\r\n");
}

void updateTacho() {
  // start of tacho measurement
  if ((unsigned long)(millis() - millisecondsLastTachoMeasurement) >= tachoUpdateCycle)
  { 
    // detach interrupt while calculating rpm
    detachInterrupt(digitalPinToInterrupt(tachoPin)); 
    // calculate rpm
    last_rpm = counter_rpm * ((float)60 / (float)numberOfInterruptsInOneSingleRotation) * ((float)1000 / (float)tachoUpdateCycle);
    // Log.printf("fan rpm = %d\r\n", last_rpm);

    // reset counter
    counter_rpm = 0; 
    // store milliseconds when tacho was measured the last time
    millisecondsLastTachoMeasurement = millis();

    // attach interrupt again
    attachInterrupt(digitalPinToInterrupt(tachoPin), rpm_fan, FALLING);
  }
}

int readFanTacho(){
  return last_rpm;
}