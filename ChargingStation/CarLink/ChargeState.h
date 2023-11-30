#ifndef CHARGE_STATE_h
#define CHARGE_STATE_h

struct ChargeState
{
    unsigned long timer = 0;
    unsigned long chargeStateUpdatePeriod = 2000;

    int carId = 0;
    int chargeLevel = 0;
    int accountBalance = 0;
    bool allowDebt = false;
    int targetChargeLevel = 0;

    bool charging = false;

    void resetTimer()
    {
        timer = millis();
    }

    void start(
        int newCarId,
        int newBalance,
        int newLevel,
        int newTargetChargeLevel,
        bool newAllowDebt)
    {
        resetTimer();
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

        if ((millis() - timer) < chargeStateUpdatePeriod)
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

        resetTimer();

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
