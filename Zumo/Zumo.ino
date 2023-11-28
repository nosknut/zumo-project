#include "Timer.h"
#include "ZumoIrSocket.h"
#include "ChargerLink.h"
#include "ChargerState.h"

Timer timer;
int carId = 1;

ZumoIrSocket irSocket(22, carId, LEFT_IR);
ChargerLink chargerLink(irSocket);

void updateChargerLink()
{
    if (chargerLink.available())
    {
        if (chargerLink.read())
        {
            ChargerState chargerState = chargerLink.getChargerState();

            if (chargerState.charging)
            {
                if (chargerState.chargeLevel >= 50)
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

    chargerLink.startCharging(carId, true, 0, 100);
}

void loop()
{
    updateChargerLink();
}