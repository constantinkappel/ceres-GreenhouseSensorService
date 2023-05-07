#include <stdint.h>

#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define DARK_THRESH                  5//at dusk with my screen turned on r, g and b are 1 and par <= 5 µmol/(m²*s), i.e. it is night
#define BLE_BUFFER_SIZES             500 //Number of bytes needed, i.e. number of characters. Note: Too small will lead to buffer overrun and freeze Arduino
#define BAUD_RATE                    9600
#define BLE_DEVICE_NAME              "Arduino Nano 33 BLE Sense"
#define BLE_LOCAL_NAME               "GreenhouseSensor"
#define AIR_TEMP_THRESH              30.0 //warning threshold for too hot temperatures [°C]
#define S_PER_DAY                    86400 //a day has 86400 seconds 
#define MIN_DLI                      20 //DLI recommended for lettuce: 10-25 mol/(m²*day), flowering crops: 25-35 mol/(m²*day)
#define TIME_INTERVAL                1000 // 1000 milliseconds


extern uint8_t isDark;
extern int writeLength;
extern float airTemp;
extern float humidity;  
//red, green, blue, whilte color from sensor
extern int r; 
extern int g; 
extern int b; 
extern int c; 
extern double par; //PAR or actually photosynthetic photon flux density [µmol/(m²*s)]
extern double dli; //accrued PPFD [mol/(m²*d)]
extern unsigned long secondsToday; //elapsed seconds today
extern unsigned long timeOffset; //time offset in seconds from midnight

//Common global variables 
extern char bleBuffer[BLE_BUFFER_SIZES];
extern char serialBuffer[BLE_BUFFER_SIZES]; //buffer for serial data