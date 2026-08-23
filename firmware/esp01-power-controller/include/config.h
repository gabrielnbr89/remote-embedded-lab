#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// Hardware
constexpr uint8_t RELAY_PIN = 0;

// Timing
constexpr unsigned long WIFI_RECONNECT_INTERVAL = 10000;
constexpr unsigned long MQTT_RECONNECT_INTERVAL = 5000;
constexpr unsigned long HEARTBEAT_INTERVAL = 60000;
constexpr unsigned long MQTT_FAILSAFE_TIMEOUT = 60000;

// MQTT
constexpr uint16_t MQTT_PORT = 1883;

constexpr char DEVICE_ID[] = "esp01-power";

constexpr size_t MQTT_PAYLOAD_SIZE = 256;

constexpr char MQTT_TOPIC_AVAILABILITY[] =
    "lab/devices/esp01-power/availability";

constexpr char MQTT_TOPIC_EVENTS[] =
    "lab/devices/esp01-power/events";

constexpr char MQTT_TOPIC_COMMAND[] =
    "lab/devices/esp01-power/command";

constexpr char MQTT_TOPIC_STATE[] =
    "lab/devices/esp01-power/state";

constexpr char MQTT_TOPIC_STATE_REQUEST[] =
    "lab/devices/esp01-power/state/request";

#endif