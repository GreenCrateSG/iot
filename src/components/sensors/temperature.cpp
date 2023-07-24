#include "temperature.h"

#include "../class/hydro.h"

extern Hydro junction_box;

DHT_Unified dhttop(DHTPINTOP, DHTTYPE);
DHT_Unified dhtbottom(DHTPINBOTTOM, DHTTYPE);

void temperature_sensor_init() {
  dhttop.begin();
  dhtbottom.begin();
  // sensor_t sensor;
  // dhttop.temperature().getSensor(&sensor);
}

void temperature_sensor_reading() {
  // Delay between measurements.
  // delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dhttop.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    D_println(F("[SENSOR]: Top: Error reading temperature!"));
  } else {
    D_print(F("[SENSOR]: Top Temperature: "));
    D_print(event.temperature);
    junction_box.top_sensors.set_temp(event.temperature);
    mqtt_publish(DHTTOP_TEMP_TOPIC, String(junction_box.top_sensors.get_temp()).c_str());
    D_println(F("°C"));
  }
  // Get humidity event and print its value.
  dhttop.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    D_println(F("[SENSOR]: Error reading humidity!"));
  } else {
    D_print(F("[SENSOR]: Top Humidity: "));
    D_print(event.relative_humidity);
    junction_box.top_sensors.set_humidity(event.relative_humidity);
    mqtt_publish(DHTTOP_HUM_TOPIC, String(junction_box.top_sensors.get_humidity()).c_str());
    D_println(F("%"));
  }

  dhtbottom.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    D_println(F("[SENSOR]: Bottom: Error reading temperature!"));
  } else {
    D_print(F("[SENSOR]: Bottom Temperature: "));
    D_print(event.temperature);
    junction_box.bottom_sensors.set_temp(event.temperature);
    mqtt_publish(DHTBOT_TEMP_TOPIC, String(junction_box.bottom_sensors.get_temp()).c_str());
    D_println(F("°C"));
  }
  // Get humidity event and print its value.
  dhtbottom.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    D_println(F("[SENSOR]: Error reading humidity!"));
  } else {
    D_print(F("[SENSOR]: Bottom Humidity: "));
    D_print(event.relative_humidity);
    D_print(event.temperature);
    junction_box.bottom_sensors.set_humidity(event.relative_humidity);
    mqtt_publish(DHTBOT_HUM_TOPIC, String(junction_box.bottom_sensors.get_humidity()).c_str());
    D_println(F("%"));
  }
}
