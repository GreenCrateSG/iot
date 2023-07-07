#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Ethernet.h>
#include <Ezo_i2c.h>       //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Ezo_i2c_util.h>  //brings in common print statements
#include <MQTT.h>
#include <SPI.h>
#include <Wire.h>        // Used to establied serial communication on the I2C bus
#include <sequencer1.h>  //imports a 1 function sequencer
#include <sequencer4.h>  //imports a 4 function sequencer

#include "TSL2561.h"

//
#include "arduino_sercret.h"

/** Functions **/

void gpio_init();
void temperature_sensor_init();
void temperature_sensor_reading();
void lux_sensor_init();
void lux_sensor_reading();
void get_power();

/****/

/** Topic **/

#define DHTTOP_TEMP_TOPIC "/dhttop/temperature"
#define DHTTOP_HUM_TOPIC "/dhttop/humidity"
#define DHTBOT_TEMP_TOPIC "/dhtbot/temperature"
#define DHTBOT_HUM_TOPIC "/dhtbot/humidity"

#define LUXTOP_TOPIC "/luxtop"
#define LUXBOT_TOPIC "/luxbot"

#define PH_TOPIC "/ph"
#define EC_TOPIC "/ec"
#define TEMP_TOPIC "/temp"

#define POWER_TOPIC "/power"
#define LOADVOLTAGE_TOPIC "/loadvoltage"
#define CURRENT_TOPIC "/current"
/****/

/** CONFIG **/

#define DHTPINTOP 6     // Digital pin connected to the DHT sensor
#define DHTPINBOTTOM 7  // Digital pin connected to the DHT sensor

#define L1 2
#define L2 3
#define L3 4
#define L4 5

#define M1_IN1 14
#define M1_IN2 15
#define M2_IN1 16
#define M2_IN2 17
#define M3_IN1 31
#define M3_IN2 33
#define M4_IN1 35
#define M4_IN2 37
#define M5_IN1 39
#define M5_IN2 41
#define M6_IN1 43
#define M6_IN2 45

// #TODO: Update pin
const int EN_PH = 25;
const int EN_EC = 27;
const int EN_RTD = 23;

/** DHT11 **/

#define DHTTYPE DHT11  // DHT 11
DHT_Unified dhttop(DHTPINTOP, DHTTYPE);
DHT_Unified dhtbottom(DHTPINBOTTOM, DHTTYPE);

/****/

Ezo_board PH = Ezo_board(99, "PH");     // create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");    // create an EC circuit object who's address is 100 and name is "EC"
Ezo_board RTD = Ezo_board(102, "RTD");  // create an RTD circuit object who's address is 102 and name is "RTD"

Ezo_board device_list[] = {  // an array of boards used for sending commands to all or specific boards
    PH,
    EC,
    RTD};

Ezo_board* default_board = &device_list[0];  // used to store the board were talking to

// gets the length of the array automatically so we dont have to change the number every time we add new boards
const uint8_t device_list_len = sizeof(device_list) / sizeof(device_list[0]);

//------For version 1.5 use these enable pins for each circuit------

//------------------------------------------------------------------

const unsigned long reading_delay = 1000;  // how long we wait to receive a response, in milliseconds

unsigned long poll_delay = 10000;  // 2000 - (reading_delay * 2) - 300;  // how long to wait between polls after accounting for the times it takes to send readings

bool polling = true;  // variable to determine whether or not were polling the circuits

void step1();  // forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();
void step4();
Sequencer4 Seq(&step1, reading_delay,  // calls the steps in sequence with time in between them
               &step2, 300,
               &step3, reading_delay,
               &step4, poll_delay);
