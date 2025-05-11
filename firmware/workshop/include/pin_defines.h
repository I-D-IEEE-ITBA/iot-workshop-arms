/**
 * @file pin_defines.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _PIN_DEFINES_H_
#define _PIN_DEFINES_H_

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define D0  16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15

#define BASE_SERVO_PIN          18
#define BASE_JOINT_SERVO_PIN    05
#define ELBOW_SERVO_PIN         22
#define WRIST_YAW_SERVO_PIN     21
#define WRIST_PITCH_SERVO_PIN   23
#define CLAW_SERVO_PIN          19

#define NUM_LEDS  8
#define LED_PIN   17

#define LED_RED   32

#define Board_LED D0
#define External_LED D1
#define Board_LED_OFF  1
#define Board_LED_ON   0
#define External_LED_OFF  0
#define External_LED_ON   1

#endif // _PIN_DEFINES_H