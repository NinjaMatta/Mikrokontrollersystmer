
#pragma once

#include "CLK.h"

#include <avr/io.h>

#define F_CPU 16000000UL // Define clock frequency of CPU
#include "util/delay.h"

#define FAN_FREQUENCY 25000 // Define frequency of San Ace 40
#define PWM_PERIOD_VALUE (F_CPU/(2*FAN_FREQUENCY)) // Define calculated PWM period

/*
* Function: increase_fan_speed_first
* -----------------------
* Increases PWM duty cycle on compare pin 0 to PWM_duty_cycle 
* (a user requested duty cycle)
*
* PWM_duty_cycle: Desired duty cycle for PWM signal to compare pin 0
* returns: void
*/
void increase_fan_speed_first(uint16_t PWM_duty_cycle);

/*
* Function: increase_fan_speed_second
* -----------------------
* Increases PWM duty cycle on compare pin 1 to PWM_duty_cycle 
* (a user requested duty cycle)
*
* PWM_duty_cycle: Desired duty cycle for PWM signal to compare pin 1
* returns: void
*/
void increase_fan_speed_second(uint16_t PWM_duty_cycle);

/*
* Function: decrease_fan_speed_first
* -----------------------
* Decreases PWM duty cycle on compare pin 0 to PWM_dutyCycle 
* (a user requested duty cycle)
* 
* PWM_duty_cycle: Desired duty cycle for PWM signal to compare pin 0
* returns: void
*/
void decrease_fan_speed_first(uint16_t PWM_duty_cycle);

/*
* Function: decrease_fan_speed_second
* -----------------------
* Decreases PWM duty cycle on compare pin 1 to PWM_duty_cycle 
* (a user requested duty cycle)
* 
* PWM_duty_cycle: Desired duty cycle for PWM signal to compare pin 1
* returns: void
*/
void decrease_fan_speed_second(uint16_t PWM_duty_cycle);

/*
* Function: set_fan_speed_first
* -----------------------
* Used to set PWM(%) on pin 0 of port D 
* and thus the fan speed controlled by the PWM signal
* 
* fan_speed: Percent(%) desired fan speed of max fan speed (13100 RPM)
* returns: void
*/
void set_fan_speed_first(uint8_t fan_speed);

/*
* Function: set_fan_speed_second
* -----------------------
* Used to set PWM(%) on pin 1 of port D 
* and thus the fan speed controlled by the PWM signal
*
* fan_speed: Percent(%) desired fan speed of max fan speed (13100 RPM)
* returns: void
*/
void set_fan_speed_second(uint8_t fan_speed);

/*
* Function: set_fan_speed_both
* -----------------------
* Used to set PWM(%) on both pin 0 and pin 1 of port D 
* and thus the fan speed controlled by the PWM signal
*
* fan_speed: Percent(%) desired fan speed of max fan speed (13100 RPM)
* returns: void
*/
void set_fan_speed_both(uint8_t fan_speed);