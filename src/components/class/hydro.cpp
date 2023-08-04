#include "hydro.h"

#include <Arduino.h>

// Sensor Class

/**
 * @brief set temperature
 * @param _val
 * @return
 */
State Sensors::set_temp(float _val) {
  temp = _val;
  return STATE_SUCCESS;
}

/**
 * @brief set humidity
 * @param _val
 * @return
 */
State Sensors::set_hum(float _val) {
  hum = _val;
  return STATE_SUCCESS;
}

/**
 * @brief set both temperature & humidity
 * @param _temp
 * @param _hum
 * @return
 */
State Sensors::set_temp_hum(float _temp, float _hum) {
  temp = _temp;
  hum = _hum;
  return STATE_SUCCESS;
}

/**
 * @brief set lux
 * @param _lux
 * @return
 */
State Sensors::set_lux(int _lux) {
  lux = _lux;
  return STATE_SUCCESS;
}

/**
 * @brief serial print
 * @return
 */
State Sensors::serial_print() {
  D_println(" ");  // todo
  return STATE_SUCCESS;
}

// Reservoir Class

/**
 * @brief set reservior temperature
 * @param _temp
 * @return
 */
State Reservoir::set_temp(float _temp) {
  temp = _temp;
  return STATE_SUCCESS;
}

/**
 * @brief set reservior ph value
 * @param _ph
 * @return
 */
State Reservoir::set_ph(float _ph) {
  ph = _ph;
  return STATE_SUCCESS;
}

/**
 * @brief set reservior ec value
 * @param _ec
 * @return
 */
State Reservoir::set_ec(float _ec) {
  ec = _ec;
  return STATE_SUCCESS;
}

/**
 * @brief set reservior temp, ph & ec value
 * @param _temp
 * @param _ph
 * @param _ec
 * @return
 */
State Reservoir::set_all(float _temp, float _ph, float _ec) {
  temp = _temp;
  ph = _ph;
  ec = _ec;
  return STATE_SUCCESS;
}

/**
 * @brief serial print
 * @return
 */
State Reservoir::serial_print() {
  D_println(" ");  // todo
  return STATE_SUCCESS;
}

// Hydro Class

/**
 * @brief set ph upper threshold
 * @param _val
 * @return
 */
State Hydro::set_ph_upper_threshold(float _val) {
  ph_upper_threshold = _val;
  return STATE_SUCCESS;
}

/**
 * @brief set ph lower threshold
 * @param _val
 * @return
 */
State Hydro::set_ph_lower_threshold(float _val) {
  ph_lower_threshold = _val;
  return STATE_SUCCESS;
}

/**
 * @brief set ec threshol
 * @param _val
 * @return
 */
State Hydro::set_ec_threshold(float _val) {
  ec_threshold = _val;
  return STATE_SUCCESS;
}

/**
 * @brief set ec target
 * @param _val
 * @return
 */
State Hydro::set_ec_target(float _val) {
  ec_target = _val;
  return STATE_SUCCESS;
}

/**
 * @brief set ph target
 * @param _val
 * @return
 */
State Hydro::set_ph_target(float _val) {
  ph_target = _val;
  return STATE_SUCCESS;
}

/**
 * @brief control Pump
 * @param _val
 */
void Hydro::set_pump(bool _val) {
  pump_control(_val);
}

/**
 * @brief control light one
 * @param _val
 */
void Hydro::set_light_one(bool _val) {
  light_control(L1, _val);
}

/**
 * @brief control light two
 * @param _val
 */
void Hydro::set_light_two(bool _val) {
  light_control(L2, _val);
}

/**
 * @brief control solution a motor
 * @param _val
 */
void Hydro::set_solution_a_motor(bool _val) {
  motor_control(M1_IN1, M1_IN2, _val);
}

/**
 * @brief control solution b motor
 * @param _val
 */
void Hydro::set_solution_b_motor(bool _val) {
  motor_control(M3_IN1, M3_IN2, _val);
}

/**
 * @brief serial print
 * @return
 */
State Hydro::print() {
  top_sensors.serial_print();
  bottom_sensors.serial_print();
  reservoir.serial_print();
  return STATE_SUCCESS;
}


State Hydro::auto_nutrition_dose()
{
  
}
