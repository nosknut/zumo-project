#ifndef CAR_DATABASE_h
#define CAR_DATABASE_h

#include "JsonStore.h"

String ACCOUNT_BALANCE_KEY = "a";

struct CarDatabase
{
    void createCar(int carId, JsonStore &jsonStore)
    {
        if (jsonStore.doc["cars"][String(carId)].isNull())
        {
            jsonStore.doc["cars"][String(carId)][ACCOUNT_BALANCE_KEY] = 100;
        }
    }

    void setCarField(int carId, int value, String field, JsonStore &jsonStore)
    {
        createCar(carId, jsonStore);
        jsonStore.doc["cars"][String(carId)][field] = value;
        jsonStore.writeToEeprom();
    }

    int getCarField(int carId, String field, JsonStore &jsonStore)
    {
        createCar(carId, jsonStore);
        return jsonStore.doc["cars"][String(carId)][field];
    }

    void setAccountBalance(int carId, int balance, JsonStore &jsonStore)
    {
        setCarField(carId, balance, ACCOUNT_BALANCE_KEY, jsonStore);
    }

    int getAccountBalance(int carId, JsonStore &jsonStore)
    {
        return getCarField(carId, ACCOUNT_BALANCE_KEY, jsonStore);
    }
};

#endif
