#ifndef _SDCARD_H_
#define _SDCARD_H_


#include <Arduino.h>

#include <WiFi.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <config.h>



// === First get Config from SD Card ====

// Function to trim whitespace from strings
String trim(String str);
// Function to read and parse config from SD
bool loadConfigFromSD();

#endif // _SDCARD_H_