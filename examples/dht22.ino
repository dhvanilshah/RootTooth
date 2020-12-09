#include <ArduinoJson.h>
#include <RootTooth.h>
#include <DHT.h>

#define NUM_SENSORS 2
#define DHTTYPE DHT22

uint8_t DHTPin = 15;
DHT dht(DHTPin, DHTTYPE);

RootTooth rootTooth("RootToothTheFirst");

int i = 0;

void setup()
{
    Serial.begin(9600);

    pinMode(DHTPin, INPUT);
    dht.begin();

    rootTooth.start();
}

void loop()
{
    // read sesnor value:
    float humidity = dht.readHumidity();
    float airTemp = dht.convertCtoF(dht.readTemperature());

    if (!isnan(airTemp) && !isnan(humidity))
    {
        //  Create a JSON document with sensor data
        const size_t capacity = JSON_OBJECT_SIZE(NUM_SENSORS);
        StaticJsonDocument<capacity> doc;

        // Put readings into the doc
        doc["at"] = airTemp;
        doc["rh"] = humidity;

        char json_string[256];
        serializeJson(doc, json_string);

        // Publish the data via bluetooth
        rootTooth.publishData(json_string);
    }

    delay(10000);
}