#ifndef CALLBACK_H
#define CALLBACK_H

#include <Arduino.h>

#include "../class/hydro.h"

void command_callback(String& subtopic, String& payload, Hydro& hydro);
void status_callback(String& subtopic, String& payload, Hydro& hydro);
void telemetry_callback(String& subtopic, String& payload, Hydro& hydro);

#endif  // CALLBACK_COMMAND_H
