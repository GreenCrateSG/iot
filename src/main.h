#pragma once

#include <DHT.h>
#include <DHT_U.h>
#include <Digital_Light_TSL2561.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>  // Used to establied serial communication on the I2C bus
#include <Adafruit_Sensor.h>

//
#include "arduino_sercret.h"

/** Functions **/

void temperature_sensor_init();
void temperature_sensor_reading();
void lux_sensor_init();

/****/
