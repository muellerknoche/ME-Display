#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define TFT_BL 2

bool pinOk = false;
bool failed = false;

bool showVideo = false;

// Zaehler PIN Eingabe
uint8_t eingabe_zaehler = 0;

uint8_t pin_len = 4;
// Array enthaelt eingegebene PIN
int incode[4];

bool first = true;
bool ftouch = true;

// fuer Display dunkel wenn nicht betaetigt
unsigned long BL_timer_start_time;             //Stratzeit wird bei jedem Tastendruck verlaengert
bool BL_timer_active = false;           // Timer laeuft oder nich bei false
unsigned long BL_timeout = 30000;       // Zeit bis BL wieder aus

unsigned long video_timer_start_time;            //Stratzeit wird bei jedem Tastendruck verlaengert
bool video_timer_active = false;        // Timer laeuft oder nich bei false
unsigned long video_timeout = 30000;     // Zeit bis BL wieder aus
bool pinOkSet = false;                  // Video und Timer nur einmal

unsigned long now;

int loopPosX = 500;
int loopPosY = 350;




#endif