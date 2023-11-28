#ifndef CHARGER_LINK_h
#define CHARGER_LINK_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ChargerState.h"
#include "ZumoIrSocket.h"
#include "ChargerLinkSignal.h"

struct ChargerLink
{
    ZumoIrSocket &stream;

    ChargerLinkSignal signal = ChargerLinkSignal::NONE;
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

#ifdef DEBUG_CHARGER_LINK
        Serial.println("Sent message to charger:");
        serializeJson(doc, Serial);
        Serial.println();
#endif
    }

    void stopCharging(int carId)
    {
        DynamicJsonDocument doc(20);

        doc["a"] = 0;

        serializeMsgPack(doc, stream);

#ifdef DEBUG_CHARGER_LINK
        Serial.println("Sent message to charger:");
        serializeJson(doc, Serial);
        Serial.println();
#endif
    }

    bool read()
    {
        if (!stream.available())
        {
            signal = ChargerLinkSignal::NONE;
            return false;
        }

        if (stream.peek() == '@')
        {
            stream.read();
            signal = ChargerLinkSignal::LINK_AVAILABLE;
            return true;
        }

        DynamicJsonDocument doc(100);
        DeserializationError error = deserializeMsgPack(doc, stream);

        while (!stream.doneReading())
            stream.read();

        if (error)
        {
#ifdef DEBUG_CHARGER_LINK
            Serial.print("deserializeMsgPack() failed: ");
            Serial.println(error.c_str());
#endif

            signal = ChargerLinkSignal::NONE;
            return false;
        }

#ifdef DEBUG_CHARGER_LINK
        Serial.println("Received message from charger:");
        serializeJson(doc, Serial);
        Serial.println();
#endif

        int command = doc["a"];

        if (command != 2)
        {
#ifdef DEBUG_CHARGER_LINK
            Serial.println("Received invalid command from charger: " + command);
#endif
            signal = ChargerLinkSignal::NONE;
            return false;
        }

        chargerState.carId = doc["b"];
        chargerState.charging = doc["c"];
        chargerState.chargeLevel = doc["d"];
        chargerState.accountBalance = doc["e"];
        chargerState.targetChargeLevel = doc["f"];

        signal = ChargerLinkSignal::CHARGE_STATE;
        return true;
    }
};

#endif
