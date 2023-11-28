/**
 * @file Zumo32U4IRsender.h
 * @author Trondfc
 * @brief Simple library for sending IR messages with the Zumo32U4.
 *        Written for the Zumo32U4 robot from Pololu.
 *        The library is based on the Zumo32U4IRPulses library.
 *        Sends data based on the NEC protocol.
 *        Made to work with the IRemote library for Arduino.
 * 
 *        Written for use in IELS1001 h2023.
 * 
 * @version 1.0
 * @date 2023-11-15
 * 
 */
#ifndef Zumo32U4IRsender_h
#define Zumo32U4IRsender_h

#define PRINT_DEGUB_INFO 0  // Set to 1 to enable debug info

#define SUBCARIER_PERIOD 420    // The default frequency is 16000000 / (420 + 1) = 38.005 kHz
#define IR_BRIGHNESS 50        // 0-400, 0 is off, 400 is max brightness

#define RIGHT_IR 0
#define LEFT_IR 1

#include <Arduino.h>
#include <Zumo32U4IRPulses.h>

class Zumo32U4IRsender
{
    public:
        Zumo32U4IRsender(uint8_t id, uint8_t direction); // Constructor, used to set up the IR sender
        Zumo32U4IRsender(); // Constructor, used to set up the IR sender (default values)

        void setDirection(uint8_t direction); // Change the direction of the IR LED
        void setDeviceId(uint8_t id); // Change the device ID

        void send(uint8_t data); // Send a byte of data with NEC protocol
    private:

        void sendStartPulse(); // Send the start pulse
        void sendByte(uint8_t byte); // Send a byte of data
        void sendStopPulse(); // Send the stop pulse

        Zumo32U4IRPulses::Direction _IR_DIRECTION = Zumo32U4IRPulses::Right;
        uint8_t _irDeviceId = 0;
};

#endif