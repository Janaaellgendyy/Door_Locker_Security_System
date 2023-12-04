 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name:PWM.h
 *
 * Description: Header file for the PWM driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include<avr/io.h>
#include"std_types.h"
#include"gpio.h"

#ifndef PWM_H_
#define PWM_H_


#define TIMER0_PORT PORTB_ID
#define OC0_PIN PIN3_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Function Name : PWM_Timer0_Start
 * Description : The function responsible for trigger the Timer0 with the PWM Mode.
 * 				 Setup the PWM mode with Non-Inverting.
 * Parameters : duty_cycle: OCR duty cycle Percentage.
 * Return : void.
 */

void PWM_Timer0_Start(uint8 duty_cycle);



#endif /* PWM_H_ */
