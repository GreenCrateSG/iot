#ifndef ETH_H
#define ETH_H

#include <Arduino.h>
#include <Ethernet.h>
#include <MQTT.h>
#include <Wire.h>

#include "../debug/debug.h"

/**  Ethernet  **/
#define W5200_nSCS 10
#define W5200_nRST 8
#define W5200_PWDN 9
#define W5200_nINT 3  // unused

void DF_W5200_Init(void);
void ethernet_init();

#endif
