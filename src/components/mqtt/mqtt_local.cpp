#include "mqtt_local.h"

#include <Arduino.h>

#include "callback.h"

extern Hydro junction_box;

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

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived

  D_println("[MQTT]: callback");

  // Check if the received topic starts with the base topic "hydroponics"
  if (String(topic).startsWith(TOPIC_CMND)) {
    // Extract the subtopic after the base topic
    String subtopic = String(topic).substring(strlen(TOPIC_CMND));
    command_callback(subtopic.c_str(), junction_box);
  } else if (String(topic).startsWith(TOPIC_STAT)) {
    String subtopic = String(topic).substring(strlen(TOPIC_STAT));
    status_callback(subtopic.c_str(), junction_box);
  } else if (String(topic).startsWith(TOPIC_TELE)) {
    String subtopic = String(topic).substring(strlen(TOPIC_TELE));
    telemetry_callback(subtopic.c_str(), junction_box);
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
    return;
  } else {
    eth_mqtt_connected = true;
    D_println("\n[MQTT]: connected!");
  }

  // client.subscribe("/arduino");
  // client.unsubscribe("/hello");
}

void mqtt_messageReceived(String& topic, String& payload) {
  D_println("[MQTT]: incoming: " + topic + " - " + payload);

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
  // D_println("[MQTT]: Loop Check");

  if (eth_mqtt_connected) {  // check if connected
    mqttClient.loop();

    if (!mqttClient.connected()) {
      mqtt_connect();
    }
  }
}

void mqtt_publish(const char* topic, const char* payload) {
  sprintf(msgBuffer, payload);

  if (eth_mqtt_connected)  // send only if connected
    mqttClient.publish(topic, payload, noRetain, QoS_2);
}
