
#pragma once

#include "ADC.h"
#include "PWM.h"
#include "USART.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define RTC_INTERRUPT_PERIODE 0x258 // 10 minutes (600 seconds)

#define MINIMUM_TEMPERATURE_VALUE 15 // Value where fans ar set to 0
#define MAXIMUM_TEMPERATURE_VALUE 35 // Value where fans ar set to 100

/*
* Interrupt Service Routine
* -----------------------
* When overflow is triggered at the Real Time Counter
* the temperature is measured and the fans rotation speed is adjusted
*
* returns: void
*/
// ISR(RTC_CNT_vect)

/*
* Function: CLK_initialize
* -----------------------
* Initializes main clock to run on a frequency of 16 000 000 Hz
*
* returns: void
*/
void CLK_initialize(void);

/*
* Function: RTC_initialize
* -----------------------
* Initializes Real Time Counter for causing interrupts in 'Automatic' mode
*
* returns: void
*/
void RTC_initialize(void);

/*
* Function: RTC_ISR_enable
* -----------------------
* Enables interrupts for overflow in the Real Time Counter
*
* returns: void
*/
void RTC_ISR_enable(void);

/*
* Function: RTC_ISR_disable
* -----------------------
* Disables interrupts for overflow in the Real Time Counter
*
* returns: void
*/
void RTC_ISR_disable(void);

/*
* Function: TCA0_initialize
* -----------------------
* Initializes Timer A0 for use in controlling PWM signal for fan speed
*
* returns: void
*/
void TCA0_initialize(void);

/*
* Function: TCB0_initialize
* -----------------------
* Initializes Timer B0 for use in measuring frequency
*
* returns: void
*/
void TCB0_initialize(void);