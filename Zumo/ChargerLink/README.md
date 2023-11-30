# ChargerLink

## Setup

```cpp
#define DEBUG_CHARGER_LINK
#include "ChargerLink/ChargerLink.h"

int carId = 0;

ZumoIrSocket irSocket(22, carId, LEFT_IR);
ChargerLink chargerLink(irSocket);


void setup()
{
    Serial.begin(115200);
    irSocket.begin();
}
```

The ZumoIrSocket is the actual communucation line to the charger, and behaves like the `Serial` port. The ChargerLink is a wrapper around the ZumoIrSocket that provides a simple interface to the chargers features.

Remove the `#define DEBUG_CHARGER_LINK` line to disable debug messages from the ChargerLink.

## Receiving messages from the charger

```cpp
void updateChargerLink()
{
    if (chargerLink.read())
    {
        if (chargerLink.signal == ChargerLinkSignal::LINK_AVAILABLE)
        {
            Serial.println("Detected charger link!");
        }

        if (chargerLink.signal == ChargerLinkSignal::BALANCE)
        {
            Serial.println(chargerLink.balance);
        }
    }
}

void loop()
{
    updateChargerLink();
}
```

The `chargerLink.read()` method will return `true` if a message was received from the charger. The `chargerLink.signal` variable will contain the type of message received. Some messages, like `ChargerLinkSignal::BALANCE` will also contain additional data. This data will be available through the appropriate variable, in this case `chargerLink.balance`.

## Sending messages to the charger

```cpp
void loop()
{
    updateChargerLink();

    if (buttonPressed)
    {
        chargerLink.requestBalance();
    }
}
```

The `chargerLink.requestBalance()` method will send a message to the charger requesting the current balance. The charger will respond with a `ChargerLinkSignal::BALANCE` message, which will be received and handled in the `updateChargerLink()` function.

## Transfer time

Due to the slow IR protocol used, it can take up to several seconds to transmit or receive a message. This means when there are incoming messages, the `chargerLink.read()` method can block the program for several seconds. If the method does not run often enough, some messages may be lost, and the transmission will fail. Similarly, sending messages with methods such as `chargerLink.requestBalance()` can also block the program for several seconds.

Take this into account when using the library.

## Dependencies

The library depends on other libraries. These can be found in the [dependencies.txt](dependencies.txt) file.

## Methods

### `chargerLink.read()`
```cpp
chargerLink.read()
```

Reads a message from the charger. Returns `true` if a message was received, `false` otherwise. If a message is incoming `chargerLink.read()` will block the program until the message is fully received.

---

### `chargerLink.requestBalance()`

```cpp
int carId = 0;
chargerLink.requestBalance(carId);
```

Requests the current bank account balance from the charger. The charger will respond with a `ChargerLinkSignal::BALANCE` message. The data will be available in the `chargerLink.balance` variable.

---

### `chargerLink.startCharging()`

```cpp
int carId = 0;
bool allowDebt = true;
int currentChargeState = 0;
int targetChargeState = 100;
chargerLink.startCharging(carId, allowDebt, currentChargeState, targetChargeState);
```

Requests the charger to start charging. The charger will respond with a `ChargerLinkSignal::CHARGE_STATE` message. The data will be available in the `chargerLink.chargeState` variable.

---

### `chargerLink.stopCharging()`

```cpp
int carId = 0;
chargerLink.stopCharging(carId);
```

Requests the charger to stop charging. The charger will respond with a `ChargerLinkSignal::CHARGE_STATE` message. The data will be available in the `chargerLink.chargeState` variable. If the charging stopped, the `chargerLink.chargeState.charging` variable will be `false`.

---

### `chargerLink.addEarnings()`

```cpp
chargerLink.addEarnings(20);
```

Adds earnings to the chargerLink. These will be transmitted to the charger when the `chargerLink.requestBalance()` method is called.

---

## Variables

### `chargerLink.signal`

Contains the last received message from the charger. If the message was somehow broken the value will be `ChargerLinkSignal::NONE`. The variable can have one of the following values:

- `ChargerLinkSignal::NONE`
- `ChargerLinkSignal::LINK_AVAILABLE`
- `ChargerLinkSignal::BALANCE`
- `ChargerLinkSignal::CHARGE_STATE`

### `chargerLink.balance`

Contains the current balance of the charger. This is only updated when the `chargerLink.requestBalance()` method is called.

### `chargerLink.chargeState`

Contains the current charge state of the charger. This is only updated when the `chargerLink.startCharging()` or `chargerLink.stopCharging()` methods are called. During charging the charger will periodically send a `ChargerLinkSignal::CHARGE_STATE` message to the car to update this variable.

The `chargerLink.chargeState` variable contains the following variables:

- `chargerLink.chargeState.charging`
- `chargerLink.chargeState.chargeLevel`
- `chargerLink.chargeState.accountBalance`
- `chargerLink.chargeState.targetChargeLevel`
