#pragma once

#include <DHT.h>
#include <DHT_U.h>
#include <Digital_Light_TSL2561.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>  // Used to establied serial communication on the I2C bus
#include <Adafruit_Sensor.h>

#include <sequencer4.h>                                          //imports a 4 function sequencer
#include <sequencer1.h>                                          //imports a 1 function sequencer
#include <Ezo_i2c_util.h>                                        //brings in common print statements
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib

//
#include "arduino_sercret.h"

/** Functions **/

void temperature_sensor_init();
void temperature_sensor_reading();
void lux_sensor_init();

/****/

Ezo_board PH = Ezo_board(99, "PH");       //create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");      //create an EC circuit object who's address is 100 and name is "EC"
Ezo_board RTD = Ezo_board(102, "RTD");    //create an RTD circuit object who's address is 102 and name is "RTD"

Ezo_board device_list[] = {               //an array of boards used for sending commands to all or specific boards
  PH,
  EC,
  RTD
};

Ezo_board* default_board = &device_list[0]; //used to store the board were talking to

//gets the length of the array automatically so we dont have to change the number every time we add new boards
const uint8_t device_list_len = sizeof(device_list) / sizeof(device_list[0]);


//------For version 1.5 use these enable pins for each circuit------
const int EN_PH = 12;
const int EN_EC = 27;
// const int EN_RTD = 15;
// const int EN_AUX = 33;
//------------------------------------------------------------------

const unsigned long reading_delay = 1000;                 //how long we wait to receive a response, in milliseconds

unsigned int poll_delay = 2000 - (reading_delay * 2) - 300; //how long to wait between polls after accounting for the times it takes to send readings

bool polling  = true;                                     //variable to determine whether or not were polling the circuits


void step1();      //forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();
void step4();
Sequencer4 Seq(&step1, reading_delay,   //calls the steps in sequence with time in between them
               &step2, 300,
               &step3, reading_delay,
               &step4, poll_delay);
