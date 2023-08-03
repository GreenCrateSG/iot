#pragma once

#ifndef HYDRO_H
#define HYDRO_H

#include <Arduino.h>

#include "../debug/debug.h"
#include "../gpio/gpio_local.h"

/**
 * @brief Enum for state return values
 */
enum State : uint8_t {
  STATE_SUCCESS,  // 0
  STATE_FAIL
};

/**
 * @brief Class Set of Sensor
 *
 */
class Sensors {
 private:
  float temp = 0.0;
  float hum = 0.0;
  int lux = 0;

 public:
  // init
  Sensors() {
    set_temp(0.0);
    set_hum(0.0);
    set_lux(0);
  }

  Sensors(float _temp, float _hum, int _lux) {
    set_temp(_temp);
    set_hum(_hum);
    set_lux(_lux);
  }

  ~Sensors() {}

  float get_temp() { return temp; };
  float get_hum() { return hum; };
  uint16_t get_lux() { return lux; }
  State set_temp(float _val);
  State set_hum(float _val);
  State set_temp_hum(float _temp, float _hum);
  State set_lux(int _lux);
  State serial_print();
};

/**
 * @brief Class Set of Reservoir
 *
 */
class Reservoir {
 private:
  float temp = 0.0;
  float ph = 0.0;
  float ec = 0.0;

 public:
  // init
  Reservoir() {
    set_temp(0.0);
    set_ph(0.0);
    set_ec(0.0);
  }
  Reservoir(float _temp, float _ph, float _ec) {
    set_temp(_temp);
    set_ph(_ph);
    set_ec(_ec);
  }

  ~Reservoir() {}

  float get_temp() { return temp; }
  float get_ph() { return ph; }
  float get_ec() { return ec; }
  State set_temp(float _temp);
  State set_ph(float _ph);
  State set_ec(float _ec);
  State set_all(float _temp, float _ph, float _ec);
  State serial_print();
};

// Main Class
class Hydro {
 private:
  // Dosing thresholds
  float ph_upper_threshold = 6.5;  // Upper pH threshold for pH adjustment
  float ph_lower_threshold = 6.0;  // Lower pH threshold for pH adjustment
  float ec_threshold = 2.0;        // EC threshold for nutrient dosing

  // Target values
  float ec_target = 2.4;  // Target EC value
  float ph_target = 6.2;  // Target pH value

 public:
  Hydro() {}
  Hydro(float _ph_upper_threshold, float _ph_lower_threshold, float _ec_threshold, float _ec_target, float _ph_target) {
    set_ph_upper_threshold(_ph_upper_threshold);
    set_ph_lower_threshold(_ph_lower_threshold);
    set_ec_threshold(_ec_threshold);
    set_ec_target(_ec_target);
    set_ph_target(_ph_target);
  }

  ~Hydro() {}
  // bool light_one = false;  // Germination
  // bool light_two = false;  // Germination

  Sensors top_sensors = Sensors();
  Sensors bottom_sensors = Sensors();

  Reservoir reservoir = Reservoir();

  void auto_dose() {}  // todo

  // Getters
  float get_ph_upper_threshold() { return ph_upper_threshold; }
  float get_ph_lower_threshold() { return ph_lower_threshold; }
  float get_ec_threshold() { return ec_threshold; }
  float get_ec_target() { return ec_target; }
  float get_ph_target() { return ph_target; }

  // Setters
  State set_ph_upper_threshold(float _val);
  State set_ph_lower_threshold(float _val);
  State set_ec_threshold(float _val);
  State set_ec_target(float _val);
  State set_ph_target(float _val);
  void set_pump(bool _val);
  void set_light_one(bool _val);
  void set_light_two(bool _val);
  void set_solution_a_motor(bool _val);
  void set_solution_b_motor(bool _val);
  State print();
};

#endif
