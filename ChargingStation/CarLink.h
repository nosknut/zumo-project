#ifndef CAR_LINK_h
#define CAR_LINK_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ChargeState.h"
#include "IrSocket.h"

struct CarLink
{
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
        
        while (!stream.doneReading());
        
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
};

#endif
