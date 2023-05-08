
#include "ADC.h"

void ADC0_initialize(void)
{	
	// Turn on VDD voltage reference for the ADC
	VREF.ADC0REF = VREF_REFSEL_VDD_gc;
	
	// ADC is enabled and set to 10-bit resolution
	ADC0.CTRLA = ADC_RESSEL_12BIT_gc | ADC_ENABLE_bm;

	// ADC prescaler is set to divide clock period by 2
	ADC0.CTRLC = ADC_PRESC_DIV2_gc;
	
	// Connect ADC to PD4
	ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;
	
	// Negative ADC inputs are connected to ground
	ADC0.MUXNEG = ADC_MUXNEG_GND_gc;
}

uint16_t ADC0_make_measurement(void)
{
	// Start converting signal from analog to digital.
	// Since ADC is sett to free run this starts the constant conversion
	ADC0.COMMAND = ADC_STCONV_bm;
	
	// Waits for ADC conversion to be done
	while(ADC0.COMMAND && ADC_STCONV_bm);
	
	// Returns measured value form ADC
	return ADC0.RES;
}

uint16_t get_temperature_celcius(void) 
{
	// Store measured ADC value in a variable
	uint16_t ADC0_value = ADC0_make_measurement();
	
	// Calculating temperature through the use of B parameter equation from 
	// (https://en.wikipedia.org/wiki/Thermistor#B_or_%CE%B2_parameter_equation)
	float ADC_resistance = (float)(4095-ADC0_value)/(ADC0_value) * (10000);
	float tempKelvin = 1 / ((1.0/298.15) + ((float)(log(ADC_resistance/10000)) / (3950.0)));
	uint8_t tempCelcius = tempKelvin - 273.15;
	return tempCelcius;
}