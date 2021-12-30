#pragma once
#include <NeoPixelBus.h>
#include "Effect.h"

class Rain : public Effect
{
public:
    using Effect::Effect;
    Rain(NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix, NeoTopology<MyLayout>* topo, RgbColor background,
        const std::vector<RgbColor>& colors, float speed, float tail, int minDistance, int maxDistance);
protected:
    void setupInternal();
    void loopInternal(unsigned long dt);
private:
    struct Drop
    {
        int x;
        float y;
        RgbColor color;
    };

    std::vector<RgbColor> colors =
    {
        RgbColor(0, 0, 100),
        RgbColor(100, 100, 255),
        RgbColor(100, 0, 255),
        RgbColor(0, 100, 255),
        RgbColor(0, 0, 255)
    };

    float speed = 0.008f;
    float tail = 3.0f;
    int minDistance = 5;
    int maxDistance = 20;
    std::vector<Drop> drops;

    Drop createDrop(int x);
    void move(unsigned long dt);
    void draw();
};

