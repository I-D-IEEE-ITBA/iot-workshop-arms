/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <Arduino.h>
#include <ESP32Servo.h>
#include <FastLED.h>

#include "debug.h"
#include "pin_defines.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SMOOTH_TARGET 0.03 // multiplier for the target value on smoothing
#define SMOOTH_PREV 0.97   // multiplier for the previous value on smoothing
#define POS_TOLERANCE 0.5  // Tolerance for position, in degrees

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum subs
{
    SUB_BASE, // Los 6 subs de los servos tienen que ser siempre los primeros 6
    SUB_BASE_JOINT,
    SUB_ELBOW,
    SUB_WRIST_YAW,
    SUB_WRIST_PITCH,
    SUB_CLAW,
    SUB_RGB,
    SUB_LED_RED,
    SUBSCRIPTIONS
} SUBS_T;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void setup_gpios(void);
static void cycle_color(CRGB color);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static Servo servos[6];

// RGB strip
static CRGB leds[NUM_LEDS];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void setup()
{
    Serial.begin(115200);

    setup_gpios(); // initialize used GPIOS

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    for (int i = 0; i < 6; i++){
        servos[i].setPeriodHertz(50); // standard 50 hz servo
    }

    servos[SUB_BASE].attach(BASE_SERVO_PIN, 1000, 2000);
    servos[SUB_BASE_JOINT].attach(BASE_JOINT_SERVO_PIN, 1000, 2000);
    servos[SUB_ELBOW].attach(ELBOW_SERVO_PIN, 1000, 2000);
    servos[SUB_WRIST_YAW].attach(WRIST_YAW_SERVO_PIN, 1000, 2000);
    servos[SUB_WRIST_PITCH].attach(WRIST_PITCH_SERVO_PIN, 1000, 2000);
    servos[SUB_CLAW].attach(CLAW_SERVO_PIN, 1000, 2000);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(100);

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
    }
    FastLED.show();

    digitalWrite(LED_RED, HIGH);
    //cycle_color(CRGB::Red);
/*
    for (int i = 0; i < 6; i++)
    {
        servos[0].write(90);
        delay(1500);
        servos[0].write(180+90*i);
        delay(1500);
    }
*/
//servo elbow no funciona!!!!!!!!!!!!!!!!!!!!!!
//tira led no anda(pensamos que se quemo), pero el pin 14 anda

    int i = 2;
    servos[i].write(0);
    delay(1000);

    printf("\n\nTesteando servo: ");
   
    servos[i].write(45);
    delay(1000);
    
    servos[i].write(90);
    delay(1000);
    
    servos[i].write(135);
    delay(1000);
    
    servos[i].write(180);
    delay(1000);

    servos[i].write(-180);
    delay(1000);
    

/*
    for (int i = 0; i < 6; i++)
    {
        Serial.print("\n\nTesteando servo: ");
        Serial.println(i);
        Serial.println("\n");

        for (int j = 90; j < 135; j++)
        {
            servos[i].write(j);
            delay(20);
        }
        for (int j = 135; j > 45; j--)
        {
            servos[i].write(j);
            delay(20);
        }
        for (int j = 45; j < 90; j++)
        {
            servos[i].write(j);
            delay(20);
        }

        delay(250);
    }
*/


}

void loop()
{
    
    digitalWrite(19, LOW);
    cycle_color(CRGB::Black);
    delay(500);
    digitalWrite(19, HIGH);
    cycle_color(CRGB::Red);
    delay(500);
    
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void setup_gpios(void)
{
    pinMode(Board_LED, OUTPUT);
    digitalWrite(Board_LED, Board_LED_OFF);
    pinMode(External_LED, OUTPUT);
    digitalWrite(External_LED, External_LED_OFF);
}

static void cycle_color(CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
        leds[i] = color;
        FastLED.show();
    }
    FastLED.show();
}
