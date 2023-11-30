#ifndef IRSOCKET_H
#define IRSOCKET_H

#include "IrReadSocket.h"

struct IrSocket : public IrReadSocket
{
    IRsend irsend;

    /**
     * @brief Construct a new Ir Socket object
     *
     * @param sendPin The pin to send IR signals from
     * @param receivePin The pin to receive IR signals from
     */
    IrSocket(
        int sendPin,
        int receivePin)
        : irsend(sendPin),
          IrReadSocket(receivePin)
    {
    }

    void flush()
    {
    }

    size_t write(uint8_t byte)
    {
        // Temporarily disable receiving IR signals to prevent it from
        // receiving the signal it just sent
        end();
        irsend.sendFAST(byte, 0);
        delay(7);
        begin();
        return 1;
    }
};

#endif