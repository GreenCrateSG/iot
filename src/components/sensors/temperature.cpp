#include "temperature.h"

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
    Serial.println(F("[SENSOR]: Top: Error reading temperature!"));
  } else {
    Serial.print(F("[SENSOR]: Top Temperature: "));
    Serial.print(event.temperature);
    mqtt_publish(DHTTOP_TEMP_TOPIC, String(event.temperature).c_str());
    // client.publish(DHTTOP_TEMP_TOPIC, String(event.temperature).c_str());
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dhttop.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("[SENSOR]: Error reading humidity!"));
  } else {
    Serial.print(F("[SENSOR]: Top Humidity: "));
    Serial.print(event.relative_humidity);
    mqtt_publish(DHTTOP_HUM_TOPIC, String(event.relative_humidity).c_str());
    // client.publish(DHTTOP_HUM_TOPIC, String(event.relative_humidity).c_str());
    Serial.println(F("%"));
  }

  dhtbottom.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("[SENSOR]: Bottom: Error reading temperature!"));
  } else {
    Serial.print(F("[SENSOR]: Bottom Temperature: "));
    Serial.print(event.temperature);
    mqtt_publish(DHTBOT_TEMP_TOPIC, String(event.temperature).c_str());
    // client.publish(DHTBOT_TEMP_TOPIC, String(event.temperature).c_str());
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dhtbottom.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("[SENSOR]: Error reading humidity!"));
  } else {
    Serial.print(F("[SENSOR]: Bottom Humidity: "));
    Serial.print(event.relative_humidity);
    mqtt_publish(DHTBOT_HUM_TOPIC, String(event.relative_humidity).c_str());
    // client.publish(DHTBOT_HUM_TOPIC, String(event.relative_humidity).c_str());
    Serial.println(F("%"));
  }
}
