#include "IoTHubClient.h"

IoTHubClient::IoTHubClient(const std::string& ssid, const std::string& password, const std::string& mqttHost, const std::string& mqttDevice, const std::string& mqttPassword, MQTT_CALLBACK_SIGNATURE)
    :ssid(ssid), password(password), mqttHost(mqttHost), mqttDevice(mqttDevice), mqttPassword(mqttPassword), psClient(wifiClient)
{
    mqttUserName = this->mqttHost + "/" + this->mqttDevice + "/?api-version=2018-06-30";
    mqttSubscribeTopic = "devices/" + this->mqttDevice + "/messages/devicebound/#";
    mqttPublishTopic = "devices/" + this->mqttDevice + "/messages/events/";
    wifiClient.setInsecure();
    psClient.setServer(this->mqttHost.c_str(), 8883);
    psClient.setCallback(callback);
}

void IoTHubClient::loop()
{
    switch (state)
    {
    case State::disconnected:
        if (delayConnectUntil < millis())
            connectWiFi();
        break;
    case State::connectingWifi:
        checkWiFiConnectingStatus();
        break;
    case State::connectingMqtt:
        psClient.loop();
        if (ensureWiFiConnected() && delayConnectUntil < millis())
            connectMqtt();
        break;
    case State::connectedMqtt:
        psClient.loop();
        if (ensureWiFiConnected())
            ensureMqttConnected();
    }
}

bool IoTHubClient::ensureWiFiConnected()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi disconnected, status = ");
        Serial.println(WiFi.status());
        state = State::disconnected;
        return false;
    }
    return true;
}

bool IoTHubClient::ensureMqttConnected()
{
    if (!psClient.connected())
    {
        Serial.print("MQTT disconnected, status = ");
        Serial.println(psClient.state());
        state = State::connectingMqtt;
        return false;
    }
    return true;
}

void IoTHubClient::connectWiFi()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid.c_str(), password.c_str());
    state = State::connectingWifi;
}

void IoTHubClient::checkWiFiConnectingStatus()
{
    switch (WiFi.status()) {
    case WL_CONNECTED:
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
        state = State::connectingMqtt;
        break;
    case WL_NO_SSID_AVAIL:
    case WL_CONNECT_FAILED:
        Serial.print("WiFi connection failed, status = ");
        Serial.print(WiFi.status());
        Serial.println(", retry in 5 sec");
        delayConnectUntil = millis() + 5000;
        state = State::disconnected;
        break;
    }
}

void IoTHubClient::connectMqtt()
{
    Serial.println("Connecting to MQTT...");

    if (psClient.connect(mqttDevice.c_str(), mqttUserName.c_str(), mqttPassword.c_str()))
    {
        psClient.subscribe(mqttSubscribeTopic.c_str());
        Serial.println("MQTT connected");
        state = State::connectedMqtt;
    }
    else
    {
        Serial.print("MQTT connection failed, status = ");
        Serial.print(psClient.state());
        Serial.println(", retry in 5 sec");
        delayConnectUntil = millis() + 5000;
    }
}
