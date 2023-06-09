#include <SPI.h>
#include <Wire.h>

 #include <Adafruit_Sensor.h>

namespace app {
  class Hydro{
    public:

    static Hydro& instance() {
      static Hydro hydro;
      return hydro;
    }
    void setup();
    void loop();
  };
}
