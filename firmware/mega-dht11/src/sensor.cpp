#include "Arduino.h"
#include "DHT.h"

#include "config.h"
#include "sensor.h"

namespace
{
    DHT dht(DHT11_PIN, DHT11);
}

void sensor_init(){
    dht.begin();
}

bool sensor_read(float* temperature, float* humidity){
    float measuredTemperature = dht.readTemperature();
    float measuredHumidity = dht.readHumidity();

    //validamos que las mediciones no sean NaN antes de pasarlas a los punteros de salida para evitar dejar valores nan en las variables de salida
    if(isnan(measuredTemperature) || isnan(measuredHumidity))
        return false;

    *temperature = measuredTemperature;
    *humidity = measuredHumidity;    
    return true;
}