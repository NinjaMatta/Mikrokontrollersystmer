
#include "USART.h"
#include "CLK.h"
#include "EEPROM.h"
#include "ADC.h"
#include "PWM.h"
#include "EVSYS.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#define DELETE 127 // Define delete in ASCII character code

char buffer[128]; // Create a buffer for storing received characters
uint8_t buffer_index = 0; // Initialize index to 0

uint8_t end_program = 0; // Initialize program end flag to 1

char* command; // Create pointer for command string
char* argument0; // Create pointer for argument0 string
char* argument1; // Create pointer for argument1 string

int main(void)
{
	CLK_initialize(); // Initialize main clock
	TCA0_initialize(); // Initialize TCA0 clock
	TCB0_initialize(); // Initialize TCB0 clock
	RTC_initialize(); // Initialize TCB0 clock
	ADC0_initialize(); // Initialize ADC0
	USART3_initialize(); // Initialize USART
	
	USART3_update_trend_data(); // Measures the speed of fans in every start up and saves it to EEPROM
	
	sei(); // Enable interrupts
	
	while(!end_program); // Infinite loop until program ends
	
	cli(); // Disables interrupts
}

ISR(USART3_RXC_vect) // USART receive interrupt
{
	uint8_t received_char = USART3_receive(); // Read received character
	USART3_transmit(received_char); // Transmit received character (ECHO)
	
	// Command entered
	if (received_char == '\r') // If the received character is a carriage return
	{
		buffer[buffer_index] = '\0'; // Terminate the buffer string
		buffer_index = 0; // Reset the buffer index to 0
		
		command = strtok(buffer, " "); // Extract command string from buffer
		argument0 = strtok(NULL, " "); // Extract argument string from buffer
		argument1 = strtok(NULL, " "); // Extract argument string from buffer
		
		USART3_new_line(); // Print a new line to USART
		
		// Fan_Control command
		if (strcmp(command, "Fan_Control") == 0) // If the command is "Fan_Control"
		{
			USART3_fan_control_command_handler(argument0, argument1);
		}

		// Read_Fan command
		else if (strcmp(command, "Read_Fan") == 0) // If the command is "Read_Fan"
		{
			USART3_read_fan_command_handler(argument0, argument1);
		}

		// Read_EEPROM command
		else if (strcmp(command, "Read_EEPROM") == 0) // If the command is "Read_EEPROM"
		{
			USART3_read_EEPROM_command_handler(argument0, argument1);
		}
		
		// Update_EEPROM command
		else if (strcmp(command, "Update_EEPROM") == 0) // If the command is "Update_EEPROM"
		{
			USART3_update_trend_data();
		}
		
		// Reset_EEPROM command
		else if (strcmp(command, "Reset_EEPROM") == 0) // If the command is "Reset_EEPROM"
		{
			USART3_reset_EEPROM_command_handler(argument0);
		}
		
		// Help command
		else if (strcmp(command, "Help") == 0) // If the command is "Help"
		{
			USART3_help_command_handler(argument0);
		}
		
		// End command
		else if (strcmp(command, "End") == 0) // If the command is "End"
		{
			end_program = USART3_end(); // Print end message and returns 1 to 'end program' 
		}
		
		else // If the command is not recognized
		{
			USART3_no_command(command); // Print error message
		}
	}

	// Part of command deleted
	else if (received_char == DELETE) // If the received character is the delete key (backspace)
	{
		if (buffer_index > 0) // If the buffer index is greater than 0
		{
			buffer_index--; // Decrement buffer index
		}
	}

	// Command processing
	else // If the received character is not a carriage return or delete key
	{
		buffer[buffer_index++] = received_char; // Add character to buffer
	}
}