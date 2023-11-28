#ifndef CHARGE_STATE_h
#define CHARGE_STATE_h

#include "Timer.h"

struct ChargeState
{
    Timer timer;

    int carId = 0;
    int chargeLevel = 0;
    int accountBalance = 0;
    bool allowDebt = false;
    int targetChargeLevel = 0;

    bool charging = false;

    void start(
        int newCarId,
        int newBalance,
        int newLevel,
        int newTargetChargeLevel,
        bool newAllowDebt)
    {
        timer.reset();
        charging = true;
        carId = newCarId;
        chargeLevel = newLevel;
        allowDebt = newAllowDebt;
        accountBalance = newBalance;
        targetChargeLevel = newTargetChargeLevel;
    }

    /**
     * @return true if the charge state was updated, false otherwise
     */
    bool update()
    {
        if (!charging)
        {
            return false;
        }

        if (!timer.isFinished(6000))
        {
            return false;
        }

        if (isBroke() && !allowDebt)
        {
            return false;
        }

        if (chargeLevel < targetChargeLevel)
        {
            chargeLevel += 10;
            accountBalance -= 1;
            chargeLevel = min(chargeLevel, targetChargeLevel);
        }

        timer.reset();

        return true;
    }

    bool isFull()
    {
        return chargeLevel >= targetChargeLevel;
    }

    bool isBroke()
    {
        return accountBalance < 1;
    }

    void stop()
    {
        charging = false;
    }

    void reset()
    {
        carId = 0;
        chargeLevel = 0;
        accountBalance = 0;
        targetChargeLevel = 0;
    }
};

#endif
