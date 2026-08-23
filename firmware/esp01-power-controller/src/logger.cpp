#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"
#include "logger.h"
#include "mqtt.h"
#include "relay.h"
#include "storage.h"
#include "wifi.h"

namespace
{
    void logger_publish(JsonDocument& document)
    {
        char payload[MQTT_PAYLOAD_SIZE];

        serializeJson(document, payload, sizeof(payload));

        mqtt_publish(MQTT_TOPIC_EVENTS, payload);
    }
}


void logger_startup()
{
    JsonDocument document;

    document["event"] = "startup";
    document["device_id"] = DEVICE_ID;
    document["boot_count"] = storage_get_boot_count();

    JsonObject wifi = document["wifi"].to<JsonObject>();

    wifi["attempts"] = wifi_get_connection_attempts();

    char mac[18];
    wifi_get_mac(mac, sizeof(mac));
    wifi["mac"] = mac;

    char ip[16];
    wifi_get_ip(ip, sizeof(ip));
    wifi["ip"] = ip;

    wifi["rssi"] = wifi_get_rssi();

    JsonObject mqtt = document["mqtt"].to<JsonObject>();
    mqtt["attempts"] = mqtt_get_connection_attempts();

    logger_publish(document);

    wifi_reset_connection_attempts();
    mqtt_reset_connection_attempts();
}

void logger_mqtt_reconnected()
{
    JsonDocument document;

    document["event"] = "mqtt_reconnected";

    JsonObject wifi = document["wifi"].to<JsonObject>();
    wifi["attempts"] = wifi_get_connection_attempts();

    JsonObject mqtt = document["mqtt"].to<JsonObject>();
    mqtt["attempts"] = mqtt_get_connection_attempts();

    logger_publish(document);

    wifi_reset_connection_attempts();
    mqtt_reset_connection_attempts();
}

void logger_relay_command(JsonDocument& commandPayload)
{
    JsonDocument document;

    document["event"] = "relay_command";
    document["command"] = commandPayload["command"];

    logger_publish(document);
}