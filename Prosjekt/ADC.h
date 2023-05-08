
#pragma once

#include <avr/io.h>
#include <stdbool.h>
#include <math.h>

/*
* Function: ADC0_init
* -----------------------
* Initializes ADC0 to read value of thermistor
* 
* returns: void
*/
void ADC0_initialize(void);

/*
* Function: ADC0_make_measurement
* -----------------------
* Starts ADC conversion and then waits for conversion to be done.
* 
* returns: Measured ADC value
*/
uint16_t ADC0_make_measurement(void);

/*
* Function: get_temperature_celcius
* -----------------------
* Reads value from ADC using ADC0_read() 
* and converts the value to a temperature in celcius
* 
* returns: Temperature read from thermistor 
*          in celcius rounded up to closest whole number
*/
uint16_t get_temperature_celcius(void);