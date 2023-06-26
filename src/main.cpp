
#include "main.h"

#include <Arduino.h>

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

/** TSL2561 **/

TSL2561 tslOne(TSL2561_ADDR_LOW);
TSL2561 tslTwo(TSL2561_ADDR_FLOAT);

/****/

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// byte ip[] = {192, 168, 8, 116};  // <- change to match your network
IPAddress ip(192, 168, 8, 104);
IPAddress server(192, 168, 8, 103);

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

  // client.subscribe("/arduino");
  // client.unsubscribe("/hello");
}

void messageReceived(String& topic, String& payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void scan_devices() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    // Serial.println(address);
    delay(10);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  // delay(1000);           // wait 5 seconds for next scan
}

void setup() {
  // put your setup code here, to run once:
  DF_W5200_Init();  // Init Ethernet
  Serial.begin(9600);

  Wire.begin();  // I2C
  scan_devices();

  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
  Serial.print("server is at : ");
  Serial.println(Ethernet.localIP());

  connect();

  temperature_sensor_init();
  lux_sensor_init();

  Seq.reset();
  Serial.println("Setup Completed!\n");

  // Light
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);

  // m1
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);

  // // m2
  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT);
  pinMode(M4_IN2, OUTPUT);

  // // m3
  pinMode(M5_IN1, OUTPUT);
  pinMode(M5_IN2, OUTPUT);
  pinMode(M6_IN1, OUTPUT);
  pinMode(M6_IN2, OUTPUT);
}

void loop() {
  // light
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);

  // m1
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);

  // m2
  digitalWrite(M3_IN1, LOW);
  digitalWrite(M3_IN2, LOW);
  digitalWrite(M4_IN1, LOW);
  digitalWrite(M4_IN2, LOW);

  // m3
  digitalWrite(M5_IN1, LOW);
  digitalWrite(M5_IN2, LOW);
  digitalWrite(M6_IN1, LOW);
  digitalWrite(M6_IN2, LOW);

  // String cmd;  // variable to hold commands we send to the kit
  // put your main code here, to run repeatedly:
  client.loop();

  if (!client.connected()) {
    connect();
  }

  if (polling == true) {  // if polling is turned on, run the sequencer
    Seq.run();
    // Seq.reset();
  }

  // publish a message roughly every second.
  // if (millis() - lastMillis > 5000) {
  //   lastMillis = millis();

  //   // temperature_sensor_reading();
  //   // lux_sensor_reading();

  //   get_power();

  //   delay(1000);
  // }
}

void lux_sensor_init() {
  if (tslOne.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor");
  }

  if (tslTwo.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor");
  }

  tslOne.setGain(TSL2561_GAIN_16X);
  tslTwo.setGain(TSL2561_GAIN_16X);

  tslOne.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
  tslTwo.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
}

void temperature_sensor_init() {
  dhttop.begin();
  dhtbottom.begin();
  // Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dhttop.temperature().getSensor(&sensor);
}

void lux_sensor_reading() {
  // Serial.print("The Light value is: ");
  Serial.println(tslOne.getLuminosity(TSL2561_VISIBLE));
  client.publish(LUXTOP_TOPIC, String(tslOne.getLuminosity(TSL2561_VISIBLE)).c_str());
  Serial.println(tslTwo.getLuminosity(TSL2561_VISIBLE));
  client.publish(LUXBOT_TOPIC, String(tslTwo.getLuminosity(TSL2561_VISIBLE)).c_str());
}

void temperature_sensor_reading() {
  // Delay between measurements.
  // delay(delayMS);
  // Get temperature event and print its value.
  String str_buff;
  sensors_event_t event;
  dhttop.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Top: Error reading temperature!"));
  } else {
    Serial.print(F("Top Temperature: "));
    Serial.print(event.temperature);
    client.publish(DHTTOP_TEMP_TOPIC, String(event.temperature).c_str());
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dhttop.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Top Humidity: "));
    Serial.print(event.relative_humidity);
    client.publish(DHTTOP_HUM_TOPIC, String(event.relative_humidity).c_str());
    Serial.println(F("%"));
  }

  dhtbottom.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Bottom: Error reading temperature!"));
  } else {
    Serial.print(F("Bottom Temperature: "));
    Serial.print(event.temperature);
    client.publish(DHTBOT_TEMP_TOPIC, String(event.temperature).c_str());
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dhtbottom.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Bottom Humidity: "));
    Serial.print(event.relative_humidity);
    client.publish(DHTBOT_HUM_TOPIC, String(event.relative_humidity).c_str());
    Serial.println(F("%"));
  }
}

void step1() {
  // send a read command. we use this command instead of RTD.send_cmd("R");
  // to let the library know to parse the reading
  RTD.send_read_cmd();

  temperature_sensor_reading();
  lux_sensor_reading();
}

void step2() {
  receive_and_print_reading(RTD);  // get the reading from the RTD circuit

  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > -1000.0)) {  // if the temperature reading has been received and it is valid
    client.publish(TEMP_TOPIC, String(RTD.get_last_received_reading()).c_str());
    PH.send_cmd_with_num("T,", RTD.get_last_received_reading());
    EC.send_cmd_with_num("T,", RTD.get_last_received_reading());

  } else {                             // if the temperature reading is invalid
    PH.send_cmd_with_num("T,", 25.0);  // send default temp = 25 deg C to PH sensor
    EC.send_cmd_with_num("T,", 25.0);
  }

  Serial.print(" ");
}

void step3() {
  // send a read command. we use this command instead of PH.send_cmd("R");
  // to let the library know to parse the reading
  PH.send_read_cmd();
  EC.send_read_cmd();
}

void step4() {
  receive_and_print_reading(PH);               // get the reading from the PH circuit
  if (PH.get_error() == Ezo_board::SUCCESS) {  // if the PH reading was successful (back in step 1)
    client.publish(PH_TOPIC, String(PH.get_last_received_reading()).c_str());
  }
  Serial.print("  ");
  receive_and_print_reading(EC);               // get the reading from the EC circuit
  if (EC.get_error() == Ezo_board::SUCCESS) {  // if the EC reading was successful (back in step 1)
    client.publish(EC_TOPIC, String(EC.get_last_received_reading()).c_str());
  }

  Serial.println();
}

// FIXME: Unable to get INA219 to work
void get_power() {
  // Read voltage and current from INA219.
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();

  // Compute load voltage, power, and milliamp-hours.
  float loadvoltage = busvoltage + (shuntvoltage / 1000);
  float power_mW = loadvoltage * current_mA;

  client.publish(POWER_TOPIC, String(power_mW).c_str());
  client.publish(LOADVOLTAGE_TOPIC, String(loadvoltage).c_str());
  client.publish(CURRENT_TOPIC, String(current_mA).c_str());

  Serial.print(F("Power: "));
  Serial.print(power_mW);
  Serial.println(F("mW"));

  Serial.print(F("Load Voltage: "));
  Serial.print(loadvoltage);
  Serial.println(F("°V"));

  Serial.print(F("Current: "));
  Serial.print(current_mA);
  Serial.println(F("°mA"));

  (void)power_mW;

  // total_mA += current_mA;
  // total_sec += 1;
  // float total_mAH = total_mA / 3600.0;
  // (void)total_mAH;
}
