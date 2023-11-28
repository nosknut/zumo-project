#ifndef CAR_DATABASE_h
#define CAR_DATABASE_h

#include "JsonStore.h"

struct CarDatabase
{
    void createCar(int carId, DynamicJsonDocument doc) {
        if (!doc.containsKey("cars")) {
            doc["cars"] = JsonObject();
        }

        if (!doc["cars"].containsKey(String(carId))) {
            doc["cars"][carId] = JsonObject();
            doc["cars"][carId]["accountBalance"] = 100;
        }
    }

    void setCarField(int carId, int value, String field, JsonStore jsonStore)
    {
        DynamicJsonDocument doc = jsonStore.read();
        createCar(carId, doc);
        doc["cars"][carId][field] = value;
        jsonStore.write(doc);
    }

    int getCarField(int carId, String field, JsonStore jsonStore)
    {
        DynamicJsonDocument doc = jsonStore.read();
        createCar(carId, doc);
        return jsonStore.read()["cars"][carId][field];
    }

    void setAccountBalance(int carId, int balance, JsonStore jsonStore)
    {
        setCarField(carId, balance, "accountBalance", jsonStore);
    }

    int getAccountBalance(int carId, JsonStore jsonStore)
    {
        return getCarField(carId, "accountBalance", jsonStore);
    }
};

#endif
