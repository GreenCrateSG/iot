#include "gpio_local.h"

void gpio_init() {
  // I2C
  Wire.begin();

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

  // m2
  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT);
  pinMode(M4_IN2, OUTPUT);

  // m3
  pinMode(M5_IN1, OUTPUT);
  pinMode(M5_IN2, OUTPUT);
  pinMode(M6_IN1, OUTPUT);
  pinMode(M6_IN2, OUTPUT);

  // gpio
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
}

void scan_devices() {
  byte error, address;
  int nDevices;

  Serial.println("[I2C]: Scanning...");

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
      Serial.print("[I2C]: I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("[I2C]: Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("[I2C]: No I2C devices found\n");
  else
    Serial.println("[I2C]: done\n");

  // delay(1000);           // wait 5 seconds for next scan
}
