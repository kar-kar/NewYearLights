#include "Rain.h"

Rain::Rain(NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod>* matrix, NeoTopology<MyLayout>* topo,
    const std::vector<RgbColor>& colors, RgbColor background, float speed, float tail, int minDistance, int maxDistance)
    : Effect(matrix, topo), colors(colors), background(background), speed(speed), tail(tail), minDistance(minDistance), maxDistance(maxDistance)
{
}

void Rain::setup()
{
    auto dropCount = ((MATRIX_HEIGHT + (int)tail * 2) / (minDistance + 1) + 1) * MATRIX_WIDTH;
    drops.clear();
    int x = 0;

    for (int i = 0; i < dropCount; i++)
    {
        drops.push_back(createDrop(x));
        x = (x + 1) % MATRIX_WIDTH;
    }

    clear();
    lastMillis = millis();
}

void Rain::loop()
{
    auto ms = millis();
    auto dt = ms - lastMillis;
    lastMillis = ms;

    if (dt == 0)
        return;

    move(dt);
    draw();
}

Rain::Drop Rain::createDrop(int x)
{
    auto minY = 0.0f;
    for (const auto& drop : drops)
        if (drop.x == x)
            minY = min(minY, drop.y);

    return Rain::Drop
    {
        x,
        minY - random(minDistance, maxDistance + 1),
        colors[random(colors.size())]
    };
}

void Rain::clear()
{
    for (int x = 0; x < MATRIX_WIDTH; x++)
        for (int y = 0; y < MATRIX_WIDTH; y++)
            matrix->SetPixelColor(topo->Map(x, y), background);
}

void Rain::move(unsigned long dt)
{
    auto dy = speed * dt;

    for (auto& drop : drops)
        drop.y += dy;

    for (int i = 0; i < drops.size(); i++)
        if (drops[i].y > MATRIX_HEIGHT + tail * 2)
            drops[i] = createDrop(drops[i].x);
}

void Rain::draw()
{
    clear();

    for (const auto& drop : drops)
    {
        if (drop.y < -1)
            continue;

        for (int pixelY = 0; pixelY < MATRIX_HEIGHT; pixelY++)
        {
            float dist = abs(pixelY - drop.y);
            float factor = 1 - pow(dist + 0.3f, 2);
            factor = max(0.0f, min(1.0f, factor));

            if (pixelY < drop.y && tail > 0)
            {
                factor += 1 - pow(1.5f, dist - tail);
                factor = max(0.0f, min(1.0f, factor));
            }

            if (factor < 0.005f)
                continue;

            auto pixelColor = (RgbColor)matrix->GetPixelColor(topo->Map(drop.x, pixelY));
            auto newColor = drop.color.Dim((byte)(factor * 255));
            newColor.R = max(newColor.R, pixelColor.R);
            newColor.G = max(newColor.G, pixelColor.G);
            newColor.B = max(newColor.B, pixelColor.B);
            matrix->SetPixelColor(topo->Map(drop.x, pixelY), newColor);
        }
    }
}
