#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>

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
    if (millis() > start_time + wait_time)
    {
        erg = true;
    }
    return erg;
} // END my_timer


#endif