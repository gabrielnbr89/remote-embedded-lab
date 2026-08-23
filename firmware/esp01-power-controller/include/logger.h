#ifndef LOGGER_H
#define LOGGER_H
#include <ArduinoJson.h>
void logger_startup();
void logger_wifi_reconnected();
void logger_mqtt_reconnected();
void logger_relay_command(JsonDocument& commandPayload);

#endif