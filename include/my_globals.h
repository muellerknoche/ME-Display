#ifndef _MYGLOBALS_0111_H_
#define _MYGLOBALS_0111_H_

#include "my_globals.h"
#include <Arduino.h>

extern uint8_t eingabe_zaehler;
extern uint8_t in_code[4];
extern uint8_t reference_code[4];

extern bool pin_ok;
extern bool no_buttons;
extern bool pinOkSet;
extern bool video;

extern bool firstTouch;
extern bool firstTouchSeen;

// defaults
extern String defaultSsid;
extern String defaultPassword;               // Must be at least 8 chars for WPA2
extern IPAddress defaultIP;
extern IPAddress defaultGateway;            // String?
extern IPAddress defaultSubnet;
extern String defaultPin;
extern String defaultLaenge;

// from SD Card
extern String ssid;
extern String password;
extern IPAddress localIP;
extern String gateway;				        // gateway has the same value than localIP
extern IPAddress subnet;
extern String pin;
extern String laenge;

extern uint8_t csPin;

extern uint8_t pin_len;


#endif //  _MYGLOBALS_0111_H_