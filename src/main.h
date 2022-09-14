#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>  // Used to establied serial communication on the I2C bus
#include <arduino_sercret.h>

// Mine
#include "constants.h"

/** Functions **/

void temperature_sensor_init();
void temperature_sensor_reading();
void lux_sensor_init();
void lux_sensor_get_reading();
void light_init();
void motor_init();

/****/
