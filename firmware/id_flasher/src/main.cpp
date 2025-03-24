

/**
 * @file main.cpp
 * @brief This file contains the code for flashing an ID to the EEPROM memory of the ESP32.
 */

#include <WiFi.h>
#include <EEPROM.h>

#define EEPROM_SIZE (1U)
#define EEPROM_ARM_ID_ADDR (0U)

#define ID_TO_FLASH (1U) // based on the N° of the arm

/**
 * @brief The setup function initializes the serial communication and flashes the ID to the EEPROM memory.
 */
void setup()
{
    Serial.begin(115200);

    EEPROM.begin(EEPROM_SIZE);

    Serial.print("Flashing ID ");
    Serial.print(ID_TO_FLASH);
    Serial.print(" at address ");
    Serial.print(EEPROM_ARM_ID_ADDR);
    Serial.println("...");
    
    EEPROM.write(EEPROM_ARM_ID_ADDR, ID_TO_FLASH);
    EEPROM.commit();

    Serial.println("ID flashed!");

    Serial.print("Reading ID from EEPROM: ");
    Serial.println(EEPROM.read(EEPROM_ARM_ID_ADDR));
}

void loop()
{
}