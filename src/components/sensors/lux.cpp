#include "lux.h"

/**
 * @brief Initialisation of luxs sensors
 * @param tslOne Pass top tsl2561 obj
 * @param tslTwo Pass bottom tsl2561 obj
 */
void lux_sensor_init(TSL2561 tslOne, TSL2561 tslTwo) {
  if (tslOne.begin()) {
    D_println("[SENSOR]: Found Lux (tslOne) sensor");
  } else {
    D_println("[SENSOR]: No Lux (tslOne) sensor");
  }

  if (tslTwo.begin()) {
    D_println("[SENSOR]: Found Lux (tslTwo) sensor");
  } else {
    D_println("[SENSOR]: No Lux (tslTwo) sensor");
  }

  tslOne.setGain(TSL2561_GAIN_16X);
  tslTwo.setGain(TSL2561_GAIN_16X);

  tslOne.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
  tslTwo.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
}

/**
 * @brief Get reading from luxs sensors
 * @param tslOne
 * @param tslTwo
 */
void lux_sensor_reading(TSL2561 tslOne, TSL2561 tslTwo, Hydro& _junction_box) {
  D_print("[SENSOR]: ");
  D_println(tslOne.getLuminosity(TSL2561_VISIBLE));
  _junction_box.top_sensors.set_lux(tslOne.getLuminosity(TSL2561_VISIBLE));
  if (_junction_box.top_sensors.get_lux() > 0)
    mqtt_publish(LUXTOP_TOPIC, String(_junction_box.top_sensors.get_lux()).c_str());

  D_print("[SENSOR]: ");
  D_println(tslTwo.getLuminosity(TSL2561_VISIBLE));
  _junction_box.bottom_sensors.set_lux(tslTwo.getLuminosity(TSL2561_VISIBLE));
  if (_junction_box.bottom_sensors.get_lux() > 0)
    mqtt_publish(LUXBOT_TOPIC, String(_junction_box.bottom_sensors.get_lux()).c_str());
}
