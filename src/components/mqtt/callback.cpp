#include "callback.h"

#include "mqtt_local.h"

String _str;

/**
 * @brief Callback for command
 * @param subtopic Subtopic
 * @param hydro Class Hydro
 */
void command_callback(String& subtopic, String& payload, Hydro& hydro) {
  D_println("[MQTT]: command");

  _str = String();  // clear string
  _str += subtopic;
  _str += "/";
  _str += TOPIC_CMND;

  if (subtopic == PH_TARGET_TOPIC)
    hydro.set_ph_target(payload.toFloat());
  else if (subtopic == EC_TARGET_TOPIC)
    hydro.set_ec_target(payload.toFloat());
  else if (subtopic == PH_SET_UPPER)
    hydro.set_ph_upper_threshold(payload.toFloat());
  else if (subtopic == PH_SET_LOWER)
    hydro.set_ph_lower_threshold(payload.toFloat());
  else if (subtopic == EC_SET_THRESHOLD)
    hydro.set_ec_threshold(payload.toFloat());
  else if (subtopic == PUMP_SET_TOPIC) {
    if (payload == "true")
      hydro.set_pump(true);
    else
      hydro.set_pump(false);
  } else if (subtopic == LIGHT_ONE_SET_TOPIC) {
    if (payload == "true")
      hydro.set_light_one(true);
    else
      hydro.set_light_one(false);
  } else if (subtopic == LIGHT_TWO_SET_TOPIC) {
    if (payload == "true")
      hydro.set_light_two(true);
    else
      hydro.set_light_two(false);
  } else if (subtopic == MOTOR_A_SET_TOPIC) {
    if (payload == "true")
      hydro.set_solution_a_motor(true);
    else
      hydro.set_solution_a_motor(false);
  } else if (subtopic == MOTOR_B_SET_TOPIC) {
    if (payload == "true")
      hydro.set_solution_b_motor(true);
    else
      hydro.set_solution_b_motor(false);
  } else
    mqtt_publish(_str.c_str(), "Error");
}

/**
 * @brief Callback for telemetry
 * @param subtopic Subtopic
 * @param hydro Class Hydro
 */
void telemetry_callback(String& subtopic, String& payload, Hydro& hydro) {
  D_println("[MQTT]: telemetry");

  _str = String();  // clear string
  _str += subtopic;
  _str += "/";
  _str += TOPIC_TELE;

  if (subtopic == DHTTOP_TEMP_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.top_sensors.get_temp()).c_str());
  else if (subtopic == DHTTOP_HUM_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.top_sensors.get_hum()).c_str());
  else if (subtopic == DHTBOT_TEMP_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.bottom_sensors.get_temp()).c_str());
  else if (subtopic == DHTBOT_HUM_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.bottom_sensors.get_hum()).c_str());
  else if (subtopic == LUXTOP_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.top_sensors.get_lux()).c_str());
  else if (subtopic == LUXBOT_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.bottom_sensors.get_lux()).c_str());
  else if (subtopic == PH_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.reservoir.get_ph()).c_str());
  else if (subtopic == EC_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.reservoir.get_ec()).c_str());
  else if (subtopic == TEMP_TOPIC)
    mqtt_publish(_str.c_str(), String(hydro.reservoir.get_temp()).c_str());
  else
    mqtt_publish(_str.c_str(), "Error");
}

/**
 * @brief Callback for status
 * @param subtopic Subtopic
 * @param hydro Class Hydro
 */
void status_callback(String& subtopic, String& payload, Hydro& hydro) {
  D_println("[MQTT]: status");

  _str = String("/");
  _str += TOPIC_STAT;
  subtopic.concat(_str);

  mqtt_publish(subtopic.c_str(), "OK");
}
