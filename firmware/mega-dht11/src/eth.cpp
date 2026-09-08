#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include "config.h"
#include "eth.h"

namespace
{
    unsigned long lastEthernetAttempt = 0;
    uint32_t ethernetConnectionAttempts = 0;

    bool ethernetConnected = false;

    void ethernet_connect()
    {
        ++ethernetConnectionAttempts;

        lastEthernetAttempt = millis();

        //copia mutable de la mac para pasarla a Ethernet.begin() porque así lo tiene definido la libreria

        byte mac[] =
            {
                ETHERNET_MAC[0],
                ETHERNET_MAC[1],
                ETHERNET_MAC[2],
                ETHERNET_MAC[3],
                ETHERNET_MAC[4],
                ETHERNET_MAC[5]};

        ethernetConnected = Ethernet.begin(mac) != 0;
    }
}

void ethernet_init()
{
    ethernet_connect();
}

void ethernet_update()
{
    if (ethernet_is_connected())
        return;

    const unsigned long now = millis();

    if (now - lastEthernetAttempt < ETHERNET_RECONNECT_INTERVAL)
        return;

    ethernet_connect();
}

bool ethernet_is_connected()
{
    return ethernetConnected &&
           Ethernet.linkStatus() == LinkON;
}

uint32_t ethernet_get_connection_attempts()
{
    return ethernetConnectionAttempts;
}

void ethernet_get_ip(char *buffer, size_t size)
{
    if (size == 0)
        return;

    const IPAddress ip = Ethernet.localIP();

    snprintf(
        buffer,
        size,
        "%u.%u.%u.%u",
        ip[0],
        ip[1],
        ip[2],
        ip[3]);
}

void ethernet_reset_connection_attempts()
{
    ethernetConnectionAttempts = 0;
}

void ethernet_get_mac(char *buffer, size_t size)
{
    if (size == 0)
        return;

    snprintf(
        buffer,
        size,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        ETHERNET_MAC[0],
        ETHERNET_MAC[1],
        ETHERNET_MAC[2],
        ETHERNET_MAC[3],
        ETHERNET_MAC[4],
        ETHERNET_MAC[5]);
}