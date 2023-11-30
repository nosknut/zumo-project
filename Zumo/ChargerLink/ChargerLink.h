#ifndef CHARGER_LINK_h
#define CHARGER_LINK_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Commands/ChargerState.h"
#include "IrSocket/ZumoIrSocket.h"
#include "LinkCommands.h"
#include "ChargerLinkSignal.h"

struct ChargerLink
{
    int earnings = 0;
    int balance = 0;

    ZumoIrSocket &stream;

    ChargerLinkSignal signal = ChargerLinkSignal::NONE;
    ChargerState chargerState;

    ChargerLink(ZumoIrSocket &stream) : stream(stream)
    {
    }

    void addEarnings(int amount)
    {
        earnings += amount;
    }

    void sendMessage(DynamicJsonDocument &doc)
    {
        serializeMsgPack(doc, stream);

#ifdef DEBUG_CHARGER_LINK
        Serial.print("ChargerLink(out): ");
        serializeJson(doc, Serial);
        Serial.println();
#endif
    }

    void requestBalance(int carId)
    {
        DynamicJsonDocument doc(30);

        doc["a"] = LinkCommands::REQUEST_BALANCE;
        doc["b"] = carId;
        doc["c"] = earnings;

        sendMessage(doc);

        earnings = 0;
    }

    void startCharging(int carId, bool allowDebt, int chargeLevel, int targetChargeLevel)
    {
        DynamicJsonDocument doc(50);

        doc["a"] = LinkCommands::START_CHARGING;
        doc["b"] = carId;
        doc["c"] = int(allowDebt);
        doc["d"] = chargeLevel;
        doc["e"] = targetChargeLevel;

        sendMessage(doc);
    }

    void stopCharging(int carId)
    {
        DynamicJsonDocument doc(20);
        doc["a"] = LinkCommands::STOP_CHARGING;
        sendMessage(doc);
    }

    bool read()
    {
        if (!stream.available())
        {
            signal = ChargerLinkSignal::NONE;
            return false;
        }

        if (stream.peek() == LinkCommands::LINK_AVAILABLE)
        {
            stream.read();
            signal = ChargerLinkSignal::LINK_AVAILABLE;

            // Wait for all remaining signals and residuals to be received
            while (!stream.doneReading())
                stream.read();

            return true;
        }

        DynamicJsonDocument doc(100);
        DeserializationError error = deserializeMsgPack(doc, stream);

        // Wait for all remaining signals and residuals to be received
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
        Serial.print("ChargerLink(in):  ");
        serializeJson(doc, Serial);
        Serial.println();
#endif

        int command = doc["a"];

        if (command == LinkCommands::CHARGE_STATE)
        {
            chargerState.carId = doc["b"];
            chargerState.charging = doc["c"];
            chargerState.chargeLevel = doc["d"];
            chargerState.accountBalance = doc["e"];
            chargerState.targetChargeLevel = doc["f"];

            signal = ChargerLinkSignal::CHARGE_STATE;
            return true;
        }

        if (command == LinkCommands::BALANCE)
        {
            balance = doc["c"];

            signal = ChargerLinkSignal::BALANCE;
            return true;
        }

#ifdef DEBUG_CHARGER_LINK
        Serial.println("Received invalid command from charger: " + command);
#endif
        signal = ChargerLinkSignal::NONE;
        return false;
    }
};

#endif
