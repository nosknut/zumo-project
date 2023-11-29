#ifndef LINK_COMMANDS_h
#define LINK_COMMANDS_h

struct LinkCommands
{
    static const int STOP_CHARGING = 0;
    static const int START_CHARGING = 1;
    static const int CHARGE_STATE = 2;
    static const int REQUEST_BALANCE = 3;
    static const int BALANCE = 4;
    static const char LINK_AVAILABLE = '@';
};

#endif