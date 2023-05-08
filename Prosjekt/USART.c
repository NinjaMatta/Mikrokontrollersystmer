
#include "USART.h"

// USART INITIALIZE
void USART3_initialize(void)
{
	// Set baud rate to 9600
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	
	// Set USART3 to default port location
	PORTMUX.USARTROUTEA = PORTMUX_USART3_DEFAULT_gc;
	
	// Set PB0 as output (TX pin)
	PORTB.DIR |= PIN0_bm;
	
	// Set PB1 as input (RX pin)
	PORTB.DIR &= ~PIN1_bm;
	
	// Enable USART3 receive interrupt
	USART3.CTRLA = USART_RXCIE_bm;
	
	// Enable USART3 transmitter and receiver
	USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	
	// Set USART3 to 8-bit data with no parity and 1 stop bit
	USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;
	
	// Set USART3 auto-baud window size to WDW0
	USART3.CTRLD = USART_ABW_WDW0_gc;
}

// USART3 READ FUNCTION
char USART3_receive(void)
{
	// Wait until USART3 receive complete flag is set
	while (!(USART3.STATUS & USART_RXCIF_bm));
	
	// Return received data
	return USART3.RXDATAL;
}

// USART3 WRITE FUNCTION
void USART3_transmit(char character)
{
	// Wait until USART3 data register empty flag is set
	while (!(USART3.STATUS & USART_DREIF_bm));
	
	// Transmit data
	USART3.TXDATAL = character;
}

void USART3_transmit_string(char *string)
{
	// Loop through each character in string
	for(int i = 0; i < strlen(string); i++)
	{
		// Transmit each character
		USART3_transmit(string[i]);
	}
}

void USART3_new_line(void)
{
	USART3_transmit_string("\r\n");
}

// USART AESTHETICS
void USART3_split(void)
{
	USART3_transmit_string("------------------------------------------------------------------------------------------------------");
}

void USART3_program_start(void)
{
	USART3_transmit_string("Write command: ");
}

void USART3_program_end(void)
{
	USART3_transmit_string("See you later!");
	USART3_new_line();
	USART3_split();
	USART3_new_line();
	USART3_new_line();
}

void USART3_command_start(void)
{
	USART3_split();
	USART3_new_line();
}

void USART3_command_end(void)
{
	USART3_new_line();
	USART3_split();
	USART3_new_line();
	USART3_new_line();
	USART3_new_line();
	USART3_transmit_string("Write command: ");
}

// USART COMMAND HANDLERS
void USART3_fan_control_command_handler(char argument0[], char argument1[])
{	
	if (strcmp(argument0, "Automatic") == 0)  // If the argument is "Automatic"
	{
		USART3_fan_control_automatic();
	}
	else if (strcmp(argument0, "All") == 0) // If the argument is "All"
	{
		USART3_fan_control_all(atoi(argument1));
	}
	else if ((strcmp(argument0, "0") == 0) | (strcmp(argument0, "1") == 0)) // If the argument is a number [0, 1]
	{
		uint8_t argument_number = atoi(argument0); // Convert agrument0 from sting to number
		
		USART3_fan_control_number(argument_number, atoi(argument1));
	}
	else // If the argument is not "All" or a number [0, 1]
	{
		USART3_wrong_argument0("Fan_Control", argument0); // Print error message
	}
}

void USART3_read_fan_command_handler(char argument0[], char argument1[])
{
	uint8_t argument_number = atoi(argument0); // Convert agrument0 from sting to number

	if (strcmp(argument0, "All") == 0) // If the argument is "All"
	{
		USART3_read_fan_all();
	}
	else if ((strcmp(argument0, "0") == 0) | ((1 <= argument_number) && (argument_number <= 7))) // If the argument is a number [0, 7]
	{	
		USART3_read_fan_number(argument_number);
	}
	else // If the argument is not "All" or a number [0, 7]
	{
		USART3_wrong_argument0("Read_Fan", argument0); // Print error message
	}	
}

void USART3_read_EEPROM_command_handler(char argument0[], char argument1[])
{
	uint8_t argument_number = atoi(argument0); // Convert agrument0 from sting to number
	
	if (strcmp(argument0, "All") == 0) // If the argument is "All"
	{
		if (strcmp(argument1, "Array") == 0) // If the argument is "Array"
		{
			USART3_read_EEPROM_array_all();
		}
		else if (strcmp(argument1, "Average") == 0) // If the argument is "Average"
		{
			USART3_read_EEPROM_average_all();
		}
		else
		{
			USART3_wrong_argument1("Read_EEPROM", argument0, argument1); // Print error message
		}
	}
	else if ((strcmp(argument0, "0") == 0) | ((1 <= argument_number) && (argument_number <= 7))) // If the argument is a number [0, 7]
	{
		if (strcmp(argument1, "Array") == 0) // If the argument is "Array"
		{
			USART3_read_EEPROM_array_number(argument_number);
		}
		else if (strcmp(argument1, "Average") == 0) // If the argument is "Average"
		{
			USART3_read_EEPROM_average_number(argument_number);
		}
		else
		{
			USART3_wrong_argument1("Read_EEPROM", argument0, argument1); // Print error message
		}
	}
	else // If the argument is not "All" or a number [0, 7]
	{
		USART3_wrong_argument0("Read_EEPROM", argument0); // Print error message
	}
}

void USART3_reset_EEPROM_command_handler(char argument0[])
{
	if (strcmp(argument0, "All") == 0)
	{
		USART3_reset_EEPROM_all();
	}
	else if (strcmp(argument0, "Iteration") == 0)
	{
		USART3_reset_EEPROM_iteration();
	}
	else
	{
		USART3_wrong_argument0("Reset_EEPROM", argument0); // Print error message
	}
}
void USART3_help_command_handler(char argument0[])
{
	if (strcmp(argument0, NULL) == 0) // If the argument is ""
	{
		USART3_help(); // Print "help" message
	}
	else if (strcmp(argument0, "Fan_Control") == 0) // If the argument is "Fan_Control"
	{
		USART3_help_fan_control(); // Print "help fan control" message
	}
	else if (strcmp(argument0, "Read_Fan") == 0) // If the argument is "Read_Fan"
	{
		USART3_help_read_fan(); // Print "help read fan" message
	}
	else if (strcmp(argument0, "Read_EEPROM") == 0) // If the argument is "Read_EEPROM"
	{
		USART3_help_read_EEPROM(); // Print "help read EEPROM" message
	}
	else if (strcmp(argument0, "Update_EEPROM") == 0) // If the argument is "Update_EEPROM"
	{
		USART3_help_update_EEPROM(); // Print "help update EEPROM" message
	}
	else if (strcmp(argument0, "Reset_EEPROM") == 0) // If the argument is "Reset_EEPROM"
	{
		USART3_help_reset_EEPROM(); // Print "help reset EEPROM" message
	}
	else  // If the argument is not an valid argument
	{
		USART3_wrong_argument0("Help", argument0);
	}
}

// USART COMMANDS
// Fan Control
void USART3_fan_control_automatic(void)
{
	RTC_ISR_enable();
	
	USART3_command_start();
	USART3_transmit_string("Fan control is set to automatic. The fan speed will now be determined by temperature.");
	USART3_command_end();
}
void USART3_fan_control(uint8_t fan_number, uint8_t PWM_duty_cycle)
{
	char char_PWM_duty_cycle[6];

	if (fan_number == 0)
	{
		set_fan_speed_first(PWM_duty_cycle);
		USART3_transmit_string("Fan number 0-3 is set to: ");
		sprintf(char_PWM_duty_cycle, "%d", PWM_duty_cycle);
		USART3_transmit_string(char_PWM_duty_cycle);
		USART3_transmit_string(".");
	}
	else if (fan_number == 1)
	{
		set_fan_speed_second(PWM_duty_cycle);
		USART3_transmit_string("Fan number 4-7 is set to: ");
		sprintf(char_PWM_duty_cycle, "%d", PWM_duty_cycle);
		USART3_transmit_string(char_PWM_duty_cycle);
		USART3_transmit_string(".");
	}
	else
	{
		USART3_transmit_string("Something went wrong.");
	}
}

void USART3_fan_control_number(uint8_t fan_number, uint8_t PWM_duty_cycle)
{
	RTC_ISR_disable();
	
	USART3_command_start();
	USART3_fan_control(fan_number, PWM_duty_cycle);
	USART3_command_end();
}

void USART3_fan_control_all(uint8_t PWM_duty_cycle)
{
	RTC_ISR_disable();
	
	USART3_command_start();
	
	for (uint8_t i = 0; i < 2; i++)
	{
		USART3_fan_control(i, PWM_duty_cycle);
		if (i < (2-1))
		{
			USART3_new_line();
		}
	}
	
	USART3_command_end();
}

// Read Fan
void USART3_read_fan(uint8_t fan_number)
{
	uint16_t RPM = read_fan_speed(fan_number);

	char char_fan_number[1];
	char char_RPM[4];	

	USART3_transmit_string("Fan number ");
	sprintf(char_fan_number, "%d", fan_number);
	USART3_transmit_string(char_fan_number);
	USART3_transmit_string(" has a measured RPM of: ");
	sprintf(char_RPM, "%d", RPM);
	USART3_transmit_string(char_RPM);
	USART3_transmit_string(".");
}

void USART3_read_fan_number(uint8_t fan_number)
{
	USART3_command_start();
	USART3_read_fan(fan_number);
	USART3_command_end();
}

void USART3_read_fan_all(void)
{
	USART3_command_start();
	
	for (uint8_t i = 0; i < 8; i++)
	{
		USART3_read_fan(i);
		if (i < (8-1))
		{
			USART3_new_line();
		}
	}
	
	USART3_command_end();
}

// Read EEPROM
void USART3_read_EEPROM_array(uint8_t fan_number)
{
	uint16_t array[TREND_DATA_SIZE];
	EEPROM_read_RPM_array(fan_number, array);
	
	char char_fan_number[1];
	
	USART3_transmit_string("Fan number ");
	sprintf(char_fan_number, "%d", fan_number);
	USART3_transmit_string(char_fan_number);
	USART3_transmit_string("'s last ten measurements are: [");
	
	for (uint8_t i = 0; i < TREND_DATA_SIZE; i++)
	{
		char char_array[5];
		sprintf(char_array, "%d", array[i]);
		USART3_transmit_string(char_array);
		
		if (i < (TREND_DATA_SIZE-1))
		{
			USART3_transmit_string(", ");
		}
	}
	
	USART3_transmit_string("].");
}

void USART3_read_EEPROM_array_number(uint8_t fan_number)
{
	USART3_command_start();
	USART3_read_EEPROM_array(fan_number);
	USART3_command_end();
}

void USART3_read_EEPROM_array_all(void)
{
	USART3_command_start();
	
	for (uint8_t i = 0; i < 8; i++)
	{
		USART3_read_EEPROM_array(i);
		if (i < (8-1))
		{
			USART3_new_line();
		}
	}
	
	USART3_command_end();
}

void USART3_read_EEPROM_average(uint8_t fan_number)
{
	uint16_t array[TREND_DATA_SIZE];
	uint16_t average = EEPROM_read_RPM_average(fan_number, array);

	char char_fan_number[1];
	char char_average[5];
	
	USART3_transmit_string("Fan number ");
	sprintf(char_fan_number, "%d", fan_number);
	USART3_transmit_string(char_fan_number);
	USART3_transmit_string(" has a average RPM of: ");
	sprintf(char_average, "%d", average);
	USART3_transmit_string(char_average);
	USART3_transmit_string(" over the last 10 measurements.");
}

void USART3_read_EEPROM_average_number(uint8_t fan_number)
{
	USART3_command_start();
	USART3_read_EEPROM_average(fan_number);
	USART3_command_end();	
}

void USART3_read_EEPROM_average_all(void)
{
	USART3_command_start();
	
	for (uint8_t i = 0; i < 8; i++)
	{
		USART3_read_EEPROM_average(i);
		if (i < (8-1))
		{
			USART3_new_line();
		}
	}
	
	USART3_command_end();	
}

// Update Trend Data
void USART3_update_trend_data(void)
{
	USART3_command_start();
	USART3_transmit_string("RPM values from each fan is being measured...\r\n");
	EEPROM_measure_and_update_RPM_all();
	USART3_transmit_string("EEPROM UPDATE COMPLETED.");
	USART3_command_end();
}

// Reset EEPROM
void USART3_reset_EEPROM_all(void)
{
	USART3_command_start();
	USART3_transmit_string("All EEPROM will be reset to 0...\r\n");
	
	for(uint8_t i = 0; i <= ITERATION_ADDRESS; i++)
	{
		EEPROM_update(i, 0);	
	}
	
	USART3_transmit_string("RESET COMPLETED.");
	USART3_command_end();	
}

void USART3_reset_EEPROM_iteration(void)
{
	USART3_command_start();
	USART3_transmit_string("The iteration number will be reset to 0...\r\n");
	EEPROM_update(ITERATION_ADDRESS, 0);
	USART3_transmit_string("RESET COMPLETED.");
	USART3_command_end();
}

// Help
void USART3_help(void)
{
	USART3_command_start();
	USART3_transmit_string("There are 7 different commands registered:\r\n");
	USART3_transmit_string("*Fan_Control <fan_number> <fan_speed>.\r\n");
	USART3_transmit_string("*Read_Fan <fan_number>.\r\n");
	USART3_transmit_string("*Read_EEPROM <fan_number> <Array/Average>.\r\n");
	USART3_transmit_string("*Update_EEPROM.\r\n");
	USART3_transmit_string("*Reset_EEPROM <All/Iteration>.\r\n");
	USART3_transmit_string("*End.\r\n");
	USART3_transmit_string("*Help <command>.");
	USART3_command_end();
}

void USART3_help_fan_control(void)
{
	USART3_command_start();
	USART3_transmit_string("Fan_Control is a command that lets the user control the fan speed.\r\n");
	USART3_transmit_string("The command syntax is 'Fan_Control <fan_number> <fan_speed>'.\r\n");
	USART3_transmit_string("<fan_number> is either a number from 0 to 1, the word 'All' or the word 'Automatic'.\r\n");
	USART3_transmit_string("<fan_speed> is a percentage from 0 to 100.");
	USART3_command_end();
}

void USART3_help_read_fan(void)
{
	USART3_command_start();
	USART3_transmit_string("Read_Fan is a command that lets the user read the speed of fans.\r\n");
	USART3_transmit_string("The command syntax is 'Read_Fan <fan_number>'.\r\n");
	USART3_transmit_string("<fan_number> is either a number from 0 to 7, or the word 'All'.");
	USART3_command_end();
}

void USART3_help_read_EEPROM(void)
{
	USART3_command_start();
	USART3_transmit_string("Read_EEPROM is a command that lets the user see the data saved on fans in EEPROM.\r\n");
	USART3_transmit_string("The command syntax is 'Read_EEPROM <fan_number> <Array/Average>'.\r\n");
	USART3_transmit_string("<fan_number> is either a number from 0 to 7, or the word 'All'.\r\n");
	USART3_transmit_string("<Array/Average> determines whether the command returns the array or the average.");
	USART3_command_end();
}

void USART3_help_update_EEPROM(void)
{
	USART3_command_start();
	USART3_transmit_string("Update_EEPROM is a command that lets the user update the data saved on fans in EEPROM manually.\r\n");
	USART3_transmit_string("The command syntax is 'Update_EEPROM'.");
	USART3_command_end();
}

void USART3_help_reset_EEPROM(void)
{
	USART3_command_start();
	USART3_transmit_string("Reset_EEPROM is a command that lets the reset the data saved in EEPROM.\r\n");
	USART3_transmit_string("The command syntax is 'Reset_EEPROM <All/Iteration>'.\r\n");
	USART3_transmit_string("<All/Iteration> determines whether the command resets all EEPROM used or just the iteration number.");
	USART3_command_end();
}

// End
uint8_t USART3_end(void)
{
	USART3_command_start();
	USART3_program_end();
	return 1;
}

// Error
void USART3_no_command(char command[])
{
	USART3_command_start();
	USART3_transmit_string("'");
	USART3_transmit_string(command);
	USART3_transmit_string("'");
	USART3_transmit_string(" is not a registered command. Try again. If you need help write 'Help'.");
	USART3_command_end();
}

void USART3_wrong_argument0(char command[], char argument0[])
{
	USART3_command_start();
	USART3_transmit_string("'");
	USART3_transmit_string(command);
	USART3_transmit_string("' has no agrument '");
	USART3_transmit_string(argument0);
	USART3_transmit_string("'.");
	USART3_command_end();
}

void USART3_wrong_argument1(char command[], char argument0[], char argument1[])
{
	USART3_command_start();
	USART3_transmit_string("'");
	USART3_transmit_string(command);
	USART3_transmit_string("' with first argument '");
	USART3_transmit_string(argument0);
	USART3_transmit_string("' has no second argument '");
	USART3_transmit_string(argument1);
	USART3_transmit_string("'.");
	USART3_command_end();
}