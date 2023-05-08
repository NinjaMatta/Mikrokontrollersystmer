
#include "EVSYS.h"

uint32_t read_tacho(uint8_t fan_number)
{
	uint32_t frequency;

	// Set the EVSYS.CHANNEL2 register to the corresponding pin based on the 'fan_number'
	switch (fan_number)
	{
		// If 'fan_number' is not one of the cases below, do nothing and break out of the switch statement
		default:
			break;

		case 0:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN0_gc;
			break;

		case 1:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN1_gc;
			break;

		case 2:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN2_gc;
			break;

		case 3:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN3_gc;
			break;

		case 4:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN4_gc;
			break;

		case 5:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN5_gc;
			break;

		case 6:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN6_gc;
			break;

		case 7:
			EVSYS.CHANNEL2 = EVSYS_CHANNEL2_PORTC_PIN7_gc;
			break;
	}

	// Set the EVSYS.USERTCB0CAPT register to use the EVSYS.CHANNEL2 signal
	EVSYS.USERTCB0CAPT = EVSYS_USER_CHANNEL2_gc;

	// Read the frequency value from the TCB0.CCMP register and store it in 'frequency'
	frequency = TCB0.CCMP;

	// Clear the TCB0.CCMP register to prepare it for the next measurement
	TCB0.CCMP = 0;

	// Return the frequency value
	return frequency;
}

uint16_t frequency_to_RPM(uint32_t frequency)
{
	uint16_t RPM;

	// Calculate the RPM value based on the input 'frequency' and the system clock frequency
	RPM = (F_CPU * 60) / (frequency * 2);

	// Return the RPM value
	return RPM;
}

uint16_t read_fan_speed(uint8_t fan_number)
{
	uint32_t frequency;

	// Read the frequency of rotation of the specified fan using the 'read_tacho' function
	frequency = read_tacho(fan_number);

	// Convert the frequency value to an RPM value using the 'frequency_to_RPM' function
	uint16_t RPM = frequency_to_RPM(frequency);

	// Return the RPM value
	return RPM;
}
