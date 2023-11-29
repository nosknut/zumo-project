#define DEBUG_CAR_LINK
#include "Timer.h"
#include "CarLink.h"
#include "Display.h"
#include "CarDatabase.h"

int chargerId = 0;

Timer chargeTimer;
JsonStore jsonStore;
ChargeState chargeState;
CarDatabase carDatabase;
IrSocket irSocket(3, 5);
CarLink carLink(irSocket);
Display display;

void stopCharging()
{
    carDatabase.setAccountBalance(
        chargeState.carId,
        chargeState.accountBalance,
        jsonStore);

    chargeState.stop();
    display.update(chargeState);
    carLink.sendChargeReport(chargeState);
    chargeState.reset();
}

void updateCharging()
{
    if (chargeState.update())
    {
        display.update(chargeState);
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

            display.update(chargeState);
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

    display.setup();
    display.update(chargeState);
}

void loop()
{
    updateCarLink();
    updateCharging();
}
