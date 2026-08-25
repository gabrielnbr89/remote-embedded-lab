#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <cstring>

#include "config.h"
#include "secrets.h"
#include "mqtt.h"
#include "wifi.h"
#include "relay.h"
#include "logger.h"

namespace
{
    WiFiClient wifiClient;
    PubSubClient mqttClient(wifiClient);

    unsigned long lastMqttAttempt = 0;
    unsigned long mqttFailsafe = 0;
    uint32_t mqttConnectionAttempts = 0;

    bool mqttHasConnected = false;
    bool mqttFailsafeTriggered = false;

    void mqtt_callback(char *topic, byte *payload, unsigned int length)
    {
        if (strcmp(topic, MQTT_TOPIC_COMMAND) == 0)
        {
            JsonDocument document;

            DeserializationError error =
                deserializeJson(document, payload, length);

            if (error)
                return;

            if (!document["command"].is<bool>())
                return;

            const bool state = document["command"].as<bool>();

            relay_set(state);
            logger_relay_command(document);
            relay_publish_state();

            return;
        }

        if (strcmp(topic, MQTT_TOPIC_STATE_REQUEST) == 0)
        {
            relay_publish_state();
            return;
        }
    }

    void mqtt_connect()
    {
        ++mqttConnectionAttempts;

        lastMqttAttempt = millis();

        if (!mqttClient.connect(
                DEVICE_ID,
                MQTT_USER,
                MQTT_PASSWORD))
        {

            return;
        }

        mqttClient.subscribe(MQTT_TOPIC_COMMAND);
        mqttClient.subscribe(MQTT_TOPIC_STATE_REQUEST);

        if (!mqttHasConnected)
        {
            mqttHasConnected = true;

            logger_startup();
        }
        else
        {
            logger_mqtt_reconnected();
        }
    }
}

void mqtt_init()
{
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(mqtt_callback);
}

void mqtt_update()
{
    const unsigned long now = millis();

    if (!mqttClient.connected())
    {
        if (mqttFailsafe == 0)
        {
            mqttFailsafe = now;
        }

        if (now - mqttFailsafe >= MQTT_FAILSAFE_TIMEOUT)
        {
            if (!mqttFailsafeTriggered)
            {
                mqttFailsafeTriggered = true;
                relay_set(false);
            }
        }
    }
    else
    {
        mqttFailsafeTriggered = false;
        mqttFailsafe = 0;
    }

    if (!wifi_is_connected())
        return;

    if (!mqttClient.connected())
    {
        if (now - lastMqttAttempt >= MQTT_RECONNECT_INTERVAL)
        {
            mqtt_connect();
        }

        return;
    }

    mqttClient.loop();
}

bool mqtt_is_connected()
{
    return mqttClient.connected();
}

uint32_t mqtt_get_connection_attempts()
{
    return mqttConnectionAttempts;
}

bool mqtt_publish(const char *topic, const char *payload)
{
    if (!mqttClient.connected())
        return false;

    return mqttClient.publish(topic, payload);
}

void mqtt_reset_connection_attempts()
{
    mqttConnectionAttempts = 0;
}