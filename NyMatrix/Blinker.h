#pragma once
#include <Arduino.h>

class Blinker {
public:
    Blinker(byte pin, int period = -1);
    void loop();
    void setPeriod(int period) { this->period = period; }
    void blinkTemp(int period, int duration) { tempPeriod = period; tempBlinkingEnd = millis() + duration; }
private:
    byte pin;
    int period;
    int tempPeriod;
    long tempBlinkingEnd;
};

