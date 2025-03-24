/**
 * @file secrets.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SECRETS_H
#define SECRETS_H

// WiFi credentials
#define __WIFI_SSID ("IEEE_ITBA_IOT")
#define __WIFI_PASSWORD ("ieee2024")

// MQTT credentials
#define __MQTT_SERVER_IP {192, 168, 0, 100} // IP de quien corre el MQTT Broker
#define __MQTT_SERVER_PORT (1883)           // Puerto de quien corre el MQTT Broker donde espera envíos
#define __MQTT_USER ("")
#define __MQTT_PASSWORD ("")
#define __MQTT_CLIENT_ID ("")

// MQTT topics
#define BASE_SERVO_TOPIC "robot_arm_base_G"
#define BASE_JOINT_SERVO_TOPIC "robot_arm_base_joint_G"
#define ELBOW_SERVO_TOPIC "robot_arm_elbow_G"
#define WRIST_YAW_SERVO_TOPIC "robot_arm_wrist_yaw_G"
#define WRIST_PITCH_SERVO_TOPIC "robot_arm_wrist_pitch_G"
#define CLAW_SERVO_TOPIC "robot_arm_claw_G"
#define RGB_TOPIC "robot_arm_rgb_G"
#define LED_RED_TOPIC "led_state_G"

#endif // SECRETS_H
