#include "hydro.h"

#include <Arduino.h>

// Sensor Class
State Sensors::set_temp(float _val) {
  lux = _val;
  return STATE_SUCCESS;
}

State Sensors::set_hum(float _val) {
  hum = _val;
  return STATE_SUCCESS;
}

State Sensors::set_temp_hum(float _temp, float _hum) {
  temp = _temp;
  hum = _hum;
  return STATE_SUCCESS;
}

State Sensors::set_lux(int _lux) {
  lux = _lux;
  return STATE_SUCCESS;
}

State Sensors::serial_print() {
  D_println(" ");  // todo
  return STATE_SUCCESS;
}

// Reservoir Class
State Reservoir::set_temp(float _temp) {
  temp = _temp;
  return STATE_SUCCESS;
}

State Reservoir::set_ph(float _ph) {
  ph = _ph;
  return STATE_SUCCESS;
}

State Reservoir::set_ec(float _ec) {
  ec = _ec;
  return STATE_SUCCESS;
}

State Reservoir::set_all(float _temp, float _ph, float _ec) {
  temp = _temp;
  ph = _ph;
  ec = _ec;
  return STATE_SUCCESS;
}

State Reservoir::serial_print() {
  D_println(" ");  // todo
  return STATE_SUCCESS;
}

// Hydro Class
State Hydro::set_ph_upper_threshold(float _val) {
  ph_upper_threshold = _val;
  return STATE_SUCCESS;
}

State Hydro::set_ph_lower_threshold(float _val) {
  ph_lower_threshold = _val;
  return STATE_SUCCESS;
}

State Hydro::set_ec_threshold(float _val) {
  ec_threshold = _val;
  return STATE_SUCCESS;
}

State Hydro::set_ec_target(float _val) {
  ec_target = _val;
  return STATE_SUCCESS;
}

State Hydro::set_ph_target(float _val) {
  ph_target = _val;
  return STATE_SUCCESS;
}

void Hydro::set_pump(bool _val) {
  pump_control(_val);
}

void Hydro::set_light_one(bool _val) {
  light_control(L1, _val);
}

void Hydro::set_light_two(bool _val) {
  light_control(L2, _val);
}

State Hydro::print() {
  top_sensors.serial_print();
  bottom_sensors.serial_print();
  reservoir.serial_print();
  return STATE_SUCCESS;
}
