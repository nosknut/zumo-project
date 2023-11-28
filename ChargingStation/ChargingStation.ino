#include "Timer.h"
#include "CarLink.h"
#include "IrSocket.h"
#include "JsonStore.h"
#include "CarDatabase.h"
#include "ChargeState.h"

int chargerId = 0;

Timer chargeTimer;
JsonStore jsonStore;
ChargeState chargeState;
CarDatabase carDatabase;
IrSocket irSocket(3, 11);
CarLink carLink(irSocket);

void stopCharging()
{
    carDatabase.setAccountBalance(
        chargeState.carId,
        chargeState.accountBalance,
        jsonStore);

    chargeState.stop();
    carLink.sendChargeReport(chargeState);
    chargeState.reset();
}

void updateCharging()
{
    if (chargeState.update())
    {
        carLink.sendChargeReport(chargeState);
    }

    if (chargeState.charging)
    {
        if (chargeState.isFull())
        {
            stopCharging();
        }

        if (!chargeState.allowDebt && chargeState.isBroke())
        {
            stopCharging();
        }
    }
}

void updateCarLink()
{
    if (carLink.available())
    {
        if (carLink.read())
        {
            DynamicJsonDocument &doc = carLink.getMessage();

            if (doc.containsKey("a"))
            {
                int command = doc["a"];

                if (command == 1)
                {
                    String carId = doc["b"];
                    bool allowDebt = doc["c"];
                    int chargeLevel = doc["d"];
                    int targetChargeLevel = doc["e"];
                    int accountBalance = carDatabase.getAccountBalance(carId, jsonStore);
                    chargeState.start(carId, accountBalance, chargeLevel, targetChargeLevel, allowDebt);
                    carLink.sendChargeReport(chargeState);
                }

                if (command == 0)
                {
                    stopCharging();
                }
            }
        }
    }

    carLink.update(chargeState);
}

void setup()
{
    Serial.begin(115200);

    irSocket.begin();

    jsonStore.address = 0;
    jsonStore.size = 100;
}

void loop()
{
    updateCarLink();
    updateCharging();
}
