 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name:PWM.c
 *
 * Description: Source file for the PWM driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/
#include "PWM.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


void PWM_Timer0_Start(uint8 duty_cycle){

	TCNT0 = 0; // Set Timer Initial Value to 0

	OCR0 = (int)((float32)duty_cycle / 100.0 * 255 + 0.5);

	/*
	 * set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
	 */

	GPIO_setupPinDirection(TIMER0_PORT, OC0_PIN, PIN_OUTPUT);

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Non Inverted Mode COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */

	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
