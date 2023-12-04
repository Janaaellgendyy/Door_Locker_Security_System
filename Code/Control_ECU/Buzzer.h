/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name:Buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"
#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN PIN4_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description:
 *    Setup the direction for the buzzer pin as output pin through the
   	  GPIO driver.
 *    Turn off the buzzer through the GPIO
*/
 void Buzzer_init();

 /* Description:
  *    Function to enable the Buzzer through the GPIO
 */
 void Buzzer_on(void);

 /* Description:
  *    Function to disable the Buzzer through the GPIO
 */
 void Buzzer_off(void);

#endif /* BUZZER_H_ */
