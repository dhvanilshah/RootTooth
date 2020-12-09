/*
RootTooth.cpp – Library for ESP32 BLE Comms
*/
#include "RootTooth.h"

bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

RootTooth::RootTooth(char *name)
{
    deviceName = name;
}

void RootTooth::start()
{
    BLEDevice::init(deviceName);
    rootServer = BLEDevice::createServer();
    rootServer->setCallbacks(new MyServerCallbacks());
    BLEService *sensorService = createReadService(
        0,
        SERVICE_UID_SENSORS,
        CHAR_UID_SENSORS);

    sensorService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UID_SENSORS);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

BLEService *RootTooth::createReadService(
    byte type,
    char *serviceUUID,
    char *characterisitcUUID)
{
    BLEService *service = rootServer->createService(serviceUUID);
    rootSensorsChar = service->createCharacteristic(
        characterisitcUUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);
    rootSensorsChar->addDescriptor(new BLE2902());
    return service;
}

void RootTooth::publishData(char *buffer)
{
    rootSensorsChar->setValue(buffer);
    // notify changed value
    if (deviceConnected)
    {
        rootSensorsChar->notify();
        delay(DELAY_TIME);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                     // give the bluetooth stack the chance to get things ready
        rootServer->startAdvertising(); // restart advertising
        Serial.println("Started advertising device again...");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}