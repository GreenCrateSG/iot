#include "mqtt_local.h"

#include <Arduino.h>

// MQTT Config
const char* mqtt_server = MQTT_SERVER;
const char* mqttUser = MQTTUSER;
const char* mqttPassword = MQTTPASSWORD;
int port = MQTT_PORT;
int QoS_2 = 2;
bool noRetain = false;

extern EthernetClient ethClient;
extern MQTTClient mqttClient;
extern char msgBuffer[100];  // MQTT message buffer

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void mqtt_connect() {
  Serial.print("[MQTT]: connecting...");
  while (!mqttClient.connect("arduino", mqttUser, mqttPassword)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n[MQTT]: connected!");

  // client.subscribe("/arduino");
  // client.unsubscribe("/hello");
}

void mqtt_messageReceived(String& topic, String& payload) {
  Serial.println("[MQTT]: incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void mqtt_init() {
  mqttClient.begin(mqtt_server, port, ethClient);
  mqttClient.onMessage(mqtt_messageReceived);
  delay(1000);
  mqtt_connect();
}

void mqtt_loop_check() {
  // Serial.println("[MQTT]: Loop Check");

  mqttClient.loop();

  if (!mqttClient.connected()) {
    mqtt_connect();
  }
}

void mqtt_publish(const char* topic, const char* payload) {
  sprintf(msgBuffer, payload);
  mqttClient.publish(topic, payload, noRetain, QoS_2);
}
