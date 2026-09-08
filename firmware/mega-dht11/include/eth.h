#ifndef ETHERNET_H
#define ETHERNET_H

#include <stddef.h>
#include <stdint.h>

void ethernet_init();
void ethernet_update();
bool ethernet_is_connected();
uint32_t ethernet_get_connection_attempts();
void ethernet_get_ip(char* buffer, size_t size);
void ethernet_get_mac(char* buffer, size_t size);
void ethernet_reset_connection_attempts();

#endif