#include <Arduino.h>

#include "config.h"
#include "relay.h"
#include "mqtt.h"
#include "arduinoJson.h"

namespace
{
    constexpr uint8_t RELAY_ON_LEVEL = LOW;
    constexpr uint8_t RELAY_OFF_LEVEL = HIGH;
}

void relay_init()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, RELAY_OFF_LEVEL);
}

void relay_set(bool state)
{
    digitalWrite(
        RELAY_PIN,
        state ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL
    );
}

bool relay_get_state()
{
    return digitalRead(RELAY_PIN) == RELAY_ON_LEVEL;
}

void relay_publish_state()
{
    JsonDocument document;

    document["relay"] = relay_get_state();

    char payload[MQTT_PAYLOAD_SIZE];

    serializeJson(document, payload, sizeof(payload));

    mqtt_publish(MQTT_TOPIC_STATE, payload);
}