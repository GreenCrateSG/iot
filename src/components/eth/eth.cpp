#include "eth.h"

#include <Arduino.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// byte ip[] = {192, 168, 8, 116};  // <- change to match your network
IPAddress ip(192, 168, 8, 104);
IPAddress dns(1, 1, 1, 1);

extern bool eth_mqtt_connected;

/**
 * @brief Initializes the W5200 Ethernet module
 *
 */
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

/**
 * @brief Initializes the Ethernet module
 *
 */
void ethernet_init() {
  DF_W5200_Init();
  Ethernet.begin(mac, ip, dns);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    D_println("Ethernet shield was not found :(");
    while (true) {  // hang
      delay(1);     // do nothing, no point running without Ethernet hardware
    }
  }

  auto link = Ethernet.linkStatus();
  D_print("Link status: ");
  switch (link) {
    case Unknown:
      D_println("Unknown");
      break;
    case LinkON:
      D_println("ON");
      break;
    case LinkOFF:
      D_println("OFF");
      break;
  }
  // if (Ethernet.linkStatus() == Unknown) {  // todo: check if this is the correct way to check for link status
  //   D_println("[ETH]: Failed to configure Ethernet");

  //   eth_mqtt_connected = false;
  // }

  delay(1000);

  D_print("[ETH]: IP Address : ");
  D_println(Ethernet.localIP());
}
