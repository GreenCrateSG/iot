#include "mqtt_local.h"

#include <Arduino.h>

#include "../eth/eth.h"
#include "callback.h"

// global (old)
// extern Hydro junction_box;

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
extern bool eth_mqtt_connected;

int _counter = 10;

void mqtt_callback(String& topic, String& payload, Hydro& junction_box) {
  // handle message arrived
  D_println("[MQTT]: incoming: " + topic + " - " + payload);
  topic.remove(0, strlen(TOPIC_API) - 1);  // remove the first character of the topic ("arduino/"

  // Check if the received topic starts with the base topic "arduino"
  if (String(topic).startsWith(TOPIC_CMND)) {
    // Extract the subtopic after the base topic
    String subtopic = String(topic).substring(strlen(TOPIC_CMND) + 1);
    command_callback(subtopic, payload, junction_box);
  } else if (String(topic).startsWith(TOPIC_STAT)) {
    String subtopic = String(topic).substring(strlen(TOPIC_STAT) + 1);
    status_callback(subtopic, payload, junction_box);
  } else if (String(topic).startsWith(TOPIC_TELE)) {
    String subtopic = String(topic).substring(strlen(TOPIC_TELE) + 1);
    telemetry_callback(subtopic, payload, junction_box);
  }
}

void mqtt_connect() {
  D_print("[MQTT]: connecting...");
  while (!mqttClient.connect("arduino", mqttUser, mqttPassword) && _counter-- > 0) {
    D_print(".");
    delay(1000);
  }

  if (_counter <= 0) {
    D_println("\n[MQTT]: connection failed!");
    eth_mqtt_connected = false;
    _counter = 10;  // reset counter

    ethClient.stop();
    delay(1000);
    ethernet_init();

    return;
  } else {
    eth_mqtt_connected = true;
    mqttClient.subscribe(TOPIC_API, QoS_2);
    D_println("\n[MQTT]: connected!");
  }

  // client.unsubscribe("/hello");
}

void mqtt_init() {
  mqttClient.begin(mqtt_server, port, ethClient);
  mqttClient.onMessage(mqtt_callback);
  delay(1000);
  mqtt_connect();
}

void mqtt_loop_check() {
  // todo: pump issue
  // D_println("[MQTT]: Loop Check");

  // if (eth_mqtt_connected) {  // check if connected
  // ethClient.loop();
  mqttClient.loop();

  if (!mqttClient.connected()) {
    mqtt_connect();
  }
  // }
}

void mqtt_publish(const char* topic, const char* payload) {
  sprintf(msgBuffer, payload);

  if (eth_mqtt_connected)  // send only if connected
    mqttClient.publish(topic, payload, noRetain, QoS_2);
}
