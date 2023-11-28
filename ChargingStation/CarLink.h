#ifndef CAR_LINK_h
#define CAR_LINK_h

#include <Arduino.h>
#include "CarLinkSignal.h"
#include "StartCommand.h"
#include <ArduinoJson.h>
#include "ChargeState.h"
#include "IrSocket.h"
#include "Timer.h"

struct CarLink
{
    Timer timer;
    IrSocket &stream;

    CarLinkSignal signal = CarLinkSignal::NONE;
    StartCommand startCommand;

    CarLink(IrSocket &stream) : stream(stream)
    {
    }

    void sendChargeReport(ChargeState chargeState)
    {
        DynamicJsonDocument doc(100);

        doc["a"] = 2;
        doc["b"] = chargeState.carId;
        doc["c"] = int(chargeState.charging);
        doc["d"] = chargeState.chargeLevel;
        doc["e"] = chargeState.accountBalance;
        doc["f"] = chargeState.targetChargeLevel;

        serializeMsgPack(doc, stream);

#ifdef DEBUG_CAR_LINK
        Serial.println("Sent message to car:");
        serializeJson(doc, Serial);
        Serial.println();
#endif
    }

    bool read()
    {
        if (!stream.available())
        {
            signal = CarLinkSignal::NONE;
            return false;
        }

        DynamicJsonDocument doc(100);
        DeserializationError error = deserializeMsgPack(doc, stream);

        while (!stream.doneReading())
            stream.read();

        if (error)
        {
#ifdef DEBUG_CAR_LINK
            Serial.print("deserializeMsgPack() failed: ");
            Serial.println(error.c_str());
#endif

            signal = CarLinkSignal::NONE;
            return false;
        }

#ifdef DEBUG_CAR_LINK
        Serial.println("Received message from car:");
        serializeJson(doc, Serial);
        Serial.println();
#endif

        if (doc.containsKey("a"))
        {
            int command = doc["a"];

            if (command == 0)
            {
                signal = CarLinkSignal::STOP_CHARGING;
                return true;
            }

            if (command == 1)
            {
                startCommand.carId = doc["b"];
                startCommand.allowDebt = doc["c"];
                startCommand.chargeLevel = doc["d"];
                startCommand.targetChargeLevel = doc["e"];

                signal = CarLinkSignal::START_CHARGING;
                return true;
            }
        }

        signal = CarLinkSignal::NONE;
        return false;
    }

    // Periodically emit a signal so the car can detect the charger
    void update(ChargeState &chargeState)
    {
        if (chargeState.charging)
            return;

        if (!stream.doneReading())
            return;

        if (!timer.isFinished(1000))
            return;

        stream.print('@');
        timer.reset();
    }
};

#endif
