#include <Wire.h>
#include <RTClib.h>
#include <Streaming.h>

extern DateTime now;

// print date and time to Serial
extern void printDateTime();

// print time to Serial
extern void printTime();

// print date to Serial
extern void printDate();

// Add leading zeros
extern void printI00(int val, char delim);