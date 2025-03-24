/**
 * @file secrets_example.h
 * @brief Example of a secrets header file.
 * @version 0.1
 * @date 2024-06-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SECRETS_EXAMPLE_H
#define SECRETS_EXAMPLE_H

// WiFi credentials
#define __WIFI_SSID ("your_wifi_ssid")
#define __WIFI_PASSWORD ("your_wifi_password")

// MQTT credentials
#define __MQTT_SERVER_IP {192, 168, 1, 1}   // Example IP of the MQTT Broker
#define __MQTT_SERVER_PORT (1883)           // Example port of the MQTT Broker
#define __MQTT_USER ("your_mqtt_user")
#define __MQTT_PASSWORD ("your_mqtt_password")
#define __MQTT_CLIENT_ID ("your_mqtt_client_id")

// MQTT topics
#define BASE_SERVO_TOPIC "your_base_servo_topic"
#define BASE_JOINT_SERVO_TOPIC "your_base_joint_servo_topic"
#define ELBOW_SERVO_TOPIC "your_elbow_servo_topic"
#define WRIST_YAW_SERVO_TOPIC "your_wrist_yaw_servo_topic"
#define WRIST_PITCH_SERVO_TOPIC "your_wrist_pitch_servo_topic"
#define CLAW_SERVO_TOPIC "your_claw_servo_topic"
#define RGB_TOPIC "your_rgb_topic"
#define LED_RED_TOPIC "your_led_red_topic"

#endif // SECRETS_EXAMPLE_H
