#ifndef MQTT_LOCAL_H

#include <Arduino.h>
#include <Ethernet.h>
#include <MQTT.h>

#include "../../arduino_sercret.h"
#include "../class/hydro.h"
#include "../debug/debug.h"

/** Topic **/

// API
#define TOPIC_API "arduino/#"  // <- any topic related to arduino
#define TOPIC_CMND "cmnd"      // command
#define TOPIC_STAT "stat"      // status
#define TOPIC_TELE "tele"      // telemetry

#define DHTTOP_TEMP_TOPIC "sensor/dhttop/temperature"
#define DHTTOP_HUM_TOPIC "sensor/dhttop/humidity"
#define DHTBOT_TEMP_TOPIC "sensor/dhtbot/temperature"
#define DHTBOT_HUM_TOPIC "sensor/dhtbot/humidity"

#define LUXTOP_TOPIC "sensor/luxtop"
#define LUXBOT_TOPIC "sensor/luxbot"

#define PH_TOPIC "sensor/ph"
#define EC_TOPIC "sensor/ec"
#define TEMP_TOPIC "sensor/temp"

#define POWER_TOPIC "sensor/power"
#define LOADVOLTAGE_TOPIC "sensor/loadvoltage"
#define CURRENT_TOPIC "sensor/current"

#define PH_TARGET_TOPIC "set/ph_target"
#define EC_TARGET_TOPIC "set/ec_target"
#define PH_SET_UPPER "set/ph_upper"
#define PH_SET_LOWER "set/ph_lower"
#define EC_SET_THRESHOLD "set/ec_threshold"

#define PUMP_SET_TOPIC "set/pump"
#define LIGHT_ONE_SET_TOPIC "set/light_one"
#define LIGHT_TWO_SET_TOPIC "set/light_two"

/****/

void mqtt_init();
void mqtt_callback(String& topic, String& payload, Hydro& junction_box);
void mqtt_connect();
void mqtt_loop_check();
void mqtt_publish(const char* topic, const char* payload);

#endif
