#define DEBUG_CHARGER_LINK
#include "ChargerLink/ChargerLink.h"
#include <Zumo32U4Buttons.h>

int carId = 4;

Zumo32U4ButtonA buttonA;
ZumoIrSocket irSocket(22, carId, LEFT_IR);
ChargerLink chargerLink(irSocket);

void updateChargerLink()
{
    if (chargerLink.read())
    {
        if (chargerLink.signal == ChargerLinkSignal::LINK_AVAILABLE)
        {
            Serial.println("Detected charger link!");
            chargerLink.requestBalance(carId);
        }

        if (chargerLink.signal == ChargerLinkSignal::BALANCE)
        {
            if (chargerLink.balance >= 100)
            {
                chargerLink.startCharging(carId, true, 0, 100);
            }
        }

        if (chargerLink.signal == ChargerLinkSignal::CHARGE_STATE)
        {
            if (chargerLink.chargerState.charging)
            {
                if (chargerLink.chargerState.chargeLevel >= 50)
                {
                    chargerLink.stopCharging(carId);
                }
            }
            else
            {
                Serial.println("Done charging!");
            }
        }
    }
}

void updateButton()
{
    if (buttonA.getSingleDebouncedPress())
    {
        Serial.println("Added 20 to balance!");
        chargerLink.addEarnings(20);
    }
}

void setup()
{
    Serial.begin(115200);

    irSocket.begin();
}

void loop()
{
    updateButton();
    updateChargerLink();
}
