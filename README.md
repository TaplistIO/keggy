# Keggy

Keggy is a simple keg monitor hardware system. It uses an ESP32 wifi device and a flow sensor. It reports readings to MQTT for onward consumption.


## What you need

Hardware

* A compatible ESP32 device. We're using an M5Stick-C, but plenty of others should.
* A hall-effect flow meter.

Software

* An MQTT server that will receive the data


## Configuration

The top of the `keggy.ino` file has a set of variables that must be overriden before you build/flash.

* `WIFI_SSID`: The name of the wifi network to connect to.
* `WIFI_PASSWORD`: The password.
* `MQTT_IP`: IP address of the MQTT broker to connect to.

The following settings can also be changed, but probably don't need to be: 

* `MQTT_PORT`: Port of the MQTT broker.
* `FLOW_METER_PIN`: Interrupt pin to use.
* `UPDATE_INTERVAL`: How often to report to MQTT.
* `MQTT_CLIENT_ID`: Client id.
* `MQTT_TOPIC_NAME`: Topic name to use.


## Building

### Install dependencies

First, be sure the following Arduino libraries are installed:

* [arduino-mqtt](https://github.com/256dpi/arduino-mqtt)

You can install these with the Arduino GUI application, or through `arduino-cli` with the following steps:

```
arduino lib install mqtt
```
