#include <string>
#include "Arduino.h"
#include <arduino-timer.h>
#include "greenhouse_io.h"
#include "rtc.h"
#include "globals.h"
#include "greenhouse_ble.h"

RTC_DS3231 rtc;
Timer<10> timer;


void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial); // TODO: Put watchdog timer here for all the infinite while loops waiting to start serial, io and BLE
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__)); //Set RTC to compile time
  setup_io();
  setupBLE();
  // Note: timer.every() is a software interrupt which causes more CPU load. timer.attachInterruptInterval() uses up a hardware interrupt and is more precise
  timer.every(TIME_INTERVAL, readPar); 
  timer.every(TIME_INTERVAL, readTemp);
  timer.every(TIME_INTERVAL, updateDli);  // integrate PPFD to DLI
  timer.every(TIME_INTERVAL, resetDli);  // reset DLI after 24 h 
  timer.every(TIME_INTERVAL, printSensorVals);
  timer.every(TIME_INTERVAL, writeBLE);
  //timer.every(TIME_INTERVAL, time_tester);
}

void loop() {
  now = rtc.now();
  timer.tick();
  Serial.println();
  printDateTime();
  Serial.println();
  if (!BLE.connected()) {
    Serial.println("BLE not connected, advertising.");
    BLE.advertise();
  } 
  else {
    Serial.println("BLE is connected, doing nothing.");
  }
  //Serial.println(BLE.connected());
  // wait a bit before reading again
  delay(TIME_INTERVAL);
}
