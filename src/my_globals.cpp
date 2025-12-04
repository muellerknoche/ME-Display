/**
 * This is my_global.cpp
 * @author Rainer Müllder-Knoche
 * @date 01.11.2025
 */

#include "my_globals.h"
#include <Arduino.h>

uint8_t eingabe_zaehler = 0;
uint8_t in_code[4];
uint8_t reference_code[4] ;

bool no_buttons = true;

bool pin_ok = false;
bool pinOkSet = false;
bool stopstream = false; 
bool video = false;


bool firstTouch = false;
bool firstTouchSeen = false;
bool is_stream = false;

// Default values if SD read fails
String defaultSsid = "ELECROW";
String defaultPassword = "dummyPASS";               // Must be at least 8 chars for WPA2
IPAddress defaultIP(192, 168, 4, 1);
IPAddress defaultGateway(192, 168, 4, 1);
IPAddress defaultSubnet(255, 255, 255, 0);
String defaultPin = "2560";
String defaultLaenge = "4";

String ssid;
String password;
IPAddress localIP;
String gateway;
IPAddress subnet; //(255, 255, 255, 0);
String pin; // = "2560";
String laenge; // = "4";

uint8_t csPin = 10;

uint8_t pin_len = 0;

String wsUrl = "";