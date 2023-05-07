#include <ArduinoBLE.h>
#include "globals.h"

//Server name
#define bleServerName "Greenhouse"

// UUIDs of services
extern const char* ServiceName;
extern const char* uuidOfTempService;
extern const char* uuidOfTempChar;
extern const char* uuidOfTempWarnChar;
extern const char* uuidOfNotifyDLIChar; // extends the daily light intake to the light switch as client
extern const char* uuidOfCumDLIChar; // Reports cumulative DLI accrued within a day
extern const char* uuidOfHumidityChar; //current humidity
extern const char* uuidOfParChar; // PAR 

extern bool setupBLE();
extern bool writeBLE(void *); //write to BLE