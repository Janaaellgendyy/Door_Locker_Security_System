/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name:Buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include "Buzzer.h"


/* Description:
 *    Setup the direction for the buzzer pin as output pin through the
   	  GPIO driver.
 *    Turn off the buzzer through the GPIO
*/
void Buzzer_init(){

	/*
	* set up pin direction of the Buzzer
	* set it OFF initially
	*/

	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}


/* Description:
 *    Function to enable the Buzzer through the GPIO
*/
void Buzzer_on(void){

	/*Turn ON the Buzzer*/
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}


/* Description:
 *    Function to disable the Buzzer through the GPIO
*/
void Buzzer_off(void){

	/*Turn OFF the Buzzer*/
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
