#ifndef ZUMO_IRSOCKET_H
#define ZUMO_IRSOCKET_H

#include "IrReadSocket.h"
#include "Zumo32u4IRsender.h"

struct ZumoIrSocket : public IrReadSocket
{

    int bootDelay = 500;
    Zumo32U4IRsender irsend;

    /**
     * @brief Construct a new Ir Socket object
     *
     * @param sendPin The pin to send IR signals from
     * @param receivePin The pin to receive IR signals from
     */
    ZumoIrSocket(int receivePin, uint8_t id, uint8_t direction)
        : irsend(id, direction),
          IrReadSocket(receivePin)
    {
    }

    void begin()
    {
        if (millis() < bootDelay)
        {
            // During startup the first 6 bytes are not received correctly
            // without waiting for 500ms
            delay(bootDelay - millis());
        }

        IrReadSocket::begin();
    }

    void flush()
    {
    }

    size_t write(uint8_t byte)
    {
        // Temporarily disable receiving IR signals to prevent it from
        // receiving the signal it just sent
        end();
        irsend.send(byte);
        delay(12);
        begin();
        return 1;
    }
};

#endif