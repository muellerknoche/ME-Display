#ifndef _MYTOOLS_H_
#define _MYTOOLS_H_

#include <Arduino.h>
#include <config.h>


/**
 * @brief checks if a start + wait time greater than millis(), start time was set
 * from millis() some time before
 * @param start_time 
 * @param wait_time
 * @returns TRUE if condition met, FALSE if not
 */
bool my_timer(unsigned long start_time, unsigned long wait_time);
void backlight_On_Off(bool On);

#endif  // _MYTOOLS_H_