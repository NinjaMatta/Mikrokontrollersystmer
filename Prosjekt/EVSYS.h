
#pragma once

#include "USART.h"

#include <avr/io.h>

/*
* Function: read_tacho
* -----------------------
* Reads the frequency of rotation of a specified fan
*
* fan_number: Specifies the fan to read
* returns: Frequency of rotation in units of ticks per second (Hz)
*/
uint32_t read_tacho(uint8_t fan_number);


/*
* Function: frequency_to_RPM
* -----------------------
* Converts a frequency value (in Hz) to an RPM value (in rotations per minute)
*
* frequency: Frequency of rotation in units of ticks per second (Hz)
* returns: The fan speed in units of rotations per minute (RPM)
*/
uint16_t frequency_to_RPM(uint32_t frequency);


/*
* Function: read_fan_speed
* -----------------------
* Reads the speed of a specified fan
*
* fan_number: Specifies the fan to read
* returns: The fan speed in units of rotations per minute (RPM)
*/
uint16_t read_fan_speed(uint8_t fan_number);