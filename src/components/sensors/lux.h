#ifndef LUX_H
#include <Arduino.h>

#include "../debug/debug.h"
#include "../mqtt/mqtt_local.h"
#include "TSL2561.h"

void lux_sensor_init(TSL2561 tslOne, TSL2561 tslTwo);
void lux_sensor_reading(TSL2561 tslOne, TSL2561 tslTwo);
#endif
