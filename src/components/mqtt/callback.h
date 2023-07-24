#ifndef CALLBACK_H

#include <Arduino.h>

#include "../class/hydro.h"

void command_callback(String subtopic, Hydro hydro);
void status_callback(String subtopic, Hydro hydro);
void telemetry_callback(String subtopic, Hydro hydro);

#endif  // CALLBACK_COMMAND_H
