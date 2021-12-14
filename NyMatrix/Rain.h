#pragma once
#include <NeoPixelBus.h>
#include "Effect.h"

class Rain : public Effect
{
public:
    using Effect::Effect;
    Rain(NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix, NeoTopology<MyLayout>* topo,
        const std::vector<RgbColor>& colors, RgbColor background, float speed, float tail, int minDistance, int maxDistance);
    void setup();
    void loop();
private:
    struct Drop
    {
        int x;
        float y;
        RgbColor color;
    };

    std::vector<RgbColor> colors =
    {
        RgbColor(100, 100, 255),
        RgbColor(255, 0, 255),
        RgbColor(100, 0, 255),
        RgbColor(0, 100, 255),
        RgbColor(0, 255, 255),
        RgbColor(0, 0, 255)
    };

    RgbColor background = RgbColor(0, 0, 0);
    float speed = 0.01f;
    float tail = 3.0f;
    int minDistance = 5;
    int maxDistance = 20;
    std::vector<Drop> drops;
    unsigned long lastMillis;

    Drop createDrop(int x);
    void clear();
    void move(unsigned long dt);
    void draw();
};

