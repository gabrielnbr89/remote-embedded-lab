#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "config.h"
#include "secrets.h"
#include "wifi.h"

namespace
{
    unsigned long lastWifiAttempt = 0;
    uint32_t wifiConnectionAttempts = 0;

    void wifi_connect()
    {
        ++wifiConnectionAttempts;

        lastWifiAttempt = millis();

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}

void wifi_init()
{
    WiFi.mode(WIFI_STA);

    wifi_connect();
}

void wifi_update()
{
    if (wifi_is_connected())
        return;

    const unsigned long now = millis();

    if (now - lastWifiAttempt < WIFI_RECONNECT_INTERVAL)
        return;

    wifi_connect();
}

bool wifi_is_connected()
{
    return WiFi.status() == WL_CONNECTED;
}

uint32_t wifi_get_connection_attempts()
{
    return wifiConnectionAttempts;
}

void wifi_get_mac(char* buffer, size_t size)
{
    if (size == 0)
        return;

    WiFi.macAddress().toCharArray(buffer, size);
}

void wifi_get_ip(char* buffer, size_t size)
{
    if (size == 0)
        return;

    WiFi.localIP().toString().toCharArray(buffer, size);
}

int32_t wifi_get_rssi()
{
    return WiFi.RSSI();
}

void wifi_reset_connection_attempts()
{
    wifiConnectionAttempts = 0;
}