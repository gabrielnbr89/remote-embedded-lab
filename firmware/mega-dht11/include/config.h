#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// Hardware
constexpr uint8_t DHT11_PIN = 8;

// Timing
constexpr unsigned long ETHERNET_RECONNECT_INTERVAL = 10000;
constexpr unsigned long MEASUREMENT_INTERVAL = 3000;

// MQTT
constexpr uint16_t MQTT_PORT = 1883;

constexpr char DEVICE_ID[] = "mega-dht11";

constexpr size_t MQTT_PAYLOAD_SIZE = 256;

constexpr char MQTT_TOPIC_AVAILABILITY[] =
    "lab/devices/mega-dht11/availability";

constexpr char MQTT_TOPIC_EVENTS[] =
    "lab/devices/mega-dht11/events";

constexpr char MQTT_TOPIC_MEASUREMENTS[] =
    "lab/devices/mega-dht11/measurements";

    // Ethernet
    constexpr byte ETHERNET_MAC[] =
{
    0x02,
    0xD1,
    0x0D,
    0xED,
    0x10,
    0xDE
};
#endif