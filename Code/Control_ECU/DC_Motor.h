/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name:DC_Motor.h
 *
 * Description: Header file for the DC Motor driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include"std_types.h"
#include "gpio.h"
#include "PWM.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_DIRECTION_PORT PORTA_ID
#define DC_MOTOR_DIRECTION_PIN1 PIN0_ID
#define DC_MOTOR_DIRECTION_PIN2 PIN1_ID

/*DESCRIPTION: enum for DC motor state*/

typedef enum{
	STOP, CW, ANTI_CW
}DcMotor_State;


/*
 * Function Name : DcMotor_Init
 * Description : Function responsible for setting the direction for the two motor pins through the GPIO driver
 * Parameters : void.
 * Return : void.
 */
 void DcMotor_Init(void);

 /*
  * Function Name : DcMotor_Rotate
  * Description : The function responsible for rotate the DC Motor CW/ or A-CW or
  *  			  stop the motor based on the state input state value.
  * Parameters : DcMotor_State: state of the Motor (STOP, CW, ANTI_CW),
  * 			 Speed : (NO_SPEED, QUARTER_mAXIMUM_SPEED, HALF_mAXIMUM_SPEED, FULL_mAXIMUM_SPEED).
  * Return : void.
  */

 void DcMotor_Rotate(DcMotor_State state,uint8 speed);



#endif /* DC_MOTOR_H_ */
