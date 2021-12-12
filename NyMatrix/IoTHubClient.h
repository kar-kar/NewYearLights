#pragma once

#include <string>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

class IoTHubClient
{
public:
    enum class State { disconnected, connectingWifi, connectingMqtt, connectedMqtt };
    IoTHubClient(const char* ssid, const char* password, const char* mqttHost, const char* mqttDevice, const char* mqttPassword, MQTT_CALLBACK_SIGNATURE);
    State getState() const { return state; }
    void loop();
private:
    State state = State::disconnected;
    WiFiClientSecure wifiClient;
    PubSubClient psClient;
    long delayConnectUntil;
    std::string ssid;
    std::string password;
    std::string mqttHost;
    std::string mqttDevice;
    std::string mqttUserName;
    std::string mqttPassword;
    std::string mqttSubscribeTopic;
    std::string mqttPublishTopic;
    void connectWiFi();
    void checkWiFiConnectingStatus();
    void connectMqtt();
    bool ensureWiFiConnected();
    bool ensureMqttConnected();
};

