#include "main.h"


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

// #define ALERT_PIN A3

// TMP102 sensor0;
// TMP102 sensor1;
// TMP102 sensor2;

/****/

/** TSL2561 **/

Adafruit_TSL2561_Unified tsltop =
    Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);
Adafruit_TSL2561_Unified tslbottom =
    Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

/****/

/** DHT11 **/

#define DHTPINTOP 2     // Digital pin connected to the DHT sensor
#define DHTPINBOTTOM 3  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT_Unified dhttop(DHTPINTOP, DHTTYPE);
DHT_Unified dhtbottom(DHTPINBOTTOM, DHTTYPE);

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



void send_float_to_broker(const char* topic, String payload, float _v, int placement)
{
  String payload_data;
  char packet[10];

  payload_data = String(_v, placement);
  payload_data.toCharArray(packet, payload_data.length() + 1);
  client.publish(topic, packet);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("arduino", mqttUser, mqttPassword)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  // TODO: ALL TOPIC
  client.subscribe(TOPIC TOP TEMPERATURE);
  client.subscribe(TOPIC TOP HUMIDITY);
  client.subscribe(TOPIC TOP LUX);
  // client.unsubscribe("/hello");
  client.subscribe(TOPIC BOTTOM TEMPERATURE);
  client.subscribe(TOPIC BOTTOM HUMIDITY);
  client.subscribe(TOPIC BOTTOM LUX);
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

  Serial.begin(9600);

  light_init();
  motor_init();

  DF_W5200_Init();  // Init Ethernet
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
  Serial.print("server is at : ");
  Serial.println(Ethernet.localIP());

  connect();

  temperature_sensor_init();
  lux_sensor_init();

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

    temperature_sensor_reading();

    lux_sensor_get_reading();
    delay(1000);

  }
}

void lux_sensor_init() {
  Wire.begin();

  if (!tsltop.begin()) {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print(
        "Ooops, no TSL2561 TOP detected ... Check your wiring or I2C ADDR!");
    // while (1);
  }

  if (!tslbottom.begin()) {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print(
        "Ooops, no TSL2561 BOTTOM detected ... Check your wiring or I2C ADDR!");
    // while (1);
  }

  tslbottom.enableAutoRange(
      true); /* Auto-gain ... switches automatically between 1x and 16x */

  tslbottom.setIntegrationTime(
      TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but
// slowest conversions */

  tsltop.enableAutoRange(
      true); /* Auto-gain ... switches automatically between 1x and 16x */

  tsltop.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
}

void lux_sensor_get_reading() {
  sensors_event_t event;
  tslbottom.getEvent(&event);

  /* Display the results (light is measured in lux) */
  if (event.light) {
    // Serial.print(event.light);
    // Serial.println(" lux");
    send_float_to_broker(TOPIC BOTTOM LUX, "", event.light, 3);
  } else {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload [bottom]");
  }

  tsltop.getEvent(&event);

  /* Display the results (light is measured in lux) */
  if (event.light) {
    // Serial.print(event.light);
    // Serial.println(" lux");
    send_float_to_broker(TOPIC TOP LUX, "", event.light, 3);
  } else {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload [top]");
  }
}

void temperature_sensor_init() {
  dhttop.begin();
  dhtbottom.begin();
  // Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dhttop.temperature().getSensor(&sensor);
  // Serial.println(F("------------------------------------"));
  // Serial.println(F("Temperature Sensor"));
  // Serial.print(F("Sensor Type: "));
  // Serial.println(sensor.name);
  // Serial.print(F("Driver Ver:  "));
  // Serial.println(sensor.version);
  // Serial.print(F("Unique ID:   "));
  // Serial.println(sensor.sensor_id);
  // Serial.print(F("Max Value:   "));
  // Serial.print(sensor.max_value);
  // Serial.println(F("°C"));
  // Serial.print(F("Min Value:   "));
  // Serial.print(sensor.min_value);
  // Serial.println(F("°C"));
  // Serial.print(F("Resolution:  "));
  // Serial.print(sensor.resolution);
  // Serial.println(F("°C"));
  // Serial.println(F("------------------------------------"));
  // // Print humidity sensor details.
  // dht.humidity().getSensor(&sensor);
  // Serial.println(F("Humidity Sensor"));
  // Serial.print(F("Sensor Type: "));
  // Serial.println(sensor.name);
  // Serial.print(F("Driver Ver:  "));
  // Serial.println(sensor.version);
  // Serial.print(F("Unique ID:   "));
  // Serial.println(sensor.sensor_id);
  // Serial.print(F("Max Value:   "));
  // Serial.print(sensor.max_value);
  // Serial.println(F("%"));
  // Serial.print(F("Min Value:   "));
  // Serial.print(sensor.min_value);
  // Serial.println(F("%"));
  // Serial.print(F("Resolution:  "));
  // Serial.print(sensor.resolution);
  // Serial.println(F("%"));
  // Serial.println(F("------------------------------------"));

  // Set delay between sensor readings based on sensor details.

  //   Wire.begin();  // Join I2C Bus

  //   pinMode(ALERT_PIN, INPUT);  // Declare alertPin as an input

  //   if (!sensor0.begin()) {
  //     Serial.println("Cannot connect to TMP102 0.");
  //     Serial.println("Is the board connected? Is the device ID correct?");
  //     // while (1)
  //     //   ;
  //   }

  //   if (!sensor1.begin(0x4A, Wire)) {
  //     Serial.println("Cannot connect to TMP102 1.");
  //     Serial.println("Is the board connected? Is the device ID correct?");
  //     // while (1)
  //     //   ;
  //   }

  //   if (!sensor2.begin(0x4B, Wire)) {
  //     Serial.println("Cannot connect to TMP102 2.");
  //     Serial.println("Is the board connected? Is the device ID correct?");
  //     // while (1)
  //     //   ;
  //   }

  //   Serial.println("Connected to TMP102!");
  //   delay(100);
  //   // sensor0.setConversionRate(0);
  //   // sensor0.setHighTempC(85.0);  // set T_HIGH
  //   // sensor0.setLowTempC(84.0);
}

void temperature_sensor_reading() {
  // Delay between measurements.
  // delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dhttop.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    // Serial.print(F("Top Temperature: "));
    // Serial.print(event.temperature);
    // Serial.println(F("°C"));
    // payload_data = String(event.temperature, 4);
    // payload_data.toCharArray(packet, payload_data.length() + 1);
    // client.publish(TOPIC TOP TEMPERATURE, packet);
    send_float_to_broker(TOPIC TOP TEMPERATURE, "", event.temperature, 3);
  }
  // Get humidity event and print its value.
  dhttop.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    // Serial.print(F("Top Humidity: "));
    // Serial.print(event.relative_humidity);
    // Serial.println(F("%"));
    send_float_to_broker(TOPIC TOP HUMIDITY, "", event.relative_humidity, 3);
  }

  dhtbottom.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    // Serial.print(F("Bottom Temperature: "));
    // Serial.print(event.temperature);
    // Serial.println(F("°C"));

    send_float_to_broker(TOPIC BOTTOM TEMPERATURE, "", event.temperature, 3);
  }
  // Get humidity event and print its value.
  dhtbottom.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    // Serial.print(F("Bottom Humidity: "));
    // Serial.print(event.relative_humidity);
    // Serial.println(F("%"));
    send_float_to_broker(TOPIC BOTTOM HUMIDITY, "", event.relative_humidity, 3);
  }

  // // TMP102

  // float temperature;
  // boolean alertPinState, alertRegisterState;

  // // Turn sensor on to start temperature measurement.
  // // Current consumtion typically ~10uA.
  // sensor0.wakeup();

  // // read temperature data
  // temperature = sensor0.readTempC();
  // // temperature = sensor0.readTempC();

  // // Check for Alert
  // alertPinState = digitalRead(ALERT_PIN);  // read the Alert from pin
  // alertRegisterState = sensor0.alert();    // read the Alert from register

  // // Place sensor in sleep mode to save power.
  // // Current consumtion typically <0.5uA.
  // sensor0.sleep();

  // // Print temperature and alarm state
  // Serial.print("Temperature 0: ");
  // Serial.println(temperature);

  // sensor1.wakeup();

  // // read temperature data
  // temperature = sensor1.readTempC();
  // // temperature = sensor0.readTempC();

  // // Check for Alert
  // alertPinState = digitalRead(ALERT_PIN);  // read the Alert from pin
  // alertRegisterState = sensor1.alert();    // read the Alert from register

  // // Place sensor in sleep mode to save power.
  // // Current consumtion typically <0.5uA.
  // sensor1.sleep();

  // // Print temperature and alarm state
  // Serial.print("Temperature 1: ");
  // Serial.println(temperature);

  // sensor2.wakeup();

  // // read temperature data
  // temperature = sensor2.readTempC();
  // // temperature = sensor0.readTempC();

  // // Check for Alert
  // alertPinState = digitalRead(ALERT_PIN);  // read the Alert from pin
  // alertRegisterState = sensor2.alert();    // read the Alert from register

  // // Place sensor in sleep mode to save power.
  // // Current consumtion typically <0.5uA.
  // sensor2.sleep();

  // // Print temperature and alarm state
  // Serial.print("Temperature 2: ");
  // Serial.println(temperature);

  //

}

void light_init() {
  // Serial.println("\nStart light config");

  pinMode(LIGHT_ONE_PIN, OUTPUT);
  pinMode(LIGHT_TWO_PIN, OUTPUT);
  pinMode(LIGHT_THREE_PIN, OUTPUT);
  pinMode(LIGHT_FOUR_PIN, OUTPUT);

  digitalWrite(LIGHT_ONE_PIN, LOW);
  digitalWrite(LIGHT_TWO_PIN, LOW);
  digitalWrite(LIGHT_THREE_PIN, LOW);
  digitalWrite(LIGHT_FOUR_PIN, LOW);

  // Serial.println("End light config");
}

void motor_init()
{
  pinMode(MOTOR_ONE_CW_PIN, OUTPUT);
  pinMode(MOTOR_ONE_ACW_PIN, OUTPUT);
  pinMode(MOTOR_TWO_CW_PIN, OUTPUT);
  pinMode(MOTOR_TWO_ACW_PIN, OUTPUT);

  digitalWrite(MOTOR_ONE_CW_PIN, LOW);
  digitalWrite(MOTOR_ONE_ACW_PIN, LOW);

  digitalWrite(MOTOR_TWO_CW_PIN, LOW);
  digitalWrite(MOTOR_TWO_ACW_PIN, LOW);
}
