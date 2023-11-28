#ifndef JSONSTORE_H
#define JSONSTORE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

// https://github.com/bblanchon/ArduinoStreamUtils
// https://arduinojson.org/v6/how-to/store-a-json-document-in-eeprom/
struct JsonStore
{
    int size = 0;
    int address = 0;

    DynamicJsonDocument read()
    {
        DynamicJsonDocument doc(size);
        EepromStream eepromStream(address, size);
        deserializeJson(doc, eepromStream);
        return doc;
    }

    void write(DynamicJsonDocument doc)
    {
        EepromStream eepromStream(address, size);
        serializeJson(doc, eepromStream);
    }
};

#endif
