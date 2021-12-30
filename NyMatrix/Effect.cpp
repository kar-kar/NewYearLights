#include "Effect.h"

void Effect::setup()
{
    clear();
    lastMillis = 0;
    setupInternal();
}

void Effect::loop()
{
    auto ms = millis();
    auto dt = ms - lastMillis;
    lastMillis = ms;

    if (dt > 0)
        loopInternal(dt);
}

void Effect::clear()
{
    for (int x = 0; x < MATRIX_WIDTH; x++)
        for (int y = 0; y < MATRIX_WIDTH; y++)
            matrix->SetPixelColor(topo->Map(x, y), background);
}

void Effect::dim()
{
    auto dimRatio = (byte)(brightness * 255);
    if (dimRatio < 255)
    {
        for (int x = 0; x < MATRIX_WIDTH; x++)
        {
            for (int y = 0; y < MATRIX_WIDTH; y++)
            {
                auto color = matrix->GetPixelColor(topo->Map(x, y));
                matrix->SetPixelColor(topo->Map(x, y), color.Dim(dimRatio));
            }
        }
    }
}
