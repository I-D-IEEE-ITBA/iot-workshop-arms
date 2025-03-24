/**
 * @file debug.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define DEBUG_OFF 0
#define DEBUG_ON 1
#define debug DEBUG_ON
#define debug_message(fmt)     \
    do                         \
    {                          \
        if (debug)             \
            Serial.print(fmt); \
    } while (0)

#define serial_message(fmt)          \
    do                               \
    {                                \
        if (debug)                   \
            Serial.print(">>> ");    \
        Serial.print(fmt);           \
        if (debug)                   \
            Serial.print(" <<<<\n"); \
    } while (0)

#endif // _DEBUG_H_