#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>
#include <WiFi.h>

// ==== for SD Card ===
// SD card CS pin (adjust based on your wiring)			// braucht man das, wohl ja da SD.h
const int csPin = 10;

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

// Default values if SD read fails
String defaultSSID = "ELECROW";
String defaultPassword = "dummyPASS";               // Must be at least 8 chars for WPA2
IPAddress defaultIP(192, 168, 4, 1);
IPAddress defaultGateway(192, 168, 4, 1);
IPAddress defaultSubnet(255, 255, 255, 0);
String defaultPin = "5260";
String defaultLaenge = "4";

// Variables to store loaded config
String ssid;
String password;
IPAddress localIP;
IPAddress gateway;
IPAddress subnet;
String pin;
String laenge;


#define TFT_BL 2

// === flags flow control ===
bool pinOk = false;                 // PIN is valid
bool failed = false;                // PIN was invalid
bool showVideo = false;             // enable video if true
// Zaehler PIN Eingabe
uint8_t eingabe_zaehler = 0;        // digits entered

int reference_code[4];              // to be filled from SD 
uint8_t pin_len;                    // to befilled from SD
// Array enthaelt eingegebene PIN
int in_code[4];                      // storage for enter digits

// === more flags ====
bool touched = false;               // screen touched
bool no_buttons = true;             // keypad not initialized
bool video;
bool pinOkSet = false;              // Video und Timer nur einmal

unsigned long now;

int loopPosX = 500;
int loopPosY = 350;

#endif // _CONFIG_H_