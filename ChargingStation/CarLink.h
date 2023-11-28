#ifndef CAR_LINK_h
#define CAR_LINK_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ChargeState.h"
#include "IrSocket.h"
#include "Timer.h"

struct CarLink
{
    Timer timer;
    IrSocket &stream;

    DynamicJsonDocument incomingMessageDoc = DynamicJsonDocument(100);

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

        Serial.println("Sent message to car:");
        serializeJson(doc, Serial);
        Serial.println();
    }

    int available()
    {
        return stream.available();
    }

    bool read()
    {
        DeserializationError error = deserializeMsgPack(incomingMessageDoc, stream);

        while (!stream.doneReading())
            stream.read();

        if (error)
        {
            Serial.print("deserializeMsgPack() failed: ");
            Serial.println(error.c_str());
            return false;
        }

        Serial.println("Received message from car:");
        serializeJson(incomingMessageDoc, Serial);
        Serial.println();

        return true;
    }

    DynamicJsonDocument &getMessage()
    {
        return incomingMessageDoc;
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
