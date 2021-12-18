#include <string>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NeoPixelBus.h>
#include "Blinker.h"
#include "IoTHubClient.h"
#include "MyLayout.h"
#include "Effect.h"
#include "Rain.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define MQTT_HOST ""
#define MQTT_DEVICE ""
#define MQTT_PASSWORD ""

#if __has_include("DeviceConfig.h")
#include "DeviceConfig.h"
#endif

void mqttCallback(char* topic, byte* payload, unsigned int length);

NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> matrix(PIXEL_COUNT);
NeoTopology<MyLayout> topo(MATRIX_WIDTH, MATRIX_HEIGHT);
IoTHubClient client(WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_DEVICE, MQTT_PASSWORD, mqttCallback);
Blinker blinker(LED_BUILTIN);
bool isRunning = false;
int currentEffectIndex;
unsigned long currentEffectStart;
unsigned long fpsStart;
int frameCount = 0;

std::vector<Effect*> effects =
{
    new Rain(&matrix, &topo),
    new Rain(&matrix, &topo, { RgbColor(255, 255, 255) }, RgbColor(0, 0, 0), 0.003, 0, 3, 7)
};

void setup()
{
    Serial.begin(115200);
    matrix.Begin();
    start();
}

void loop()
{
    client.loop();
    updateBlinker();
    blinker.loop();

    if (isRunning)
    {
        effects[currentEffectIndex]->loop();
        matrix.Show();
        countFps();
    }
}

void start()
{
    if (isRunning)
        return;

    Serial.println("Start effects");

    effects[currentEffectIndex]->setup();
    currentEffectStart = millis();
    fpsStart = currentEffectStart;
    frameCount = 0;
    isRunning = true;
}

void stop()
{
    if (!isRunning)
        return;

    Serial.println("Stop effects");
    matrix.ClearTo(RgbColor(0));
    matrix.Show();
    isRunning = false;
}

void next()
{
    currentEffectIndex = (currentEffectIndex + 1) % effects.size();
    Serial.print("Switch to effect #");
    Serial.println(currentEffectIndex);
    effects[currentEffectIndex]->setup();
}

void setBrightness(int percentage)
{
    Serial.print("Set brightness to ");
    Serial.println(percentage);

    auto brightness = (float)percentage / 100.0f;
    for (auto& effect : effects)
        effect->setBrightness(brightness);
}

void countFps()
{
    auto ms = millis();
    frameCount++;

    if (ms - fpsStart >= 1000)
    {
        auto fps = frameCount * 1000.0 / (ms - fpsStart);
        Serial.print("FPS: ");
        Serial.println(fps, 2);
        fpsStart = ms;
        frameCount = 0;
    }
}

void updateBlinker()
{
    switch (client.getState()) {
    case IoTHubClient::State::disconnected:
        blinker.setPeriod(-1);
        break;
    case IoTHubClient::State::connectingWifi:
        blinker.setPeriod(1000);
        break;
    case IoTHubClient::State::connectingMqtt:
        blinker.setPeriod(500);
        break;
    case IoTHubClient::State::connectedMqtt:
        blinker.setPeriod(0);
        break;
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    blinker.blinkTemp(100, 300);
    std::string message((char*)payload, length);

    Serial.print("Message arrived: ");
    Serial.println(message.c_str());

    if (message == "start")
        start();
    else if (message == "stop")
        stop();
    else if (message == "next")
        next();
    else if (message.rfind("br ", 0) == 0)
        setBrightness(std::atoi(message.substr(3).c_str()));
}
