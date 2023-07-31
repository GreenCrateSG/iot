#ifndef TEMPERATURE_H

#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

#include "../class/hydro.h"
#include "../debug/debug.h"
#include "../mqtt/mqtt_local.h"

#define DHTTYPE DHT11   // DHT 11
#define DHTPINTOP 6     // Digital pin connected to the DHT sensor
#define DHTPINBOTTOM 7  // Digital pin connected to the DHT sensor

void temperature_sensor_init();
void temperature_sensor_reading(Hydro& junction_box);

#endif
