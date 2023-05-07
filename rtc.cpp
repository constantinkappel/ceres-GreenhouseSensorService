#include "rtc.h"



// print date and time to Serial
void printDateTime()
{
    printDate();
    Serial << ' ';
    printTime();
}

// print time to Serial
void printTime()
{
    printI00(now.hour(), ':');
    printI00(now.minute(), ':');
    printI00(now.second(), 0);
}

// print date to Serial
void printDate()
{
    printI00(now.year(), '/');
    printI00(now.month(), '/');
    printI00(now.day(), 0);
}

// Print an integer in "00" format (with leading zero),
// followed by a delimiter character to Serial.
// Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}