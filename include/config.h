#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

bool pinOk = false;
bool failed = false;

bool showVideo = false;

// Zaehler PIN Eingabe
uint8_t eingabe_zaehler = 0;


bool first = true;
bool ftouch = true;

// fuer Display dunkel wenn nicht betaetigt
unsigned long dispOutStart;             //Stratzeit wird bei jedem Tastendruck verlaengert
bool dispOutRuns = false;               // Timer laeuft oder nich bei false
unsigned long dispTimeout = 30000;      // Zeit bis BL wieder aus

unsigned long videoOutStart;            //Stratzeit wird bei jedem Tastendruck verlaengert
bool videoOutRuns = false;              // Timer laeuft oder nich bei false
unsigned long videoTimeout = 30000;     // Zeit bis BL wieder aus
bool pinOkSet = false;                  // Video und Timer nur einmal

unsigned long now;

int loopPosX = 500;
int loopPosY = 350;




#endif