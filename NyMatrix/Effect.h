#pragma once
#include <memory>
#include <NeoPixelBus.h>
#include "MyLayout.h"

class Effect
{
public:
    Effect(NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix, NeoTopology<MyLayout>* topo)
        : matrix(matrix), topo(topo) { }
    void setBrightness(float value) { brightness = value; }
    virtual void setup() = 0;
    virtual void loop() = 0;
protected:
    NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix;
    NeoTopology<MyLayout>* topo;
    float brightness = 0.5f;
};

