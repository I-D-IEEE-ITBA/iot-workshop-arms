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
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <FastLED.h>
#include <EEPROM.h>

#include "debug.h"
#include "pin_defines.h"

#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets_example.h"
#pragma message("Usando parámetros de secrets_example.h ...")
#endif

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SMOOTH_TARGET 0.02 // multiplier for the target value on smoothing
#define SMOOTH_PREV 0.98   // multiplier for the previous value on smoothing
#define POS_TOLERANCE 0.5  // Tolerance for position, in degrees

// Un byte de EEPROM (dirección 0x0) para guardar el N° de grupo (arm_id)
#define EEPROM_SIZE 1
#define EEPROM_ARM_ID_ADDR 0

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
    SUBSCRIPTIONS_N
} SUBS_T;

typedef enum servos_id
{
    BASE_SERVO_ID = SUB_BASE,
    BASE_JOINT_SERVO_ID,
    ELBOW_SERVO_ID,
    WRIST_YAW_SERVO_ID,
    WRIST_PITCH_SERVO_ID,
    CLAW_SERVO_ID,
    SERVOS_N
} SERVOS_ID_T;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void setup_gpios(void);
static void setup_mqtt(void);
static void setup_wifi(void);

static void mqtt_reconnect();
static void mqtt_callback(char *topic, byte *payload, unsigned int length);

static void cycle_color(CRGB color);
static void rgb_update(byte *data, unsigned int length);

static void led_update(byte *data, unsigned int length);

static void smooth_target(float &target, float &smoothed, float &prev);

static void update_subscriptions_with_arm_id(String *subscriptions, uint8_t arm_id);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static const char WIFI_SSID[] = __WIFI_SSID;
static const char WIFI_PASSWORD[] = __WIFI_PASSWORD;

static const uint8_t MQTT_SERVER_IP[] = __MQTT_SERVER_IP;
static const uint16_t MQTT_SERVER_PORT = __MQTT_SERVER_PORT;
static const char MQTT_USER[] = __MQTT_USER;
static const char MQTT_PASSWORD[] = __MQTT_PASSWORD;
static const char MQTT_CLIENT_ID[] = __MQTT_CLIENT_ID;

static String subscriptions[SUBSCRIPTIONS_N] = {
    BASE_SERVO_TOPIC,
    BASE_JOINT_SERVO_TOPIC,
    ELBOW_SERVO_TOPIC,
    WRIST_YAW_SERVO_TOPIC,
    WRIST_PITCH_SERVO_TOPIC,
    CLAW_SERVO_TOPIC,
    RGB_TOPIC,
    LED_RED_TOPIC};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static WiFiClient wclient;
static PubSubClient client(wclient);

static uint8_t arm_id;
static Servo servos[SERVOS_N];
static CRGB leds[NUM_LEDS];

static unsigned long currentMillis;
static unsigned long previousMillis = 0; // set up timers
static unsigned long interval = 10;      // time constant for timer (in ms)

static int stepFlag = 0;
static unsigned long previousStepMillis = 0;

static float target_pos[SERVOS_N] = {90, 90, 90, 90, 90, 90};
static float target_pos0[SERVOS_N] = {90, 90, 90, 90, 90, 90};
static float target_pos1[SERVOS_N] = {90, -90, 90, 90, 90, 90};
static float target_pos2[SERVOS_N] = {0, 90, 0, 0, 0, 0};
static float target_pos3[SERVOS_N] = {0, -90, 0, 0, 0, 0};
static float smoothed_pos[SERVOS_N] = {0, 0, 0, 0, 0, 0};
static float prev_pos[SERVOS_N] = {0, 0, 0, 0, 0, 0};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void setup()
{
    Serial.begin(115200);

    EEPROM.begin(EEPROM_SIZE);
    arm_id = EEPROM.read(EEPROM_ARM_ID_ADDR);
    update_subscriptions_with_arm_id(subscriptions, arm_id);

    setup_gpios();
    setup_wifi();
    setup_mqtt();

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    for (int i = 0; i < SERVOS_N; i++)
        servos[i].setPeriodHertz(50); // standard 50 hz servo

    // servos[BASE_SERVO_ID].attach(BASE_SERVO_PIN);
    // servos[BASE_JOINT_SERVO_ID].attach(BASE_JOINT_SERVO_PIN);
    // servos[ELBOW_SERVO_ID].attach(ELBOW_SERVO_PIN);
    // servos[WRIST_YAW_SERVO_ID].attach(WRIST_YAW_SERVO_PIN);
    // servos[WRIST_PITCH_SERVO_ID].attach(WRIST_PITCH_SERVO_PIN);
    // servos[CLAW_SERVO_ID].attach(CLAW_SERVO_PIN);

    servos[BASE_SERVO_ID].attach(BASE_SERVO_PIN, 1000, 1900);
    
    // if(arm_id == 5)
    //     servos[BASE_JOINT_SERVO_ID].attach(BASE_JOINT_SERVO_PIN, DEFAULT_uS_LOW, 1500);
    // else
    //     servos[BASE_JOINT_SERVO_ID].attach(BASE_JOINT_SERVO_PIN, 1000, 1900);

    // if(arm_id == 6)
    //     servos[ELBOW_SERVO_ID].attach(BASE_JOINT_SERVO_PIN, 1200, 1600);
    // else
    //     servos[ELBOW_SERVO_ID].attach(ELBOW_SERVO_PIN, 1000, 1900);

    servos[BASE_JOINT_SERVO_ID].attach(BASE_JOINT_SERVO_PIN, 1000, 1900);
    
    servos[ELBOW_SERVO_ID].attach(ELBOW_SERVO_PIN, 1000, 1900);
    servos[WRIST_YAW_SERVO_ID].attach(WRIST_YAW_SERVO_PIN, 1000, 1900);
    servos[WRIST_PITCH_SERVO_ID].attach(WRIST_PITCH_SERVO_PIN, 1000, 1900);
    servos[CLAW_SERVO_ID].attach(CLAW_SERVO_PIN, 1000, 1900);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(100);
}

void loop()
{
    if (!client.connected())
        mqtt_reconnect();

    client.loop();

    currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {                                   // start timed loop
        previousMillis = currentMillis; // Reset timer

        // *** smoothing ***
        for (int i = 0; i < SERVOS_N; i++)
        {
            smooth_target(target_pos[i], smoothed_pos[i], prev_pos[i]);
            // Move servos that need to be mooved
            if (abs(target_pos[i] - smoothed_pos[i]) > POS_TOLERANCE)
                servos[i].write(smoothed_pos[i]);
        }
        // *** end of smoothing ***
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void setup_gpios(void)
{
    // pinMode(Board_LED, OUTPUT);
    // digitalWrite(Board_LED, Board_LED_OFF);

    // pinMode(External_LED, OUTPUT);
    // digitalWrite(External_LED, External_LED_OFF);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    // LED interno de la placa del ESP32
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

static void setup_mqtt(void)
{
    client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    client.setCallback(mqtt_callback);
}

static void setup_wifi(void)
{
    debug_message("\n\n");
    debug_message("Connecting to ");
    debug_message(WIFI_SSID);
    debug_message("\n");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        cycle_color(CRGB::Blue);
        debug_message(".");
    }
    debug_message("\nWiFi connected");
    debug_message("IP address: ");
    debug_message(WiFi.localIP());
    debug_message("\n\n");
    cycle_color(CRGB::White);
}

static void mqtt_reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        debug_message("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD))
        {
            debug_message("connected \r\n");

            cycle_color(CRGB::White);
            // and subscribe to topics
            for (int i = 0; i < SUBSCRIPTIONS_N; i++)
                client.subscribe(subscriptions[i].c_str());
        }
        else
        {
            debug_message("failed, rc=");
            debug_message(client.state());
            debug_message(" try again in 3 seconds \r\n");
            // Wait before retrying
            cycle_color(CRGB::Red);
            delay(3000);
        }
    }
}

static void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    debug_message("Message arrived [");
    debug_message("Topic:");
    debug_message(topic);
    debug_message("  Length:");
    debug_message(length);
    debug_message("] ");
    debug_message("  Payload: ");
    for (int i = 0; i < length; i++)
    {
        debug_message((char)payload[i]);
    }
    debug_message("\r\n");
    // client.publish("esp_answer_topic","chau",false); //answers
    //  ParseTopic(topic,payload,length);

    int i = 0;
    String payloadString = "";

    for (i = 0; i < length; i++)
        payloadString += (char)payload[i];

    for (i = 0; i < SUBSCRIPTIONS_N; i++)
    {
        // Ignoro si el topic que llega no coincide con alguna de las suscripciones
        if (strcmp(topic, subscriptions[i].c_str()) == 0)
            break;
    }

    switch (i)
    {
    case SUB_BASE:
        target_pos[BASE_SERVO_ID] = (byte)payloadString.toInt();
        break;
    case SUB_BASE_JOINT:
        target_pos[BASE_JOINT_SERVO_ID] = (byte)payloadString.toInt();
        break;
    case SUB_ELBOW:
        target_pos[ELBOW_SERVO_ID] = (byte)payloadString.toInt();
        break;
    case SUB_WRIST_YAW:
        target_pos[WRIST_YAW_SERVO_ID] = (byte)payloadString.toInt();
        break;
    case SUB_WRIST_PITCH:
        target_pos[WRIST_PITCH_SERVO_ID] = (byte)payloadString.toInt();
        break;
    case SUB_CLAW:
        target_pos[CLAW_SERVO_ID] = (byte)payloadString.toInt();
        break;
    case SUB_RGB:
        rgb_update(payload, length);
        delay(10);
        break;
    case SUB_LED_RED:
        led_update(payload, length);
        delay(10);
        break;
    }
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

static void rgb_update(byte *data, unsigned int length)
{
    String aux = "";
    byte RGB[3] = {0, 0, 0};
    int j = 0;
    int i = 0;
    char current_char = 0;
    char last_char = 0;

    while (i < length)
    {                                 // We know that in this case the format of data is "rgb(r, g, b)"
        current_char = (char)data[i]; // Save current character
        if (current_char >= '0' && current_char <= '9')
        {
            aux += current_char; // If we get a number, we add it to the aux number
        }
        else
        {
            if (last_char >= '0' && last_char <= '9')
            {                               // If last character was a number, and I now got a non-number
                RGB[j] = (byte)aux.toInt(); // We save current number
                aux = "";                   // Reset aux string
                j++;                        // And move to next number
            }
            // If last char was a non-nuber, and I now got a non-number, we just move to the next character
        }
        last_char = current_char;
        i++;
    }

    CRGB color = CRGB(RGB[0], RGB[1], RGB[2]);
    cycle_color(color);
}

static void led_update(byte *data, unsigned int length)
{
    String payloadString = "";

    for (int i = 0; i < length; i++)
        payloadString += (char)data[i];

    if (payloadString == "true")
    {
        digitalWrite(LED_RED, HIGH);
        debug_message("LED ON");
        debug_message("\r\n");
    }
    else if (payloadString == "false")
    {
        digitalWrite(LED_RED, LOW);
        debug_message("LED OFF");
        debug_message("\r\n");
    }
}

static void smooth_target(float &target, float &smoothed, float &prev)
{
    // Pondera por SMOOTH_PREV el valor anterior y por SMOOTH_TARGET el valor objetivo.
    smoothed = target * SMOOTH_TARGET + prev * SMOOTH_PREV;
    prev = smoothed;
}

static void update_subscriptions_with_arm_id(String *subscriptions, uint8_t arm_id)
{
    for (int i = 0; i < SUBSCRIPTIONS_N; ++i)
    {
        subscriptions[i].replace("_G", "_G" + String(arm_id));
    }
}
