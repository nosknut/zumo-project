#include "Timer.h"
#include "IrSocket/IrSocket.h"
#include <ArduinoJson.h>

Timer timer;
IrSocket irSocket(3, 11);

int a = 0;

bool sender = false;

void setup()
{
    Serial.begin(115200);

    irSocket.begin();
}

void loop()
{
    if (irSocket.available())
    {
        char value = irSocket.read();
        Serial.print(value);

        // DynamicJsonDocument doc(200);
        // DeserializationError error = deserializeJson(doc, irSocket);
        //
        // if (error)
        // {
        //     Serial.print(F("deserializeJson() failed: "));
        //     Serial.println(error.c_str());
        //     return;
        // }
        //
        // String command = doc["command"];
        // int chargeLevel = doc["chargeLevel"];
        // a = chargeLevel;
        // Serial.println("Received command: " + command + " with charge level: " + chargeLevel);
    }

    if (Serial.available())
    {
        char value = Serial.read();
        irSocket.print(value);
    }

    if (sender && timer.isFinished(10000) && irSocket.doneReading())
    {
        a += 1;
        irSocket.print("{\"command\":\"start-charging\",\"carId\":\"1\",\"allowDebt\":true,\"chargeLevel\":" + String(a) + ",\"targetChargeLevel\":80}");
        timer.reset();
    }
}
