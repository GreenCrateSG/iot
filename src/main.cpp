#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <SparkFunTMP102.h>  // Used to send and recieve specific information from our sensor
#include <Wire.h>  // Used to establied serial communication on the I2C bus

/**  Ethernet  **/

#define W5200_nSCS 10
#define W5200_nRST 8
#define W5200_PWDN 9
#define W5200_nINT 3  // unused

void DF_W5200_Init(void) {
  pinMode(W5200_nSCS, OUTPUT);
  pinMode(W5200_nRST, OUTPUT);
  pinMode(W5200_PWDN, OUTPUT);
  pinMode(W5200_nINT, INPUT);     // unused
  digitalWrite(W5200_PWDN, LOW);  // Normal Mode Enable
  digitalWrite(W5200_nRST, LOW);  // Hardware reset
  delay(10);
  digitalWrite(W5200_nRST, HIGH);
  delay(200);
  Ethernet.init(W5200_nSCS);
}

/***/

/**  TMP102  **/

TMP102 sensor0;

/****/

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// byte ip[] = {192, 168, 8, 116};  // <- change to match your network
IPAddress ip(192, 168, 8, 116);
IPAddress server(192, 168, 8, 115);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

// MQTT Config
const char* mqtt_server = "192.168.8.115";
const char* mqttUser = "GreenCrateSG";
const char* mqttPassword = "GreenCrateSG";

unsigned long lastMillis = 0;

void connect() {
  Serial.print("connecting...");
  while (!client.connect("arduino", mqttUser, mqttPassword)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String& topic, String& payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}



void setup() {
  // put your setup code here, to run once:
  DF_W5200_Init(); // Init Ethernet
  Serial.begin(9600);

  // client.setServer(mqtt_server, 1883);
  // client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
  Serial.print("server is at : ");
  Serial.println(Ethernet.localIP());

  connect();

}


void loop() {
  // put your main code here, to run repeatedly:
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world.");
  }
}
