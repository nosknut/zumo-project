#ifndef IR_READ_SOCKET_H
#define IR_READ_SOCKET_H

#define DECODE_NEC
#define DECODE_FAST

#include <Arduino.h>
#include <Stream.h>
#include <IRremote.hpp>

struct IrReadSocket : public Stream
{
    int value = -1;

    unsigned long bitTransmissionTime = 100;
    unsigned long lastRecvTime = 0;
    bool unread = false;
    IRrecv irrecv;

    /**
     * @brief Construct a new Ir Socket object
     *
     * @param receivePin The pin to receive IR signals from
     */
    IrReadSocket(int receivePin) : irrecv(receivePin)
    {
    }

    void begin()
    {
        irrecv.enableIRIn();
    }

    void end()
    {
        irrecv.disableIRIn();
    }

    int available()
    {
        if (unread)
        {
            return 1;
        }

        if (irrecv.decode())
        {
            irrecv.resume();

            if (irrecv.decodedIRData.protocol == UNKNOWN)
            {

#ifdef DEBUG_UNKNOWN_IR_PROTOCOL
                Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
                // We have an unknown protocol here, print more info
                irrecv.printIRResultRawFormatted(&Serial, true);
#endif

                value = -1;
                unread = false;

                return 0;
            }

            lastRecvTime = millis();

            value = irrecv.decodedIRData.command;
            unread = true;

            return 1;
        }

        return 0;
    }

    int read()
    {
        available();

        int v = value;

        value = -1;
        unread = false;

        return v;
    }

    bool doneReading()
    {
        return !available() && ((millis() - lastRecvTime) >= bitTransmissionTime);
    }

    int peek()
    {
        return value;
    }
};

#endif