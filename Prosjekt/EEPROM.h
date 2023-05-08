
#pragma once

#include "EVSYS.h"

#include <avr/io.h>

// Define amount of trend data to be stored in EEPROM
#define TREND_DATA_SIZE 10

// Define a start address for each of the eight fans
#define FAN_0_START_ADDRESS (0*TREND_DATA_SIZE)
#define FAN_1_START_ADDRESS (1*TREND_DATA_SIZE)
#define FAN_2_START_ADDRESS (2*TREND_DATA_SIZE)
#define FAN_3_START_ADDRESS (3*TREND_DATA_SIZE)
#define FAN_4_START_ADDRESS (4*TREND_DATA_SIZE)
#define FAN_5_START_ADDRESS (5*TREND_DATA_SIZE)
#define FAN_6_START_ADDRESS (6*TREND_DATA_SIZE)
#define FAN_7_START_ADDRESS (7*TREND_DATA_SIZE)

// Define an address for storing iteration number in EEPROM
#define ITERATION_ADDRESS (8*TREND_DATA_SIZE)

// Variable used to adjust the size of fan speed to fit in one byte
#define RPM_TO_SIZE_OF_BYTE 52

/*
* Function: EEPROM_read
* -----------------------
* Reads a value from an EEPROM address
*
* adr: The address to read EEPROM from
* returns: The value stored in EEPROM
*/
uint8_t EEPROM_read(uint8_t adr);

/*
* Function: EEPROM_read_RPM
* -----------------------
* Reads a value from an EEPROM address. Multiplies this value 
* with RPM_TO_SIZE_OF_BYTE and returns it
*
* adr: The address to read EEPROM from
* returns: The value stored in EEPROM adjusted to the right RPM value
*/
uint16_t EEPROM_read_RPM(uint8_t adr);

/*
* Function: EEPROM_read_RPM_array
* -----------------------
* Reads multiple values from the EEPROM. The values read gets
* stored in an array
*
* fan_number: The fan numbers trend data that is read
* array: The array that EEPROM values gets stored in
* returns: void
*/
void EEPROM_read_RPM_array(uint8_t fan_number, uint16_t array[]);

/*
* Function: EEPROM_read_RPM_average
* -----------------------
* Reads multiple values from the EEPROM. The values read gets
* stored in an array. The elements of the array then gets summed
* up and divided by the array size to get the average value
*
* fan_number: The fan numbers trend data that is read
* array: The array that EEPROM values gets stored in
* returns: The average value of the trend data
*/
uint16_t EEPROM_read_RPM_average(uint8_t fan_number, uint16_t array[]);

/*
* Function: EEPROM_update
* -----------------------
* Updates a value to an EEPROM address
*
* adr: The EEPROM address to update
* value: The value to be updated
* returns: void
*/
void EEPROM_update(uint8_t adr, uint8_t value);

/*
* Function: EEPROM_update_RPM
* -----------------------
* Updates a value to an EEPROM address. The value
* is divided by RPM_TO_SIZE_OF_BYTE before updated.
* This is to make the value fit in one byte
*
* adr: The EEPROM address to update
* value: The value to be updated
* returns: void
*/
void EEPROM_update_RPM(uint8_t adr, uint16_t value);

/*
* Function: EEPROM_measure_and_update_RPM
* -----------------------
* Measures the RPM value of a fan. This value is divided 
* by RPM_TO_SIZE_OF_BYTE before updated to one of the fans
* EEPROM addresses. Which address it is updated to is
* depending on the iteration.
*
* fan_number: The fan to be measured and updated
* iteration: The current iteration
* returns: void
*/
void EEPROM_measure_and_update_RPM(uint8_t fan_number, uint8_t iteration);

/*
* Function: EEPROM_measure_and_update_RPM_all
* -----------------------
* Measures the RPM value of all fans. Theese values
* are divided by RPM_TO_SIZE_OF_BYTE before updated to 
* one of the fans EEPROM addresses. Which address it is
* updated to is depending on the iteration. The iteration
* is read from its EEPROM address before overwriting the 
* other addresses. The iteration value is incremented or
* reset to zero at the end of th function
*
* returns: void
*/
void EEPROM_measure_and_update_RPM_all(void);