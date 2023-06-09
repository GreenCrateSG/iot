#include "app.h"


void setup() {
  app::Hydro::instance().setup();
}

void loop() {
  app::Hydro::instance().loop();
}
