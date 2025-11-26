#ifndef _KEYPA_D0111_H_
#define _KEYPAD_0111_H_


#include "my_globals.h"

#include <lvgl.h>


// Konstanten für Tastenposition

extern int col_1;
extern int col_2;
extern int col_3;

extern int row_1;
extern int row_2; 
extern int row_30;
extern int row_4;

extern int sizeX;
extern int sizeY;

// flags



// Array enthaelt eingegebene PIN
extern uint8_t in_code[4];
extern uint8_t refference_code[4];                  // storage for enter digits

extern int loopPosX;
extern int loopPosY;


// prueft nach Eingabe der PIN Laenge auf Gueltigkeit

void check_pin(uint8_t number);
static void eins_event(lv_event_t * e);
static void zwei_event(lv_event_t * e);
static void drei_event(lv_event_t * e);
static void vier_event(lv_event_t * e);
static void fuenf_event(lv_event_t * e);
static void sechs_event(lv_event_t * e);
static void sieben_event(lv_event_t * e);
static void acht_event(lv_event_t * e);
static void neun_event(lv_event_t * e);
static void cl_event(lv_event_t * e);
static void nullx_event(lv_event_t * e);
static void ok_event(lv_event_t * e);

void create_buttons(uint8_t farbe );

#endif  // _KEYPAD_H_
