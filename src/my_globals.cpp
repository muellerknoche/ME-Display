/**
 * This is my_global.cpp
 * @author Rainer Müllder-Knoche
 * @date 01.11.2025
 */

#include "my_globals.h"
#include <Arduino.h>

uint8_t pin_len = 4;
uint8_t eingabe_zaehler = 0;
uint8_t in_code[4];
uint8_t reference_code[4] ;

bool no_buttons = true;

bool pin_ok = false;
bool pinOkSet = false;

bool video = false;

bool firstTouch = false;
bool firstTouchSeen = false;

