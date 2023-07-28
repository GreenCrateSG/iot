#ifndef GPIO_LOCAL_H

#include <Arduino.h>
#include <Wire.h>

#include "../debug/debug.h"

/** CONFIG **/

#define PUMP 2
#define L3 3
#define L2 4
#define L1 5

#define M1_IN1 14
#define M1_IN2 15
#define M2_IN1 16
#define M2_IN2 17
#define M3_IN1 31
#define M3_IN2 33
#define M4_IN1 35
#define M4_IN2 37
#define M5_IN1 39
#define M5_IN2 41
#define M6_IN1 43
#define M6_IN2 45

void gpio_init();
void scan_devices();
void light_control(uint8_t _pin, bool _val);
void pump_control(bool _val);

#endif
