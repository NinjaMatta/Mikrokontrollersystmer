
#include "EEPROM.h"

uint8_t EEPROM_read(uint8_t adr)
{
	// Declare a variable for the byte to be read
	uint8_t byte;
	
	// Cast the memory address to a uint8_t pointer and read the byte at the specified address
	// This part of the function is copied from the function nvm_eeprom_read_byte (line 103) in
	// (https://github.com/avrxml/asf/blob/master/xmega/drivers/nvm/nvm.c)
	byte = *(uint8_t*) (MAPPED_EEPROM_START+adr);
	
	// Return the byte that was read
	return byte;
}

uint16_t EEPROM_read_RPM(uint8_t adr)
{
	// Declare a variable for the value to be read
	uint16_t value;
	
	// Read the byte at the specified address and multiply it by the RPM conversion factor
	value = (RPM_TO_SIZE_OF_BYTE*EEPROM_read(adr));
	
	// Return the value that was read
	return value;
}

void EEPROM_read_RPM_array(uint8_t fan_number, uint16_t array[])
{
	// Declare a variable for the starting address of the fan's RPM data
	uint8_t adr = 0;
	
	// Switch on the fan number to determine the starting address of its RPM data
	switch(fan_number)
	{
		default:
			break;
			
		case 0:
			adr = FAN_0_START_ADDRESS;
			break;
		
		case 1:
			adr = FAN_1_START_ADDRESS;
			break;
		
		case 2:
			adr = FAN_2_START_ADDRESS;
			break;
		
		case 3:
			adr = FAN_3_START_ADDRESS;
			break;
		
		case 4:
			adr = FAN_4_START_ADDRESS;
			break;
		
		case 5:
			adr = FAN_5_START_ADDRESS;
			break;
		
		case 6:
			adr = FAN_6_START_ADDRESS;
			break;
		
		case 7:
			adr = FAN_7_START_ADDRESS;
			break;
	}
	
	// Loop over the trend data size and read each RPM value into the array
	for (uint8_t i = 0; i < TREND_DATA_SIZE; i++)
	{
		array[i] = EEPROM_read_RPM(adr+i);
	}
}

uint16_t EEPROM_read_RPM_average(uint8_t fan_number, uint16_t array[])
{
	// Read the RPM data array for the specified fan number
	EEPROM_read_RPM_array(fan_number, array);
	
	// Declare a variable for the average RPM value
	uint16_t average = 0;
	
	// Loop over the trend data size and sum up the RPM values in the array
	for (uint8_t i = 0; i < TREND_DATA_SIZE; i++)
	{
		average = (average+array[i]);
	}
	
	// Calculate the average by dividing the sum by the number of elements in the array
	average = (average/TREND_DATA_SIZE);
	
	// Return the average RPM value
	return average;
}

void EEPROM_update(uint8_t adr, uint8_t value)
{
	// Wait for the EEPROM to become ready for writing
	while (NVMCTRL_STATUS & NVMCTRL_EEBUSY_bm);
	
	// Set the SPM command register to write to EEPROM
	_PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, 0x13);
	
	// Write the value to the specified address in EEPROM
	*(uint8_t*) (MAPPED_EEPROM_START+adr) = value;
	
	// Clear the SPM command register to stop writing to EEPROM
	_PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, 0x00);
}

void EEPROM_update_RPM(uint8_t adr, uint16_t value)
{
	// Declare a variable for divided RPM value
	uint8_t dividedValue = (value/RPM_TO_SIZE_OF_BYTE);
	
	// Updates the value to EEPROM
	EEPROM_update(adr, dividedValue);
}

void EEPROM_measure_and_update_RPM(uint8_t fan_number, uint8_t iteration)
{
	// Declare a variable for reading a RPM value
	uint16_t RPM;
	RPM = read_fan_speed(fan_number);	
	
	// Switch on the fan_number to determine which fan to update
	switch(fan_number)
	{
		default:
			break;
		
		case 0:
			EEPROM_update_RPM(FAN_0_START_ADDRESS+iteration, RPM);
			break;
		
		case 1:
			EEPROM_update_RPM(FAN_1_START_ADDRESS+iteration, RPM);
			break;
		
		case 2:
			EEPROM_update_RPM(FAN_2_START_ADDRESS+iteration, RPM);
			break;
		
		case 3:
			EEPROM_update_RPM(FAN_3_START_ADDRESS+iteration, RPM);
			break;
		
		case 4:
			EEPROM_update_RPM(FAN_4_START_ADDRESS+iteration, RPM);
			break;
		
		case 5:
			EEPROM_update_RPM(FAN_5_START_ADDRESS+iteration, RPM);
			break;
		
		case 6:
			EEPROM_update_RPM(FAN_6_START_ADDRESS+iteration, RPM);
			break;
		
		case 7:
			EEPROM_update_RPM(FAN_7_START_ADDRESS+iteration, RPM);
			break;
	}
}

void EEPROM_measure_and_update_RPM_all(void)
{
	// Declare a variable for reading iteration
	uint8_t iteration;
	iteration = EEPROM_read(ITERATION_ADDRESS);
	
	// Sets fan spedd of all fans to 100%
	set_fan_speed_both(100);
	
	// Loops through all fans measuring the fan speed and updating th value to EEPROM
	for (uint8_t i = 0; i < 8; i++)
	{
		EEPROM_measure_and_update_RPM(i, iteration);
	}
	
	// Updates the iteration number
	if (iteration == 9)
	{
		// Resets the iteration number to 0 if reached maximum of 9
		EEPROM_update(ITERATION_ADDRESS, 0);
	} 
	else
	{
		// Increments the iteration number if its not reached 9
		EEPROM_update(ITERATION_ADDRESS, iteration++);
	}
}