/******************************************************************************
 *
 * Module: HMI_ECU
 *
 * File Name: HMI_ECU.c
 *
 * Description: Source file for the HMI_ECU
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "Timer1.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*commands used to sync between HMI_ECU and CONTROL_ECU */
#define CHANGE_PASSWORD '-'
#define OPEN_DOOR '+'
#define ENTER '='
#define PASSWORDS_NOT_MATCH 0xFF
#define PASSWORDS_MATCH 0xF1
#define PASSWORD1 0x02
#define PASSWORD2 0x03
#define PASSWORD_ADDRESS_LOCATION 0x50
#define PASSWORD_MATCH_ADDRESS_LOCATION 0x60
#define WRONG_TRIAL_3 0x20
#define HOLD_TIME_IN_SECONDS 3
#define OPEN_TIME_IN_SECONDS 15
#define CLOSE_TIME_IN_SECONDS 15
#define BUZZER_TIME_IN_SECONDS 60
#define MC1_READY 			0x10
#define MC2_READY 			0x20
#define SAVE 				0xA1
#define END_SAVING 			0xA2
#define CHECK_PASSWORD      0xA3


uint8 g_passFLag = 0;

/* a global variable to store the entered password*/
uint8 g_staticPassword[6];
/* a global variable to keep track of the received byte from contror_ECU*/
uint8 g_receivedByte;
/* a global variable to count seconds*/
uint8 g_secondsCounter = 0;


/* TIMER1 configurations:
 *		initial_value = 0,
 *		compare_value = 31250 ticks per 1 second,
 *		pre-scaler = 256
 *		CTC mode of operation
 */
Timer1_ConfigType Timer1_config_ptr = { 0, TICKS_PER_ONE_SECOND, prescaler_256, CTC_Mode };


/* Function Name : checkNumber
 * Description : checks if number is from 0 to 9 or not
 * Parameters : num.
 * Return : uint8 value.
 */
uint8 checkNumber(uint8 num){
	uint8 value;
	value = (num >= 0 && num <= 9);
	return value;
}

void recievePassword(void){

	uint8 counter = 0;

	uint8 pressedKey;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "plz enter pass:");
	LCD_displayStringRowColumn(1, 0, "pass is 5 digits");

	_delay_ms(1000);

	LCD_clearScreen();

	LCD_displayStringRowColumn(0, 0, "plz enter pass:");

	LCD_moveCursor(1, 0);

	while(counter < 5){

		pressedKey = KEYPAD_getPressedKey();

		if(checkNumber(pressedKey)){

			LCD_displayCharacter('*');

			UART_sendByte(pressedKey);

			counter++;
		}
		_delay_ms(500);
	}

	pressedKey = KEYPAD_getPressedKey();

	while(KEYPAD_getPressedKey() != ENTER){

		LCD_displayStringRowColumn(0, 0, "Press '='");
	}

	_delay_ms(500);

	/*re-taking password to proceed with checking for password validity*/
	LCD_clearScreen();

	LCD_displayStringRowColumn(0, 0, "plz re-enter");
	LCD_displayStringRowColumn(1, 0, "pass:");
	LCD_moveCursor(1, 6);

	counter = 0;

	while(counter < 5){

		pressedKey = KEYPAD_getPressedKey();

		if(checkNumber(pressedKey)){

			LCD_displayCharacter('*');

			UART_sendByte(pressedKey);

			counter++;
		}
		_delay_ms(500);
	}

	/*wait until the entered key is = to continue*/
	while (KEYPAD_getPressedKey() != ENTER) {
		LCD_displayStringRowColumn(0, 0, "Press '='");
	}

	/*receive the byte that indicates if  passwords match or not*/
	g_receivedByte = UART_receiveByte();
	_delay_ms(100);

}

/* Function Name : verifyPasswoord
 * Description : take password from user to send it to CONTROL_ECU
 * then receive a byte from CONTROL_ECU to check validity of password
 * Parameters : void.
 * Return : void.
 */
void verifyPassword(void){

	uint8 pressedKey;
	uint8 counter = 0;

	while(counter < 5){

		pressedKey = KEYPAD_getPressedKey();

		if(checkNumber(pressedKey)){

			LCD_displayCharacter('*');

			UART_sendByte(pressedKey);

			counter++;
		}
		_delay_ms(500);
	}


	while (KEYPAD_getPressedKey() != ENTER) {
		LCD_displayStringRowColumn(0, 0, "Press '='");
	}

	/*receive the byte that indicates if passwords match or not*/
	g_receivedByte = UART_receiveByte();
	_delay_ms(100);


}


/* Function Name : buzzerOperation
 * Description : Activate Buzzer for 1-minute.
 *  Display error message on LCD for 1 minute.
 *  System should be locked no inputs from Keypad will be accepted during
 *  this time period.
 * Parameters : void.
 * Return : void.
 */
void buzzerOperation(void){

	LCD_clearScreen();

	/*display error message*/
	LCD_displayStringRowColumn(0, 0, "ERROR!!!");

	/*start the timer and count 1 minute*/
	Timer1_init(&Timer1_config_ptr);

	g_secondsCounter = 0;

	/*display ERROR message for 1 minute*/
	while (g_secondsCounter != BUZZER_TIME_IN_SECONDS) {
	}

	g_secondsCounter = 0;

	/*stop the timer*/
	Timer1_deInit();

}

/* Function Name : doorOperation
 * Description : display a message on the screen “Door is Unlocking” for 15 seconds.
 * display hold for 3-seconds.
 * and display a message on the screen “Door is Locking for 15 seconds.
 * turn off timer1 and clear seconds counter
 * Parameters : void.
 * Return : void.
 */
void doorOperation(void){
	LCD_clearScreen();

	/*display a message to the user that the door in unlocking*/
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Unlocking...");

	/*start timer1*/
	Timer1_init(&Timer1_config_ptr);

	g_secondsCounter = 0;

	/*wait for 15 seconds*/
	while (g_secondsCounter <= OPEN_TIME_IN_SECONDS) {
	}

	LCD_clearScreen();

	/*display a message to the user that the door is on hold*/
	LCD_displayStringRowColumn(0, 0, "Hold Door...");

	g_secondsCounter = 0;

	/*hold door for 3 seconds*/
	while (g_secondsCounter <= HOLD_TIME_IN_SECONDS) {
	}

	LCD_clearScreen();

	/*display a message to the user that the door in locking*/
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Locking...");

	g_secondsCounter = 0;

	/*wait for 15 seconds*/
	while (g_secondsCounter <= CLOSE_TIME_IN_SECONDS) {
	}

	/*turn off the timer*/
	Timer1_deInit();

	g_secondsCounter = 0;

	/*display a message to the user that the door in locked*/
	LCD_displayStringRowColumn(0, 0, "Door is locked");

	_delay_ms(500);


}

/* Function Name : Timer1_INT
 * Description : the call back function that is called in Timer1's ISR
 * Parameters : void.
 * Return : void.
 */
void Timer1_INT(void) {

	g_secondsCounter++;
}


int main(void){

	SREG |= (1 << 7);
	uint8 pressedKey;

	UART_ConfigType UART_ConfigPtr = {BAUD_RATE_9600, SIZE_BIT_8, DISABLED, BIT_1};

	/*Call initialization function of UART driver*/
	UART_init(&UART_ConfigPtr);

	/*set the call back function for timer1 to count seconds*/
	Timer1_setCallBack(Timer1_INT);

	/* Call initialization function of LCD driver */
	LCD_init();

	while(1){
		/*ask the user to enter new password*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 5, "Create");
		LCD_displayStringRowColumn(1, 2, "New Password");
		_delay_ms(1500);

		UART_sendByte(PASSWORD1);

		/*receive password 2 times from the user*/
		recievePassword();

		/*if passwords match break the while loop*/
		if (g_receivedByte == PASSWORDS_MATCH)
			break;
		else {
			LCD_clearScreen();
			/*display a message if the passwords do not match while signing in*/
			LCD_displayStringRowColumn(0, 0, "Passwords don't");
			LCD_displayStringRowColumn(1, 0, "Match");
			_delay_ms(1000);

			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Try again");
			_delay_ms(500);
		}

	}

	while(1){
		LCD_clearScreen();
		/*display the main options*/
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Change pass");
		_delay_ms(100);


		/*wait until the user enters '+' to open door or '-' to change password*/
		while (1) {
			pressedKey = KEYPAD_getPressedKey();
			if (pressedKey == OPEN_DOOR || pressedKey == CHANGE_PASSWORD)
				break;
		}
		LCD_clearScreen();

		/*send PASSWORD22 to CONTROL_ECU to receive the password and verify it*/

		uint8 trialCounter;

		/*check for password validity
		 *user has only 3 trials for password entries
		 */
		for (trialCounter = 0; trialCounter < 3; ++trialCounter) {

			/*send to Control_ECU PASSWORD2 command */
			UART_sendByte(PASSWORD2);

			LCD_displayStringRowColumn(0, 0, "plz enter pass:");
			LCD_moveCursor(1, 6);

			verifyPassword();

			/*if the password is right break the loop*/
			if (g_receivedByte == PASSWORDS_MATCH) {
				break;
			}

			LCD_displayStringRowColumn(0, 0, "WRONG PASS     ");
			LCD_displayStringRowColumn(1, 0, "              ");
			_delay_ms(500);
		}

		/*if all 3 trials were wrong entries
		 * alert CONTROL_ECU
		 * turn buzzer on
		 */
		if (g_receivedByte == PASSWORDS_NOT_MATCH) {
			UART_sendByte(WRONG_TRIAL_3);
			buzzerOperation();
		}
		 /* if the pressed key is open door
		 * send to CONTROL_ECU open door command
		 */
		else if (pressedKey == OPEN_DOOR) {
			UART_sendByte('+');
			doorOperation();
		}
		/*
		 * if the pressed key is change password
		 * the user has 3 trials to enter the password and verify it
		 * if the user wronged all trials
		 * the buzzer is turned on for 1 minute
		 */
		else if (pressedKey == CHANGE_PASSWORD) {
			LCD_clearScreen();

			LCD_displayStringRowColumn(0, 5, "change");
			LCD_displayStringRowColumn(1, 2, "your password");

			_delay_ms(1000);

			uint8 trialCounter = 0;

			for (trialCounter = 0; trialCounter < 3; ++trialCounter) {

				UART_sendByte(CHANGE_PASSWORD);

				recievePassword();

				/*if passwords match break the for loop*/
				if (g_receivedByte == PASSWORDS_MATCH)
					break;

				else {
					/*if the password is wrong print a warning message to the user*/
					LCD_clearScreen();

					LCD_displayStringRowColumn(0, 0, "passwords don't");
					LCD_displayStringRowColumn(1, 0, "Match");

					_delay_ms(1000);

					LCD_clearScreen();

					/*if the user has wronged 3 trials he cannot try again*/
					if (trialCounter < 2)
						LCD_displayStringRowColumn(0, 0, "try again");

					_delay_ms(500);
				}
			}
			/*
			 * if the user enters password wrong for 3 times after choosing from the main menu
			 * send to Control_ECU the WRONG_TTIAL_3 command
			 * and turn on the buzzer
			 */
			if (g_receivedByte == PASSWORDS_NOT_MATCH) {
				UART_sendByte(WRONG_TRIAL_3);
				buzzerOperation();
			}
			/*
			 * in the change password option
			 * if the 2 passwords entered by the user match
			 */
			else if (g_receivedByte == PASSWORDS_MATCH) {

				LCD_clearScreen();

				/*display a message to indicate that the password changed*/
				LCD_displayStringRowColumn(0, 0, "Pass changed");

				_delay_ms(500);
			}

		}

	}


	}

