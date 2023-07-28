#include "main.h"

#include <Arduino.h>

Hydro junction_box;

EthernetClient ethClient;
MQTTClient mqttClient;

char msgBuffer[100];  // MQTT message buffer
const float epsilon = 0.000001;
/** TSL2561 **/
TSL2561 tslOne(TSL2561_ADDR_LOW);
TSL2561 tslTwo(TSL2561_ADDR_FLOAT);
/****/

/** Atlas Scientific Sensors **/
Ezo_board PH = Ezo_board(99, "PH");     // create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");    // create an EC circuit object who's address is 100 and name is "EC"
Ezo_board RTD = Ezo_board(102, "RTD");  // create an RTD circuit object who's address is 102 and name is "RTD"

Ezo_board device_list[] = {  // an array of boards used for sending commands to all or specific boards
    PH,
    EC,
    RTD};

Ezo_board* default_board = &device_list[0];  // used to store the board were talking to

const unsigned long reading_delay = 1000;  // how long we wait to receive a response, in milliseconds

// TODO: Update loop
unsigned long poll_delay = 10000;  // 2000 - (reading_delay * 2) - 300;  // how long to wait between polls after accounting for the times it takes to send readings

bool polling = true;  // variable to determine whether or not were polling the circuits

bool eth_mqtt_connected = false;  // ethernet connection status

// gets the length of the array automatically so we dont have to change the number every time we add new boards
const uint8_t device_list_len = sizeof(device_list) / sizeof(device_list[0]);

Sequencer1 Mqtt_Seq(&mqtt_loop_check, 1000);  // calls the eth and mqtt reconnect function every 10 seconds

Sequencer4 Seq(&step1, reading_delay,  // calls the steps in sequence with time in between them
               &step2, 300,
               &step3, reading_delay,
               &step4, poll_delay);

void setup() {
  gpio_init();
  D_SerialBegin(9600);
  scan_devices();

  ethernet_init();
  mqtt_init();

  // Sensor Init
  lux_sensor_init(tslOne, tslTwo);
  temperature_sensor_init();

  Mqtt_Seq.run();
  Seq.reset();

  D_println("[DEBUG]: Setup Completed!\n");
}

void loop() {
  Mqtt_Seq.run();

  if (polling == true) {  // if polling is turned on, run the sequencer
    Seq.run();
  }

  // if (millis() - lastMillis > 5000) {
  //   lastMillis = millis();

  //   lux_sensor_reading(tslOne, tslTwo);
  //   temperature_sensor_reading();

  //   delay(1000);
  // }
}

void step1() {
  // send a read command. we use this command instead of RTD.send_cmd("R");
  // to let the library know to parse the reading
  RTD.send_read_cmd();

  temperature_sensor_reading();
  lux_sensor_reading(tslOne, tslTwo);
}

void step2() {
  receive_and_print_reading(RTD);  // get the reading from the RTD circuit

  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > -1000.0)) {  // if the temperature reading has been received and it is valid
    junction_box.reservoir.set_temp(RTD.get_last_received_reading());
    if (junction_box.reservoir.get_temp() > epsilon)
      mqtt_publish(TEMP_TOPIC, String(junction_box.reservoir.get_temp()).c_str());
    // client.publish(TEMP_TOPIC, String(RTD.get_last_received_reading()).c_str());
    PH.send_cmd_with_num("T,", RTD.get_last_received_reading());
    EC.send_cmd_with_num("T,", RTD.get_last_received_reading());

  } else {                             // if the temperature reading is invalid
    PH.send_cmd_with_num("T,", 25.0);  // send default temp = 25 deg C to PH sensor
    EC.send_cmd_with_num("T,", 25.0);
  }

  D_print(" ");
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
    junction_box.reservoir.set_ph(PH.get_last_received_reading());
    if (junction_box.reservoir.get_ph() > epsilon)
      mqtt_publish(PH_TOPIC, String(junction_box.reservoir.get_ph()).c_str());
    // client.publish(PH_TOPIC, String(PH.get_last_received_reading()).c_str());
  }
  D_print("  ");
  receive_and_print_reading(EC);               // get the reading from the EC circuit
  if (EC.get_error() == Ezo_board::SUCCESS) {  // if the EC reading was successful (back in step 1)
    junction_box.reservoir.set_ec(EC.get_last_received_reading());
    if (junction_box.reservoir.get_ec() > epsilon)
      mqtt_publish(EC_TOPIC, String(junction_box.reservoir.get_ec()).c_str());
    // client.publish(EC_TOPIC, String(EC.get_last_received_reading()).c_str());
  }

  D_println();
}
