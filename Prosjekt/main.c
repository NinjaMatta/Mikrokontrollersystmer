
#include "USART.h"
#include "CLK.h"
#include "EEPROM.h"
#include "ADC.h"
#include "PWM.h"
#include "EVSYS.h"

#include <avr/interrupt.h>
#include <avr/io.h>

// Create a buffer for storing received characters
char buffer[128];

// Initialize index of buffer to 0
uint8_t buffer_index = 0;

// Initialize program end flag to 0
uint8_t end_program = 0;

// Create pointers for command handeling
char* command;
char* argument0;
char* argument1;

int main(void)
{
	// Initialize different peripheral devices
	CLK_initialize();
	TCA0_initialize();
	TCB0_initialize();
	RTC_initialize();
	ADC0_initialize();
	USART3_initialize();
	
	// Measures the speed of fans in every start up and saves it to EEPROM
	USART3_update_trend_data();
	
	// Enable interrupts
	sei();
	
	// Infinite loop until 'End'-command is ran
	while(!end_program);
	
	// Disables interrupts
	cli();
}
// USART receive interrupt
ISR(USART3_RXC_vect)
{
	// Read received character
	uint8_t received_char = USART3_receive();
	
	// Transmit received character (ECHO)
	USART3_transmit(received_char);
	
	// Command entered
	// If the received character is a carriage return
	if (received_char == '\r')
	{
		// Terminate the buffer string
		buffer[buffer_index] = '\0';
		 
		// Reset the buffer index to 0
		buffer_index = 0;
		
		 // Extract command string from buffer
		command = strtok(buffer, " ");
		
		// Extract argument string from buffer
		argument0 = strtok(NULL, " ");
		argument1 = strtok(NULL, " ");
		
		// Print a new line to USART
		USART3_new_line();
		
		// Fan_Control command
		// If the command is "Fan_Control"
		if (strcmp(command, "Fan_Control") == 0)
		{
			USART3_fan_control_command_handler(argument0, argument1);
		}

		// Read_Fan command
		// If the command is "Read_Fan"
		else if (strcmp(command, "Read_Fan") == 0)
		{
			USART3_read_fan_command_handler(argument0, argument1);
		}

		// Read_EEPROM command
		// If the command is "Read_EEPROM"
		else if (strcmp(command, "Read_EEPROM") == 0)
		{
			USART3_read_EEPROM_command_handler(argument0, argument1);
		}
		
		// Update_EEPROM command
		// If the command is "Update_EEPROM"
		else if (strcmp(command, "Update_EEPROM") == 0)
		{
			USART3_update_trend_data();
		}
		
		// Reset_EEPROM command
		// If the command is "Reset_EEPROM"
		else if (strcmp(command, "Reset_EEPROM") == 0)
		{
			USART3_reset_EEPROM_command_handler(argument0);
		}
		
		// Help command
		// If the command is "Help"
		else if (strcmp(command, "Help") == 0)
		{
			USART3_help_command_handler(argument0);
		}
		
		// End command
		// If the command is "End"
		else if (strcmp(command, "End") == 0)
		{
			// Print end message and returns 1 to 'end program' 
			end_program = USART3_end();
		}
		// If the command is not recognized
		else
		{
			// Print error message
			USART3_no_command(command);
		}
	}

	// Part of command deleted
	// If the received character is the delete key (backspace)
	else if (received_char == DELETE)
	{
		// If the buffer index is greater than 0
		if (buffer_index > 0)
		{
			// Decrement buffer index
			buffer_index--;
		}
	}

	// Command processing
	// If the received character is not a carriage return or delete key
	else
	{
		// Add character to buffer
		buffer[buffer_index++] = received_char;
	}
}