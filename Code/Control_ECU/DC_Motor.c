/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name:DC_Motor.c
 *
 * Description: Source file for the DC Motor driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include "DC_Motor.h"

void DcMotor_Init(void){

/* setting direction of DC Motor pins to output pins */
	GPIO_setupPinDirection(DC_MOTOR_DIRECTION_PORT,DC_MOTOR_DIRECTION_PIN1, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_DIRECTION_PORT,DC_MOTOR_DIRECTION_PIN2, PIN_OUTPUT);


/*	making the DC Motor stop at the beginning */
	GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN1, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN2, LOGIC_LOW);


}

void DcMotor_Rotate(DcMotor_State state,uint8 speed){

	switch (state){
	case STOP:

//		stop the motor.
		GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN1, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN2, LOGIC_LOW);
		break;
	case CW:

//		Rotate the motor  Clockwise
		GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN1, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN2, LOGIC_HIGH);
		break;
	case ANTI_CW:

//		Rotate the motor Anti Clockwise
		GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN1, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_DIRECTION_PORT, DC_MOTOR_DIRECTION_PIN2, LOGIC_LOW);
		break;

	}

	PWM_Timer0_Start(speed);


}
