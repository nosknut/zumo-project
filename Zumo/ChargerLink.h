#ifndef CAR_LINK_h
#define CAR_LINK_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ChargerState.h"
#include "ZumoIrSocket.h"

enum class ChargerLinkSignal
{
    NONE = 0,
    COMMAND = 1,
    LINK_AVAILABLE = 2,
};

struct ChargerLink
{
    ZumoIrSocket &stream;

    ChargerState chargerState;

    ChargerLink(ZumoIrSocket &stream) : stream(stream)
    {
    }

    void startCharging(int carId, bool allowDebt, int chargeLevel, int targetChargeLevel)
    {
        DynamicJsonDocument doc(50);

        doc["a"] = 1;
        doc["b"] = carId;
        doc["c"] = int(allowDebt);
        doc["d"] = chargeLevel;
        doc["e"] = targetChargeLevel;

        serializeMsgPack(doc, stream);

        Serial.println("Sent message to charger:");
        serializeJson(doc, Serial);
        Serial.println();
    }

    void stopCharging(int carId)
    {
        DynamicJsonDocument doc(20);

        doc["a"] = 0;

        serializeMsgPack(doc, stream);

        Serial.println("Sent message to charger:");
        serializeJson(doc, Serial);
        Serial.println();
    }

    int available()
    {
        return stream.available();
    }

    ChargerLinkSignal read()
    {
        if (stream.peek() == '@')
        {
            stream.read();
            return ChargerLinkSignal::LINK_AVAILABLE;
        }

        DynamicJsonDocument doc(100);
        DeserializationError error = deserializeMsgPack(doc, stream);

        while (!stream.doneReading())
            stream.read();

        if (error)
        {
            Serial.print("deserializeMsgPack() failed: ");
            Serial.println(error.c_str());
            return ChargerLinkSignal::NONE;
        }

        Serial.println("Received message from charger:");
        serializeJson(doc, Serial);
        Serial.println();

        int command = doc["a"];

        if (command != 2)
        {
            Serial.println("Received invalid command from car: " + command);
            return ChargerLinkSignal::NONE;
        }

        chargerState.carId = doc["b"];
        chargerState.charging = doc["c"];
        chargerState.chargeLevel = doc["d"];
        chargerState.accountBalance = doc["e"];
        chargerState.targetChargeLevel = doc["f"];

        return ChargerLinkSignal::COMMAND;
    }

    ChargerState getChargerState()
    {
        return chargerState;
    }
};

#endif
