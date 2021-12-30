#pragma once
#include <memory>
#include <NeoPixelBus.h>
#include "MyLayout.h"

class Effect
{
public:
    Effect(NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix, NeoTopology<MyLayout>* topo, RgbColor background)
        : matrix(matrix), topo(topo), background(background) { }
    void setBrightness(float value) { brightness = value; }
    void setup();
    void loop();
protected:
    NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix;
    NeoTopology<MyLayout>* topo;
    RgbColor background;
    float brightness = 0.2f;
    unsigned long lastMillis;

    virtual void setupInternal() = 0;
    virtual void loopInternal(unsigned long dt) = 0;
    void clear();
    void dim();
};

