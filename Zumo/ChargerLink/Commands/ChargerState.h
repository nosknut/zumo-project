#ifndef CHARGER_STATE_h
#define CHARGER_STATE_h

struct ChargerState
{
    int carId = 0;

    int chargeLevel = 0;
    bool charging = false;
    int accountBalance = 0;
    int targetChargeLevel = 0;

    bool isFull()
    {
        return chargeLevel >= targetChargeLevel;
    }

    bool isBroke()
    {
        return accountBalance < 1;
    }
};

#endif
