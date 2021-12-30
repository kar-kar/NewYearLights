#include "Wave.h"

void Wave::setupInternal()
{
}

void Wave::loopInternal(unsigned long dt)
{
    phase = fmod(phase + dt * waveSpeed, 1.0f);
    draw();
}

void Wave::draw()
{
    clear();

    for (int x = 0; x < MATRIX_WIDTH; x++)
    {
        auto dist = fmod(abs(x - waveLength * phase), waveLength);
        dist = min(dist, waveLength - dist);
        auto ratio = dist < 1.0f ? 1.0f : 1.0f / dist;
        ratio = max(0.0f, min(1.0f, ratio));
        auto color = RgbColor::LinearBlend(lowColor, highColor, ratio);

        for (int y = 0; y < MATRIX_HEIGHT; y++)
            if ((x + y) % 2 != 0)
                matrix->SetPixelColor(topo->Map(x, y), color);
    }

    dim();
}
