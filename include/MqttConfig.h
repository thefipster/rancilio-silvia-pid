#ifndef MQTTTOPICS_H
#define MQTTTOPICS_H

// Server & Client
#define MQTT_IP IPAddress(192, 168, 1, 100)
#define MQTT_PORT 1883
#define MQTT_INTERVAL 1000
#define MQTT_CLIENTID "rancilio-silvia"

// Publish topics
#define MQTT_PID_TOPIC "kitchen/rancilio/control"
#define MQTT_BOILER_TOPIC "kitchen/rancilio/boiler"
#define MQTT_HEAD_TOPIC "kitchen/rancilio/head"
#define MQTT_HEATER_TOPIC "kitchen/rancilio/state"

// Subscribe topics
#define MQTT_TARGET_TOPIC "hassio/input_number/kitchen_rancilio_target_temperature/state"
#define MQTT_COLDSTART_TOPIC "hassio/input_number/kitchen_rancilio_coldstart_temperature/state"
#define MQTT_WINDOW_TOPIC "hassio/input_number/kitchen_rancilio_duty_window/state"
#define MQTT_CYCLE_TOPIC "hassio/input_number/kitchen_rancilio_duty_cycle/state"
#define MQTT_P_TOPIC "hassio/input_number/kitchen_rancilio_p_value/state"
#define MQTT_I_TOPIC "hassio/input_number/kitchen_rancilio_i_value/state"
#define MQTT_D_TOPIC "hassio/input_number/kitchen_rancilio_d_value/state"

#endif