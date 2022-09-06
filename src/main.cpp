#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <SparkFunTMP102.h>  // Used to send and recieve specific information from our sensor
#include <Wire.h>  // Used to establied serial communication on the I2C bus
#include <arduino_sercret.h>

/** Functions **/

void temperature_sensor_init();
void temperature_sensor_reading();

/****/

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

/****/

/**  TMP102  **/

#define ALERT_PIN A3

TMP102 sensor0;
TMP102 sensor1;
TMP102 sensor2;

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
const char* mqtt_server = MQTT_SERVER;
const char* mqttUser = MQTTUSER;
const char* mqttPassword = MQTTPASSWORD;

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
  DF_W5200_Init();  // Init Ethernet
  Serial.begin(9600);

  // client.setServer(mqtt_server, 1883);
  // client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
  Serial.print("server is at : ");
  Serial.println(Ethernet.localIP());

  // connect();

  temperature_sensor_init();
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

    temperature_sensor_reading();
  }
}

void temperature_sensor_init() {
  Wire.begin();  // Join I2C Bus

  pinMode(ALERT_PIN, INPUT);  // Declare alertPin as an input

  if (!sensor0.begin()) {
    Serial.println("Cannot connect to TMP102 0.");
    Serial.println("Is the board connected? Is the device ID correct?");
    // while (1)
    //   ;
  }

  if (!sensor1.begin(0x4A, Wire)) {
    Serial.println("Cannot connect to TMP102 1.");
    Serial.println("Is the board connected? Is the device ID correct?");
    // while (1)
    //   ;
  }

  if (!sensor2.begin(0x4B, Wire)) {
    Serial.println("Cannot connect to TMP102 2.");
    Serial.println("Is the board connected? Is the device ID correct?");
    // while (1)
    //   ;
  }

  Serial.println("Connected to TMP102!");
  delay(100);
  // sensor0.setConversionRate(0);
  // sensor0.setHighTempC(85.0);  // set T_HIGH
  // sensor0.setLowTempC(84.0);
}

void temperature_sensor_reading() {
  // TMP102

  float temperature;
  boolean alertPinState, alertRegisterState;

  // Turn sensor on to start temperature measurement.
  // Current consumtion typically ~10uA.
  sensor0.wakeup();

  // read temperature data
  temperature = sensor0.readTempC();
  // temperature = sensor0.readTempC();

  // Check for Alert
  alertPinState = digitalRead(ALERT_PIN);  // read the Alert from pin
  alertRegisterState = sensor0.alert();    // read the Alert from register

  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.sleep();

  // Print temperature and alarm state
  Serial.print("Temperature 0: ");
  Serial.println(temperature);

  sensor1.wakeup();

  // read temperature data
  temperature = sensor1.readTempC();
  // temperature = sensor0.readTempC();

  // Check for Alert
  alertPinState = digitalRead(ALERT_PIN);  // read the Alert from pin
  alertRegisterState = sensor1.alert();    // read the Alert from register

  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor1.sleep();

  // Print temperature and alarm state
  Serial.print("Temperature 1: ");
  Serial.println(temperature);

  sensor2.wakeup();

  // read temperature data
  temperature = sensor2.readTempC();
  // temperature = sensor0.readTempC();

  // Check for Alert
  alertPinState = digitalRead(ALERT_PIN);  // read the Alert from pin
  alertRegisterState = sensor2.alert();    // read the Alert from register

  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor2.sleep();

  // Print temperature and alarm state
  Serial.print("Temperature 2: ");
  Serial.println(temperature);

  //
}
