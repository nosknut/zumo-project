#include "Timer.h"
#include "CarLink.h"
#include "CarDatabase.h"

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
    if (carLink.read())
    {
        if (carLink.signal == CarLinkSignal::START_CHARGING)
        {
            int accountBalance = carDatabase.getAccountBalance(carLink.startCommand.carId, jsonStore);

            chargeState.start(
                carLink.startCommand.carId,
                accountBalance,
                carLink.startCommand.chargeLevel,
                carLink.startCommand.targetChargeLevel,
                carLink.startCommand.allowDebt);

            carLink.sendChargeReport(chargeState);
        }

        if (carLink.signal == CarLinkSignal::STOP_CHARGING)
        {
            stopCharging();
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
