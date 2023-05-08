
#pragma once

#include "PWM.h"
#include "EVSYS.h"
#include "EEPROM.h"
#include "CLK.h"

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define delete in ASCII character code
#define DELETE 127

// Define baud rate of USART3
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

/*
* Function: USART3_initialize
* -----------------------
* Initializes USART3 with 8-bit data, no parity and 1 stop bit
*
* returns: void
*/
void USART3_initialize(void);

/*
* Function: USART3_receive
* -----------------------
* Handles the USARTs received bytes at RX
*
* returns: The char received at RX
*/
char USART3_receive(void);

/*
* Function: USART3_transmit
* -----------------------
* Handles the USARTs request of transmitting
* bytes at TX
*
* returns: void
*/
void USART3_transmit(char character);

/*
* Function: USART3_transmit_string
* -----------------------
* Handles the USARTs request of transmitting
* multiple bytes at TX in from of a string
*
* returns: void
*/
void USART3_transmit_string(char *string);

/*
* Function: USART3_new_line
* -----------------------
* Handles the USARTs request of transmitting
* a new line
*
* returns: void
*/
void USART3_new_line(void);

/*
* Functions for USART AESTHETICS
* -----------------------
* The five functions below are functions for
* making the UI more user-friendly
*
* returns: void (but prints to USART)
*/
void USART3_split(void);
void USART3_program_start(void);
void USART3_program_end(void);
void USART3_command_start(void);
void USART3_command_end(void);

/*
* Functions for USART COMMAND HANDLERS
* -----------------------
* The five functions below are functions that handles
* each USART command. It calls the right functions based on which
* arguments the user has entered
*
* argument0[]: The first argument which decider
* argument1[]: The second argument which decider
* returns: void
*/
void USART3_fan_control_command_handler(char argument0[], char argument1[]);
void USART3_read_fan_command_handler(char argument0[], char argument1[]);
void USART3_read_EEPROM_command_handler(char argument0[], char argument1[]);
void USART3_reset_EEPROM_command_handler(char argument0[]);
void USART3_help_command_handler(char argument0[]);

// USART COMMANDS
/*
* Functions for Fan Control
* -----------------------
* The four functions below are functions called when
* the 'Fan_Control' command is entered correctly
*
* fan_number: The number of the fan the user wants to adjust
* PWM_duty_cycle: A percentage that decides the fan speed
* returns: void (but prints to USART)
*/
void USART3_fan_control_automatic(void);
void USART3_fan_control(uint8_t fan_number, uint8_t PWM_duty_cycle);
void USART3_fan_control_number(uint8_t fan_number, uint8_t PWM_duty_cycle);
void USART3_fan_control_all(uint8_t PWM_duty_cycle);

/*
* Functions for Read Fan
* -----------------------
* The three functions below are functions called when
* the 'Read_Fan' command is entered correctly
*
* fan_number: The number of the fan the user wants to adjust
* returns: void (but prints to USART)
*/
void USART3_read_fan(uint8_t fan_number);
void USART3_read_fan_number(uint8_t fan_number);
void USART3_read_fan_all(void);

/*
* Functions for Read EEPROM
* -----------------------
* The six functions below are functions called when
* the 'Read_EEPROM' command is entered correctly
*
* fan_number: The number of the fan the user wants to adjust
* returns: void (but prints to USART)
*/
void USART3_read_EEPROM(uint8_t fan_number);
void USART3_read_EEPROM_array_number(uint8_t fan_number);
void USART3_read_EEPROM_array_all(void);
void USART3_read_EEPROM_average(uint8_t fan_number);
void USART3_read_EEPROM_average_number(uint8_t fan_number);
void USART3_read_EEPROM_average_all(void);

/*
* Function: USART3_update_trend_data
* -----------------------
* The function below are functions called when
* the 'Update_EEPROM' command is entered correctly
*
* returns: void (but prints to USART)
*/
void USART3_update_trend_data(void);

/*
* Functions for Reset EEPROM
* -----------------------
* The two functions below are functions called when
* the 'Reset_EEPROM' command is entered correctly
*
* returns: void (but prints to USART)
*/
void USART3_reset_EEPROM_all(void);
void USART3_reset_EEPROM_iteration(void);

/*
* Functions for Help
* -----------------------
* The six functions below are functions called when
* the 'Help' command is entered correctly. The functions
* prints a guide for using the UI in USART
*
* returns: void (but prints to USART)
*/
void USART3_help(void);
void USART3_help_fan_control(void);
void USART3_help_read_fan(void);
void USART3_help_read_EEPROM(void);
void USART3_help_update_EEPROM(void);
void USART3_help_reset_EEPROM(void);

/*
* Function: USART3_end
* -----------------------
* The function ends the program. The user is no longer able to
* write new commands after running this command.
*
* returns: The number 1 (and prints to USART)
*/
uint8_t USART3_end(void);


/*
* Functions for Error
* -----------------------
* The three functions below are functions called when the
* user enters a command or argument that is not registered
* in this program.
*
* returns: void (but prints to USART)
*/
void USART3_no_command(char command[]);
void USART3_wrong_argument0(char command[], char argument0[]);
void USART3_wrong_argument1(char command[], char argument0[], char argument1[]);