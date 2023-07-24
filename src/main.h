#ifndef MAIN_H
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Ezo_i2c.h>       //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Ezo_i2c_util.h>  //brings in common print statements
#include <TSL2561.h>
#include <sequencer1.h>  //imports a 1 function sequencer
#include <sequencer4.h>  //imports a 4 function sequencer

#include "components/class/hydro.h"
#include "components/debug/debug.h"
#include "components/eth/eth.h"
#include "components/gpio/gpio_local.h"
#include "components/mqtt/mqtt_local.h"
#include "components/sensors/lux.h"
#include "components/sensors/temperature.h"

unsigned long lastMillis = 0;  // counter

void step1();  // forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();
void step4();

#endif
