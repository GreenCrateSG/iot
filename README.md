# iot

## Needing `ardunio_sercret.h`

```

#define SECRET_SSID "" // Don't need
#define SECRET_PASS "" // Don't need

#define MQTT_SERVER "..."
#define MQTTUSER "..."
#define MQTTPASSWORD "..."

```

## Remote Dev

```

python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
sudo apt -y install rustc libffi-dev
echo "export PATH=\$PATH:/home/${USER}/.platformio/penv/bin" >> ~/.profile

```
