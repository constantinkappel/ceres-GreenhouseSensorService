#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>
#include "globals.h"

extern void setup_io();
extern bool readPar(void *); //read PAR sensor
extern bool readTemp(void *);
extern bool updateDli(void *); //update DLI
extern bool resetDli(void *); //reset DLI
extern bool printSensorVals(void *); //print sensor values