# Rancilio Silvia PID

Yet another implementation of a PID controller for the Rancilio Silvia Coffee Machine. Using an ESP32 (ESP8266 should work as well) with a solid state relay and a TSIC 306 temperature probe the boiler temperature can be controller within one degree celsius.

The coffee machine connects to a MQTT broker to publish sensor data. Control parameters for the pid loop can also be set via MQTT topics. These topics can be used to integrate the coffee maker into `home assistant`.

The firmware can be updated via the OTA endpoint `/update`.

## Table of Contents

- [Rancilio Silvia PID](#rancilio-silvia-pid)
  - [Table of Contents](#table-of-contents)
  - [Hardware](#hardware)
    - [ESP 32 NodeMCU Dev Kit C](#esp-32-nodemcu-dev-kit-c)
    - [TSIC 306 Temperature Probes](#tsic-306-temperature-probes)
    - [Solid State Relay](#solid-state-relay)
  - [Setup](#setup)
  - [OTAU](#otau)
  - [MQTT Topics](#mqtt-topics)
    - [Availability Topics](#availability-topics)
    - [Sensor Topics](#sensor-topics)
    - [Control Topics](#control-topics)
      - [Target](#target)
      - [Coldstart](#coldstart)
      - [Window](#window)
      - [Cycle](#cycle)
      - [P Value](#p-value)
      - [I Value](#i-value)
      - [D Value](#d-value)
  - [Contributing](#contributing)
  - [License](#license)

## Hardware

### ESP 32 NodeMCU Dev Kit C

This is the microcontroller running the code. Other variants of the ESP32 and the ESP8266 should work as well, but I have not tested this.

### TSIC 306 Temperature Probes

These temperature probes provide accurate measurements up to 150°C and are used to measure the boiler and grouphead temperature. The connected pins can be configured in the `Pins.h` header file.

### Solid State Relay

The relay is used to switch the heating coil of the boiler on and off. A solid state relay is chosen to provide fast switching without wear and click noises. The heating element can draw up to 1.3 kW which must be compatible with the relay specs. The connected pins can be configured in the `Pins.h` header file.

## Setup

Copy the File `Credentials.h.sample` and rename it to `Credentials.h` and set appropriate values.

Do the same for `MqttConfig.h.sample`. Explanation for the topics is provided below in the section `MQTT Topics`.

## OTAU

The firmware of the coffee machine can be updated via OTAU using the `/update` endpoint. The hostname of the device can be cofigured with the `HOSTNAME` setting.   

## MQTT Topics

MQTT is used to communicate with the coffee machine. The topics are configured in the `MqttConfig.h`. The Client Id of the device it defined by the `MQTT_CLIENTID` setting.

### Availability Topics

`MQTT_STATE_TOPIC`

This topic can be used to check if the coffee machine is turned on by checking for the message `on` which will be published continously according to the `MQTT_HEARTBEAT_INTERVAL_MS` setting.

### Sensor Topics

These values are published by the coffee machine when they change their values. Max rate is configured by the `MQTT_PUBLISH_INTERVAL_MS` setting. 

| Entity                | Setting           | Value                         |
|-----------------------|-------------------|-------------------------------|
| Boiler Temperature    | MQTT_BOILER_TOPIC | double / Temperature          |
| Grouphead Temperature | MQTT_HEAD_TOPIC   | double / Temperature          |
| PID Output            | MQTT_PID_TOPIC    | double / Control Output (0-1) |
| Heater state          | MQTT_HEATER_TOPIC | binary / "on" or "off"        |

### Control Topics

These values define the control loop of the boiler heater. These values have read/write topics to set values and retrieve them. 

Publish means the coffee machine will publish these topics.

Subscribe means the coffee machine will subscribe to these topics.

#### Target

Defines the target temperature of the boiler.

| Publish   | Subscribe  |
|-----------|---|
| MQTT_TARGET_PUB_TOPIC | MQTT_TARGET_SUB_TOPIC  |

#### Coldstart

Defines the boiler temperature until the heater will be constantly turned on to heat up faster in the beginning.

| Publish   | Subscribe  |
|-----------|---|
| MQTT_COLDSTART_PUB_TOPIC | MQTT_COLDSTART_SUB_TOPIC  |

#### Window

After the boiler reaches the coldstart temperature, the heating element will be controlled based on time slices of this length in milliseconds

| Publish   | Subscribe  |
|-----------|---|
| MQTT_WINDOW_PUB_TOPIC | MQTT_WINDOW_SUB_TOPIC  |

#### Cycle

This value is a normalized double from 0-1. The defined value is the maximum proportion of the window the heating element can be turned on. E.g. cycle of 0.2 with a window of 1000 ms means the heating element can turn on at max 200 ms out of every 1000 ms.

| Publish   | Subscribe  |
|-----------|---|
| MQTT_CYCLE_PUB_TOPIC | MQTT_CYCLE_SUB_TOPIC  |

#### P Value

Weight of the proportional component. See PID Control for more information.

| Publish   | Subscribe  |
|-----------|---|
| MQTT_P_PUB_TOPIC | MQTT_P_SUB_TOPIC  |

#### I Value

Weight of the integral component. See PID Control for more information.

| Publish   | Subscribe  |
|-----------|---|
| MQTT_I_PUB_TOPIC | MQTT_I_SUB_TOPIC  |

#### D Value

Weight of the derivative component. See PID Control for more information.

| Publish   | Subscribe  |
|-----------|---|
| MQTT_D_PUB_TOPIC | MQTT_D_SUB_TOPIC  |

## Contributing

Since C++ is not my strongest of languages and this is my first implementation of a pid control loop, there are probably a lot of things that can be done easier or cleaner.

Please feel free to dive in! [Open an issue](https://github.com/thefipster/rancilio-silvia-pid/issues/new) or submit PRs.

## License

MIT © thefipster
