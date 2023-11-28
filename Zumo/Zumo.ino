#define DEBUG_CHARGER_LINK
#include "ChargerLink.h"

int carId = 1;

ZumoIrSocket irSocket(22, carId, LEFT_IR);
ChargerLink chargerLink(irSocket);

void updateChargerLink()
{
    if (chargerLink.read())
    {
        if (chargerLink.signal == ChargerLinkSignal::LINK_AVAILABLE)
        {
            Serial.println("Detected charger link!");
            chargerLink.startCharging(carId, true, 0, 100);
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
        }
    }
}

void setup()
{
    Serial.begin(115200);

    irSocket.begin();
}

void loop()
{
    updateChargerLink();
}
