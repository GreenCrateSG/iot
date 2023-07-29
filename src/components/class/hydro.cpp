#include "hydro.h"

#include <Arduino.h>

// Sensor Class
State Sensors::set_temp(float _val) {
  lux = _val;
  return STATE_SUCCESS;
}

State Sensors::set_hum(float _val) {
  humidity = _val;
  return STATE_SUCCESS;
}

State Sensors::set_temp_hum(float _temp, float _humidity) {
  temp = _temp;
  humidity = _humidity;
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
