#ifndef RELAY_H
#define RELAY_H

void relay_init();
void relay_set(bool state);
bool relay_get_state();
void relay_publish_state();

#endif