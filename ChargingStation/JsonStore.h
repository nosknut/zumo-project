#ifndef JSONSTORE_H
#define JSONSTORE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

// https://github.com/bblanchon/ArduinoStreamUtils
// https://arduinojson.org/v6/how-to/store-a-json-document-in-eeprom/
struct JsonStore
{
    int address;
    int size;

    DynamicJsonDocument doc;

    JsonStore(int address, int size) : address(address), size(size), doc(size)
    {
    }

    void setup()
    {
        // Read from EEPROM
        EepromStream stream(address, size);
        DeserializationError error = deserializeJson(doc, stream);

        if (error)
        {
            Serial.println("Failed to read from EEPROM");
            Serial.println(error.c_str());

            doc.as<JsonObject>();
        }

        Serial.println("Read from EEPROM:");
        serializeJson(doc, Serial);
        Serial.println();
    }

    void writeToEeprom()
    {
        EepromStream stream(address, size);
        serializeJson(doc, stream);
    }
};

#endif
