#include "gpio_local.h"

/**
 * @brief Initializes the GPIO module
 */
void gpio_init() {
  // I2C
  Wire.begin();

  // Light
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(PUMP, OUTPUT);

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

  // light
  digitalWrite(PUMP, LOW);
  digitalWrite(L3, LOW);  // FIXME: ISSUE
  digitalWrite(L2, LOW);
  digitalWrite(L1, LOW);

  // m1
  digitalWrite(M1_IN1, LOW);  // TOP
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);

  // m2
  digitalWrite(M3_IN1, LOW);  // RIGHT TOP
  digitalWrite(M3_IN2, LOW);
  digitalWrite(M4_IN1, LOW);  // RIGHT BOTTOM
  digitalWrite(M4_IN2, LOW);

  // m3
  digitalWrite(M5_IN1, LOW);
  digitalWrite(M5_IN2, LOW);
  digitalWrite(M6_IN1, LOW);
  digitalWrite(M6_IN2, LOW);
}

/**
 * @brief Scans for I2C devices
 */
void scan_devices() {
  byte error, address;
  int nDevices;

  D_println("[I2C]: Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    // D_println(address);
    delay(10);
    error = Wire.endTransmission();

    if (error == 0) {
      D_print("[I2C]: I2C device found at address 0x");
      if (address < 16)
        D_print("0");
      D_print(address, HEX);
      D_println("  !");

      nDevices++;
    } else if (error == 4) {
      D_print("[I2C]: Unknown error at address 0x");
      if (address < 16)
        D_print("0");
      D_println(address, HEX);
    }
  }
  if (nDevices == 0)
    D_println("[I2C]: No I2C devices found\n");
  else
    D_println("[I2C]: done\n");

  // delay(1000);           // wait 5 seconds for next scan
}

/**
 * @brief Controls the light
 * @param _pin Light pin
 * @param _val Light state
 */
void light_control(uint8_t _pin, bool _val) {
  if (_pin == L1 || _pin == L2 || _pin == L3) {
    digitalWrite(_pin, _val);
  } else
    D_println("[GPIO]: Invalid pin for light control");
}

void pump_control(bool _val) {
  digitalWrite(PUMP, _val);
}
