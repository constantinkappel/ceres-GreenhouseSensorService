#include "greenhouse_io.h"
#include "globals.h"
#include "rtc.h"

uint8_t isDark = 0;
int writeLength = 0;
float airTemp = 0.0;
float humidity = 0.0;
int r = 0;
int g = 0;
int b = 0;
int c = 0;
double par = 0.0;
double dli = 0.0;
unsigned long secondsElapsed = 0;
unsigned long timeOffset = 0;
char bleBuffer[BLE_BUFFER_SIZES];
char serialBuffer[BLE_BUFFER_SIZES];
DateTime now;


void setup_io() {
    int colorAvailable = 0;

    DPRINTLN("Setting up APDS sensor...");
    if (!APDS.begin()) {
        Serial.println("Error initializing APDS-9960 sensor.");
        while (1);
        }
    /*
    colorAvailable = APDS.colorAvailable();
    delay(5);
    if (!APDS.colorAvailable()) {
      Serial.println("Error initializing APDS-9960 color reading.");
    }
    */

    //sprintf(serialBuffer, "Color available %d", colorAvailable);
    //Serial.println(serialBuffer);

    DPRINTLN("Setting up HTS sensor...");
    if (!HTS.begin()) {
      Serial.println("Failed to initialize humidity temperature sensor!");
      while (1);
      }
    }

bool readPar(void *) {
    while (! APDS.colorAvailable()) {
      delay(5);
    }
    // read the color
    APDS.readColor(r, g, b, c); //Colour.pop(colourData);
    par = (c*0.65847) + (r*1.60537) + (g*2.30216) + (b*0.50019); // These calibration values from Kutschera et al http://dx.doi.org/10.4236/ajps.2018.912175
    if (par < DARK_THRESH) { // this is rather simplistic. More sophisticated would be to use the rgb light color
        isDark = 1;
    }
    else {
        isDark = 0;
    }
    return true; // needed for timer
    }

bool readTemp(void *) {
    airTemp = HTS.readTemperature();
    humidity = HTS.readHumidity();
    return true;
    }

bool updateDli(void *) {
  DateTime startOfDay(now.year(), now.month(), now.day(), 0, 0, 0);
  TimeSpan elapsedTime = now - startOfDay;
  secondsElapsed = elapsedTime.totalseconds();
  //DPRINTLN(secondsElapsed);
  dli += par*1e-6*secondsElapsed/S_PER_DAY;
  return true;
}


bool resetDli(void *) {
  // Note we introduce a 3s buffer to ensure we don't miss the reset in case it gets delayed.
  // This will introduce a small error in updateDli and could lead to negative values around midnight
  // However, unless it is summer north of the Arctic Circle, it will be dark anyways, so should be OK. 
  if (secondsElapsed >= (S_PER_DAY-3)) {
  secondsElapsed = 0;
  dli = 0;
  }
  return true;
}
 

bool printSensorVals(void *) {
    memset(serialBuffer, 0, sizeof(serialBuffer));
    sprintf(serialBuffer, "R: %d, G: %d, B: %d, White: %d, PAR: %3.3f [µmol/(m²*s) adds up to DLI: %3.5f [mol/(m²*day)] after %lu seconds. \r\n", r, g, b, c, par, dli, secondsElapsed);
    Serial.print(serialBuffer);
    sprintf(serialBuffer, "Air Temperature [°C]: %f, Humidity: %f\r\n", airTemp, humidity);
    Serial.print(serialBuffer);    
    //DPRINTF("Is dark: %d, Light On: %d\r\n", isDark, lightOn);
    //DPRINTF("timeOffset: %d", timeOffset);
    return true; 
}