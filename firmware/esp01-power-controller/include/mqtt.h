#ifndef MQTT_H
#define MQTT_H

void mqtt_init();
void mqtt_update();
bool mqtt_is_connected();
uint32_t mqtt_get_connection_attempts();
bool mqtt_publish(const char* topic, const char* payload);
void mqtt_reset_connection_attempts();

#endif