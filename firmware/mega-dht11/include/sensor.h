#ifndef SENSOR_H
#define SENSOR_H

void sensor_init();

bool sensor_read(float* temperature, float* humidity);

#endif