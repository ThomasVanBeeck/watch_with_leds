#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Arduino.h>

class TimeManager {
    public:
    // Constructor
    TimeManager(byte setSeconds = 0, byte setMinutes = 0, byte setHours = 0);

    // getters en setters voor seconden, minuten en uren
    byte* getSeconds();
    byte* getMinutes();
    byte* getHours();
    
    void setSeconds(byte setSeconds);
    void setMinutes(byte setMinutes);
    void setHours(byte setHours);
    
    // functie die een seconde toevoegd aan huidige tijd
    void addSecond();
    private:
    byte seconds, minutes, hours;
};

#endif