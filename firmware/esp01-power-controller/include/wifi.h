#ifndef WIFI_H
#define WIFI_H

void wifi_init();
void wifi_update();
bool wifi_is_connected();
uint32_t wifi_get_connection_attempts();

void wifi_get_mac(char* buffer, size_t size);
void wifi_get_ip(char* buffer, size_t size);
int32_t wifi_get_rssi();
void wifi_reset_connection_attempts();

#endif