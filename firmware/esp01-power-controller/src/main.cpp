#include <Arduino.h>

#include "mqtt.h"
#include "relay.h"
#include "storage.h"
#include "wifi.h"
#include "logger.h"

void setup()
{
    relay_init();
    storage_init();
    wifi_init();
    mqtt_init();
}

void loop()
{
    wifi_update();
    mqtt_update();
    logger_update();
}