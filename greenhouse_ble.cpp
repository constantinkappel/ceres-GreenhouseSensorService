#include "greenhouse_ble.h"

// UUIDs of services
extern const char* ServiceName = "greenhouseSensorService";
extern const char* uuidOfTempService = "2c9d0585-09f0-4e8f-9bf3-ddc13f9cd36d";
extern const char* uuidOfTempChar = "0a459982-c317-4bad-b234-b6c2ff69f055";
extern const char* uuidOfTempWarnChar = "ef0d5dfd-f466-435e-8bd9-b069f8e3a5db";
extern const char* uuidOfNotifyDLIChar = "137baafd-8dc6-46b5-985b-0bd88ee83f82"; // extends the daily light intake to the light switch as client
extern const char* uuidOfCumDLIChar = "befc0ce1-a854-46e4-a4b3-77db0d208998"; // Reports cumulative DLI accrued within a day
extern const char* uuidOfHumidityChar = "2516cd1f-feee-457d-bc5b-3b6c7d7c37a3"; //current humidity
extern const char* uuidOfParChar = "cb2693fc-83fd-4f81-8ac3-4ab23e6d4797"; // PAR 

//BLE Services
BLEService sensorService(uuidOfTempService);
BLECharacteristic airTempChar(uuidOfTempChar, BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic airTempWarning(uuidOfTempWarnChar, BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES); //Notifies client of exceeding airTemp Threshold
BLECharacteristic notifyDliChar(uuidOfNotifyDLIChar, BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES); // sends notify when light threshold reached and it is dark
BLECharacteristic cumDliChar(uuidOfCumDLIChar, BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES); //reports cumulative DLI accrued in a day
BLECharacteristic humidityChar(uuidOfHumidityChar, BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES); 
BLECharacteristic parChar(uuidOfParChar, BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

bool setupBLE() {

    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }

    // Set the local name
    BLE.setLocalName(bleServerName);
    BLE.setDeviceName(bleServerName);
    BLE.setAdvertisedService(sensorService);
    
    // Add the characteristic to the service
    sensorService.addCharacteristic(airTempChar);
    sensorService.addCharacteristic(airTempWarning);
    sensorService.addCharacteristic(notifyDliChar);
    sensorService.addCharacteristic(cumDliChar);
    sensorService.addCharacteristic(humidityChar);
    sensorService.addCharacteristic(parChar);
    
    // Add the service
    BLE.addService(sensorService);
    
    // Start advertising
    BLE.advertise();
    
    return true;
}

bool writeBLE(void *) {
    DPRINT("Writing to BLE...");
    snprintf(bleBuffer, BLE_BUFFER_SIZES, "%.1f", airTemp);
    airTempChar.writeValue(bleBuffer);
    memset(bleBuffer, 0, BLE_BUFFER_SIZES);//bleBuffer[0] = '\0'; // set to empty string

    snprintf(bleBuffer, BLE_BUFFER_SIZES, "%.1f", humidity);
    humidityChar.writeValue(bleBuffer);
    memset(bleBuffer, 0, BLE_BUFFER_SIZES);//bleBuffer[0] = '\0'; // set to empty string

    snprintf(bleBuffer, BLE_BUFFER_SIZES, "%.1f", par);
    parChar.writeValue(bleBuffer);
    memset(bleBuffer, 0, BLE_BUFFER_SIZES);//bleBuffer[0] = '\0'; // set to empty string

    snprintf(bleBuffer, BLE_BUFFER_SIZES, "%.3f", dli);
    cumDliChar.writeValue(bleBuffer);
    memset(bleBuffer, 0, BLE_BUFFER_SIZES);//bleBuffer[0] = '\0'; // set to empty string
    //sprintf(bleBuffer, "%d", isDark);
    //notifyDliChar.writeValue(bleBuffer);
    //sprintf(bleBuffer, "%d", isDark);
    //airTempWarning.writeValue(bleBuffer);
    DPRINTLN(" done.");
    return true;
}