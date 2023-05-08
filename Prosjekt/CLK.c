
#include "CLK.h"

ISR(RTC_CNT_vect)
{
	// Get the current temperature in Celsius
	uint16_t temp = get_temperature_celcius();
	
	USART3_transmit_string("NO COMMAND");
	USART3_new_line();
	USART3_split();
	USART3_new_line();
	USART3_transmit_string("TEMPERATURE MEASURED TO: ");
	
	char char_temp[5];
	sprintf(char_temp, "%d", temp);
	USART3_transmit_string(char_temp);
	
	
	USART3_transmit_string(".");
	
	// If the temperature is less than 15°C, turn off both fans
	if (temp < MINIMUM_TEMPERATURE_VALUE)
	{
		set_fan_speed_both(0);
	}
	
	// If the temperature is greater than 35°C, set both fans to 100% speed
	else if (temp > MAXIMUM_TEMPERATURE_VALUE)
	{
		set_fan_speed_both(100);
	}
	
	// If the temperature is between 15°C and 35°C, set fan speed based on temperature
	else
	{
		temp = (temp - MINIMUM_TEMPERATURE_VALUE) * (100/(MAXIMUM_TEMPERATURE_VALUE - MINIMUM_TEMPERATURE_VALUE));
		set_fan_speed_both(temp);
	}

	// Clear the RTC overflow interrupt flag
	RTC.INTFLAGS = RTC_OVF_bm;
	
	USART3_new_line();
	USART3_transmit_string("FAN SPEED UPDATED.\r\n");
	USART3_split();
	USART3_transmit_string("\r\n\r\n\r\n");
	USART3_program_start();
}

void CLK_initialize(void)
{
	// Set the frequency selection to 16 MHz
	_PROTECTED_WRITE(CLKCTRL.OSCHFCTRLA,(CLKCTRL_FRQSEL_16M_gc));
}

void RTC_initialize(void)
{
	// Enable the external 32 kHz oscillator
	_PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, CLKCTRL_ENABLE_bm);

	// Set the RTC clock source to the external 32 kHz oscillator
	RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc;

	// Set the RTC period to 600 (0x258) - this is used for overflow detection
	RTC.PER = RTC_INTERRUPT_PERIODE;
	
	RTC.CTRLA = RTC_PRESCALER_DIV32768_gc;
}

void RTC_ISR_enable(void)
{
	// Enable the RTC overflow interrupt
	RTC.INTCTRL = RTC_OVF_bm;
	
	// Set the RTC prescaler to DIV32768 and enable the RTC
	RTC.CTRLA |= RTC_RTCEN_bm;
}

void RTC_ISR_disable(void)
{
	// Disable the RTC overflow interrupt
	RTC.INTCTRL &= ~RTC_OVF_bm;
	
	RTC.CTRLA &= ~RTC_RTCEN_bm;
}

void TCA0_initialize(void)
{
	// Set PWM signal output on PORT D
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
	
	// Set counter to dual-slope wave generation mode and enable compare channel 0 and 1 for PWM signal
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_DSBOTTOM_gc | TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm;
	
	// Set timer to count clock ticks
	TCA0.SINGLE.EVCTRL &= ~(TCA0_SINGLE_CNT);
	
	// Set PWM frequency with period value
	TCA0.SINGLE.PER = 0xFF;
	
	// Set clock source to half of system clock(sys_clk /2) and start the timer
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm;
	
	// Set pin 0 and 1 of PORT D as output. These ports will be used to output PWM signals
	PORTD.DIR |= PIN0_bm | PIN1_bm;
}

void TCB0_initialize(void)
{
	// Configure PORTC
	PORTC.DIRCLR = 0x00;
	
	// Set the compare value for TCB0 to 0x00
	TCB0.CCMP = 0x00;
	
	// Set the count value for TCB0 to 0x00
	TCB0.CNT = 0x00;
	
	// Enable TCB0 and select clock divider 1 as the clock source
	TCB0.CTRLA |= TCB_ENABLE_bm | TCB_CLKSEL_DIV1_gc;
	
	// Enable TCB0 compare/capture and set it to frequency generation mode
	TCB0.CTRLB |= TCB_CCMPEN_bm | TCB_CNTMODE_FRQ_gc;
	
	// Enable input capture event for TCB0
	TCB0.EVCTRL |= TCB_CAPTEI_bm;
}