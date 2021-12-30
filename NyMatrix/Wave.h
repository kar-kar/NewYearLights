#pragma once

#include <NeoPixelBus.h>
#include "Effect.h"


class Wave : public Effect
{
public:
    using Effect::Effect;
protected:
    void setupInternal();
    void loopInternal(unsigned long dt);
private:
    float phase;
    float waveLength = 22.0f;
    float waveSpeed = 0.0004f;
    RgbColor lowColor = RgbColor(20, 10, 0);
    RgbColor highColor = RgbColor(255, 200, 40);

    void draw();
};

