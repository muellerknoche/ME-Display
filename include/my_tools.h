#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>
#include <config.h>


/**
 * @brief checks if a start + wait time greater than millis(), start time was set
 * from millis() some time before
 * @param start_time 
 * @param wait_time
 * @returns TRUE if condition met, FALSE if not
 */
bool my_timer(unsigned long start_time, unsigned long wait_time)
{
    bool erg;
    erg = false;
    unsigned long  now = millis();
    unsigned long summe = start_time + wait_time;

    // #ifdef DEBUG
    //     Serial.print("millis ");Serial.println(now);
    //     Serial.print("start_time ");Serial.println(start_time);
    //     Serial.print("wait ");Serial.println(wait_time);
    //     Serial.print("summe ");Serial.println(summe);
    // #endif

    if (now > summe)
    {
             erg = true;
    }

    // #ifdef DEBUG
    // Serial.print("ERG ");Serial.println(erg);
    // #endif
    
    return erg;
} // END my_timer


void backlight_On_Off(bool On)
{
    if (On)
    {
        digitalWrite(2, HIGH);
    } else {
        digitalWrite(2,LOW);
    }
}

/**
 * @brief reset values to start again if there is no power in reset
 */
void set_defaults()
{
    eingabe_zaehler = 0;

} // END my_reset


#endif