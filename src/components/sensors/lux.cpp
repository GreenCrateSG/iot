#include "lux.h"

void lux_sensor_init(TSL2561 tslOne, TSL2561 tslTwo) {
  if (tslOne.begin()) {
    Serial.println("[SENSOR]: Found Lux (tslOne) sensor");
  } else {
    Serial.println("[SENSOR]: No Lux (tslOne) sensor");
  }

  if (tslTwo.begin()) {
    Serial.println("[SENSOR]: Found Lux (tslTwo) sensor");
  } else {
    Serial.println("[SENSOR]: No Lux (tslTwo) sensor");
  }

  tslOne.setGain(TSL2561_GAIN_16X);
  tslTwo.setGain(TSL2561_GAIN_16X);

  tslOne.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
  tslTwo.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
}

void lux_sensor_reading(TSL2561 tslOne, TSL2561 tslTwo) {
  Serial.print("[SENSOR]: ");
  Serial.println(tslOne.getLuminosity(TSL2561_VISIBLE));

  mqtt_publish(LUXTOP_TOPIC, String(tslOne.getLuminosity(TSL2561_VISIBLE)).c_str());

  Serial.print("[SENSOR]: ");
  Serial.println(tslTwo.getLuminosity(TSL2561_VISIBLE));
  mqtt_publish(LUXBOT_TOPIC, String(tslTwo.getLuminosity(TSL2561_VISIBLE)).c_str());
}
