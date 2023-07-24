# iot

## Create `arduino_secrets.h` file

```c++

#define MQTT_SERVER "..."
#define MQTT_PORT ...
#define MQTTUSER ...
#define MQTTPASSWORD ...

```

## Use plateformio

**Current libraries and packages used**

```ini
[env:megaADK]
platform = atmelavr
board = megaADK
framework = arduino
monitor_speed = 9600
lib_deps =
	arduino-libraries/Ethernet@^2.0.1
	256dpi/MQTT @ ^2.5.1
	adafruit/DHT sensor library @ ^1.4.4
	mulmer89/EZO I2C Sensors@2.0.0+640de15
	adafruit/TSL2561 Arduino Library@^1.0.0
```
