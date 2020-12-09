/*
RootTooth.h - Library for ESP32 BLE Comms
*/

#ifndef RootTooth_h
#define RootTooth_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
using namespace std;

#define SERVICE_UID_SENSORS "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UID_SENSORS "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DELAY_TIME 500

class RootTooth
{
public:
    RootTooth(char *name);
    void start();
    void publishData(char *buffer);
    char *deviceName;

private:
    BLEServer *rootServer;
    BLECharacteristic *rootSensorsChar;
    BLEService *createReadService(byte type, char *serviceUUID, char *characterisitcUUID);
};

#endif