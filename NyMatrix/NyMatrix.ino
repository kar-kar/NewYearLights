#include <string>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "Blinker.h"
#include "IoTHubClient.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define MQTT_HOST ""
#define MQTT_DEVICE ""
#define MQTT_PASSWORD ""

//define actual settings in DeviceConfig.h
#if __has_include("DeviceConfig.h")
#include "DeviceConfig.h"
#endif

void mqttCallback(char* topic, byte* payload, unsigned int length);
IoTHubClient client(WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_DEVICE, MQTT_PASSWORD, mqttCallback);
Blinker blinker(LED_BUILTIN);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    client.loop();
    updateBlinker();
    blinker.loop();
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
}
