#ifndef HYDRO_H

#include <Arduino.h>

#include "../debug/debug.h"

enum State {
  STATE_SUCCESS,
  STATE_FAIL
};

class Sensors {
 private:
  float temp = 0.0;
  float humidity = 0.0;
  int lux = 0;

 public:
  float get_temp() { return temp; }

  float get_humidity() { return humidity; }

  uint16_t get_lux() { return lux; }

  State set_temp(float _val) {
    temp = _val;
    return STATE_SUCCESS;
  }

  State set_humidity(float _val) {
    humidity = _val;
    return STATE_SUCCESS;
  }

  State set_temp_hum(float _temp, float _humidity) {
    temp = _temp;
    humidity = _humidity;
    return STATE_SUCCESS;
  }

  State set_lux(int _lux) {
    lux = _lux;
    return STATE_SUCCESS;
  }

  State serial_print() {
    D_println(" ");  // todo
    return STATE_SUCCESS;
  }
};

class Reservoir {
 private:
  float temp = 0.0;
  float ph = 0.0;
  float ec = 0.0;

 public:
  float get_temp() {
    return temp;
  }

  float get_ph() {
    return ph;
  }

  float get_ec() {
    return ec;
  }

  State set_temp(float _temp) {
    temp = _temp;
    return STATE_SUCCESS;
  }

  State set_ph(float _ph) {
    ph = _ph;
    return STATE_SUCCESS;
  }

  State set_ec(float _ec) {
    ec = _ec;
    return STATE_SUCCESS;
  }

  State set_all(float _temp, float _ph, float _ec) {
    temp = _temp;
    ph = _ph;
    ec = _ec;
    return STATE_SUCCESS;
  }

  State serial_print() {
    D_println(" ");  // todo
    return STATE_SUCCESS;
  }
};

// Main Class
class Hydro {
 private:
  // Dosing thresholds
  float PH_UPPER_THRESHOLD = 6.5;  // Upper pH threshold for pH adjustment
  float PH_LOWER_THRESHOLD = 6.0;  // Lower pH threshold for pH adjustment
  float EC_THRESHOLD = 2.0;        // EC threshold for nutrient dosing

  // Target values
  float TARGET_EC = 2.4;  // Target EC value
  float TARGET_PH = 6.2;  // Target pH value

 public:
  bool light_one = false;  // Germination
  bool light_two = false;  // Germination

  Sensors top_sensors = Sensors();
  Sensors bottom_sensors = Sensors();

  Reservoir reservoir = Reservoir();

  void auto_dose() {}  // todo

  State print() {
    top_sensors.serial_print();
    bottom_sensors.serial_print();
    reservoir.serial_print();
    return STATE_SUCCESS;
  }
};

#endif
