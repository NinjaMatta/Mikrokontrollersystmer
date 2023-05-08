
#include "PWM.h"

void increase_fan_speed_first(uint16_t PWM_duty_cycle)
{
	// Increase fan speed by one every 10 ms until fan has reached desired speed
	for (uint16_t i = TCA0.SINGLE.CMP0; i < PWM_duty_cycle; i++)
	{
		TCA0.SINGLE.CMP0 = PWM_duty_cycle;
		PORTD.DIR |= PIN0_bm;
		_delay_ms(10);
	}
}

void increase_fan_speed_second(uint16_t PWM_duty_cycle)
{
	// Increase fan speed by one every 10 ms until fan has reached desired speed
 	for (uint16_t i = TCA0.SINGLE.CMP1; i < PWM_duty_cycle; i++)
 	{
		TCA0.SINGLE.CMP1 = PWM_duty_cycle;
		PORTD.DIR |= PIN1_bm;
		_delay_ms(10);
	}
}

void decrease_fan_speed_first(uint16_t PWM_duty_cycle)
{
	// Decrease fan speed by one every 10 ms until fan has reached desired speed
 	for (uint16_t i = TCA0.SINGLE.CMP0; i > PWM_duty_cycle; i--)
	{
		TCA0.SINGLE.CMP0 = PWM_duty_cycle;
		PORTD.DIR |= PIN0_bm;
		_delay_ms(10);
	}
}

void decrease_fan_speed_second(uint16_t PWM_duty_cycle)
{
	// Decrease fan speed by one every 10 ms until fan has reached desired speed
	 for (uint16_t i = TCA0.SINGLE.CMP1; i > PWM_duty_cycle; i--)
	 {
		TCA0.SINGLE.CMP1 = PWM_duty_cycle;
		PORTD.DIR |= PIN1_bm;
		_delay_ms(10);
	}
}

void set_fan_speed_first(uint8_t fan_speed)
{
	// Changes percent values over 0 and under 10 to 10
	if ((fan_speed > 0) && (fan_speed < 10))
	{
		fan_speed = 10;
	}
	
	// Transforms percentage value (0-100) to duty cycle value (0-0xFF)
	fan_speed = 100-fan_speed;
	uint16_t PWM_duty_cycle = (0xFF*fan_speed*0.01);
	
	// If fan speed input into fuction is larger than current fan speed 
	// increase the fan speeed
	if (TCA0.SINGLE.CMP0 < PWM_duty_cycle)
	{
		increase_fan_speed_first(PWM_duty_cycle);
	}
	
	// If fan speed input into fuction is smaller than current fan speed 
	// decrease the fan speed
	else if (TCA0.SINGLE.CMP0 > PWM_duty_cycle)
	{
		decrease_fan_speed_first(PWM_duty_cycle);
	}
}

void set_fan_speed_second(uint8_t fan_speed)
{
	// Changes percent values over 0 and under 10 to 10
	if ((fan_speed > 0) && (fan_speed < 10))
	{
		fan_speed = 10;
	}
	
	// Transforms percentage value (0-100) to duty cycle value (0-0xFF)
	fan_speed = 100-fan_speed;
	uint16_t PWM_duty_cycle = (0xFF*fan_speed*0.01);
	
	// If desired fan speed is set higher increase fan speed
	if (TCA0.SINGLE.CMP1 < PWM_duty_cycle)
	{
		increase_fan_speed_second(PWM_duty_cycle);
	}
	
	// If fan speed input into fuction is smaller than current fan speed 
	// decrease the fan speed
	else if (TCA0.SINGLE.CMP1 > PWM_duty_cycle)
	{
		decrease_fan_speed_second(PWM_duty_cycle);
	}
}

void set_fan_speed_both(uint8_t fan_speed)
{
	set_fan_speed_first(fan_speed);
	set_fan_speed_second(fan_speed);
}