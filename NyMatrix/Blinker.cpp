#include "Blinker.h"

Blinker::Blinker(byte pin, int period) : pin(pin), period(period)
{
    pinMode(pin, OUTPUT);
}

void Blinker::loop()
{
    auto ms = millis();
    auto currentPeriod = ms < tempBlinkingEnd ? tempPeriod : period;

    if (currentPeriod < 0)
    {
        digitalWrite(pin, HIGH);
    }
    else if (currentPeriod == 0)
    {
        digitalWrite(pin, LOW);
    }
    else
    {
        auto phase = ms % currentPeriod;
        digitalWrite(pin, phase < currentPeriod / 2 ? LOW : HIGH);
    }
}
