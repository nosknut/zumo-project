/**
 * @file Zumo32U4IRsender.cpp
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

#include "Zumo32U4IRsender.h"

/**
 * @brief Construct a new Zumo32U4IRsender::Zumo32U4IRsender object
 * 
 * @param id ID of the device, used to identify the robot
 * @param direction Direction of the IR LEDs to be used, 0 for right, 1 for left
 */
Zumo32U4IRsender::Zumo32U4IRsender(uint8_t id, uint8_t direction){
    if(PRINT_DEGUB_INFO)Serial.print("Setting up IR sender with ID ");
    if(PRINT_DEGUB_INFO)Serial.print(id);
    if(direction == 0){
        if(PRINT_DEGUB_INFO)Serial.println(" and direction Right");
    } else {
        if(PRINT_DEGUB_INFO)Serial.println(" and direction Left");
    }

    _irDeviceId = id;
    if(direction == 0) {
        _IR_DIRECTION = Zumo32U4IRPulses::Direction::Right;
    } else {
        _IR_DIRECTION = Zumo32U4IRPulses::Direction::Left;
    }
}

/**
 * @brief Construct a new Zumo32U4IRsender::Zumo32U4IRsender object
 *        Default values: ID = 0, Direction = Right
 */
Zumo32U4IRsender::Zumo32U4IRsender(){
    if(PRINT_DEGUB_INFO)Serial.print("Setting up IR sender with ID ");
    if(PRINT_DEGUB_INFO)Serial.print(0);
    if(PRINT_DEGUB_INFO)Serial.println(" and direction Right");

    _irDeviceId = 0;
    _IR_DIRECTION = Zumo32U4IRPulses::Direction::Right;
}

/**
 * @brief Function for changing the direction of the IR LEDs
 * 
 * @param direction Direction of the IR LEDs to be used, 0 for right, 1 for left
 */
void Zumo32U4IRsender::setDirection(uint8_t direction){
    if(direction == 0){
        if(PRINT_DEGUB_INFO)Serial.println("Setting direction Right");
    } else {
        if(PRINT_DEGUB_INFO)Serial.println("Setting direction Left");
    }

    if(direction == 0) {
        _IR_DIRECTION = Zumo32U4IRPulses::Direction::Right;
    } else {
        _IR_DIRECTION = Zumo32U4IRPulses::Direction::Left;
    }
}

/**
 * @brief Function for changing the device ID
 * 
 * @param id Device ID
 */
void Zumo32U4IRsender::setDeviceId(uint8_t id){
    if(PRINT_DEGUB_INFO)Serial.print("Setting device ID to ");
    if(PRINT_DEGUB_INFO)Serial.println(id);

    _irDeviceId = id;
}

/**
 * @brief Function for sending a byte of data with the NEC protocol
 * 
 * @param data Data to be sent
 */
void Zumo32U4IRsender::send(uint8_t data){
    if(PRINT_DEGUB_INFO)Serial.print("Sending : ");
    if(PRINT_DEGUB_INFO)Serial.print(data);
    if(PRINT_DEGUB_INFO)Serial.print(" From : ");
    if(PRINT_DEGUB_INFO)Serial.println(_irDeviceId);

    sendStartPulse();
    sendByte(_irDeviceId);
    sendByte(~_irDeviceId);
    sendByte(data);
    sendByte(~data);
    sendStopPulse();
}

/**
 * @brief Function for sending the start pulse acording to the NEC protocol
 * 
 */
void Zumo32U4IRsender::sendStartPulse(){
    Zumo32U4IRPulses::start(_IR_DIRECTION, IR_BRIGHNESS, SUBCARIER_PERIOD);  // Turn on IR LED int the specified direction with subcarrier
    delayMicroseconds(9000);
    Zumo32U4IRPulses::stop();
    delayMicroseconds(4500);
}

/**
 * @brief Function for sending a byte of data with the NEC protocol
 * 
 * @param byte byte of data to be sent
 */
void Zumo32U4IRsender::sendByte(uint8_t byte){
    for (uint8_t i = 0; i < 8; i++) {   // iterate through the bits in the byte, LSB first
        uint8_t bit = (byte >> (i)) & 1;  // get the current bit
        if (bit == 1) {                   // if the bit is 1, send 1
            Zumo32U4IRPulses::start(_IR_DIRECTION, IR_BRIGHNESS, SUBCARIER_PERIOD);  // Turn on IR LED int the specified direction with subcarrier
            delayMicroseconds(560);
            Zumo32U4IRPulses::stop();
            delayMicroseconds(1690);
        } else {  // if the bit is 0, send 0
            Zumo32U4IRPulses::start(_IR_DIRECTION, IR_BRIGHNESS, SUBCARIER_PERIOD);  // Turn on IR LED int the specified direction with subcarrier
            delayMicroseconds(560);
            Zumo32U4IRPulses::stop();
            delayMicroseconds(560);
        }
    }
}

/**
 * @brief Function for sending the stop pulse acording to the NEC protocol
 * 
 */
void Zumo32U4IRsender::sendStopPulse(){
    Zumo32U4IRPulses::start(_IR_DIRECTION, IR_BRIGHNESS, SUBCARIER_PERIOD);
    delayMicroseconds(560);
    Zumo32U4IRPulses::stop();
    delayMicroseconds(560);
}