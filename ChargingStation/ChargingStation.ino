#define DEBUG_CAR_LINK
#include "CarLink/CarLink.h"
#include "Display.h"
#include "Database/CarDatabase.h"

int chargerId = 0;

JsonStore jsonStore(0, 200);
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
        if (carLink.signal == CarLinkSignal::REQUEST_BALANCE)
        {
            int carId = carLink.requestBalanceCommand.carId;
            int accountBalance = carDatabase.getAccountBalance(carId, jsonStore);
            accountBalance += carLink.requestBalanceCommand.earnings;
            carDatabase.setAccountBalance(carId, accountBalance, jsonStore);
            carLink.sendBalance(carId, accountBalance);
            chargeState.resetTimer();
        }

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
            chargeState.resetTimer();
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

    display.setup();
    display.update(chargeState);

    jsonStore.setup();
}

void loop()
{
    updateCarLink();
    updateCharging();
}
