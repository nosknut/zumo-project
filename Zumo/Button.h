#ifndef BUTTON_h
#define BUTTON_h

#include <Arduino.h>
#include "Timer.h"

struct Button
{
    int pin = 0;
    bool isPulldown = false;
    bool rawState = false;
    bool state = false;
    bool prevState = false;
    bool pressed = false;
    bool released = false;

    Timer debounceTimer;
    unsigned long debounceDuration = 50;

    void setup()
    {
        pinMode(pin, INPUT);
    }

    void updateState()
    {
        bool newState = isPulldown ? !digitalRead(pin) : digitalRead(pin);

        if (newState != rawState)
        {
            debounceTimer.reset();
            rawState = newState;
        }

        if (debounceTimer.isFinished(debounceDuration))
        {
            state = newState;
        }
    }

    void update()
    {
        updateState();

        pressed = !prevState && state;
        released = prevState && !state;
        prevState = state;
    }
};

#endif
