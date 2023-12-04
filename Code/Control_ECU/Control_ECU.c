/******************************************************************************
 *
 * Module: Control_ECU
 *
 * File Name: Control_ECU.c
 *
 * Description: Source file for the Control_ECU
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "Timer1.h"
#include "I2c.h"
#include "DC_motor.h"
#include "buzzer.h"
#include "std_types.h"
#include "external_eeprom.h"



/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*commands used to sync between HMI_ECU and CONTROL_ECU*/
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


/* Function Name : savePassword
 * Description : store the password received from HMI_ECU
 *store the re-entered password received from HMI_ECU
 *compare the 2 passwords and store the password if it is right
 *send a command to HMI_ECU whether the passwords match or not
 * Parameters : void.
 * Return : void.
 */
void savePassword(void){
	uint8 password[6], passwordCheck[6];
	uint8 FLAG = PASSWORDS_MATCH;
	uint8 i;

	for(i = 0; i < 5; ++i){

		/*receive the password for the first time from HMI_ECU*/
		password[i] = UART_receiveByte();

		/* store the password in the EEPROM*/
		EEPROM_writeByte(PASSWORD_ADDRESS_LOCATION + i, password[i]);

		_delay_ms(500);
	}

	for(i = 0; i < 5; ++i){
		/*check for password match
		 * if not then clear flag*/

		passwordCheck[i] = UART_receiveByte();
		/* store the password in the EEPROM*/
		EEPROM_writeByte(PASSWORD_ADDRESS_LOCATION + i, passwordCheck[i]);

		_delay_ms(500);


	}

	for(i = 0; i < 5; ++i){
		if(password[i] != passwordCheck[i]){
			FLAG = PASSWORDS_NOT_MATCH;
			break;
		}
	}

	if(FLAG == PASSWORDS_MATCH){

		for(i = 0; i < 5; ++i){
			g_staticPassword[i] = password[i];
		}
	}

	/* send the flag to the HMI_ECU */
	UART_sendByte(FLAG);

}

/* Function Name : verifyPasswoord
 * Description : take password from CONTROL_ECU to verify whether the entered
 * password is same as the stored one or not
 * send a command to HMI_ECU of match validity
 * Parameters : void.
 * Return : void.
 */
void verifyPassword(void){
	uint8 password[6];
	uint8 FLAG = PASSWORDS_MATCH;
	uint8 i;

	for(i = 0; i < 5; ++i){

		/*Receive the password for second time in the re-entering phase*/
		password[i] = UART_receiveByte();

		_delay_ms(500);
	}

	for(i = 0; i < 5; ++i){
		/*check for password match
		* if not then clear flag*/
		if(password[i] != g_staticPassword[i]){
			FLAG = PASSWORDS_NOT_MATCH;
			break;
		}
	}

	/* send the flag to the HMI_ECU */
	UART_sendByte(FLAG);

}

void motorOperation(void){

	/*start timer1*/
	Timer1_init(&Timer1_config_ptr);

	/*rotate the DC_motor in CW for 15 seconds "open door" */
	DcMotor_Rotate(CW, 100);

	g_secondsCounter = 0;

	while(g_secondsCounter != OPEN_TIME_IN_SECONDS){
	}

	/*hold the DC_motor for 3 seconds */
	DcMotor_Rotate(STOP, 0);

	g_secondsCounter = 0;

	while(g_secondsCounter != HOLD_TIME_IN_SECONDS){
	}

	/*rotate the DC_motor in ANTI_CW for 15 seconds "Close door" */
	DcMotor_Rotate(ANTI_CW, 100);

	g_secondsCounter = 0;

	while(g_secondsCounter != CLOSE_TIME_IN_SECONDS){
	}

	/*Stop the DC_motor */
	DcMotor_Rotate(STOP, 0);

	g_secondsCounter = 0;

	/*clear timer*/
	Timer1_deInit();
}

/* Function Name : buzzerOperation
 * Description : Activate Buzzer for 1-minute.
 * then turn Buzzer OFF
 * Parameters : void.
 * Return : void.
 */
void buzzerOperation(void){

	/*start timer1*/
	Timer1_init(&Timer1_config_ptr);

	/*clear the counter to start counting 60 seconds*/
	g_secondsCounter = 0;

	Buzzer_on();

	while(g_secondsCounter != BUZZER_TIME_IN_SECONDS){

	}

	Buzzer_off();

	g_secondsCounter = 0;

	/*clear timer*/
	Timer1_deInit();
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

	UART_ConfigType UART_ConfigPtr = {BAUD_RATE_9600, SIZE_BIT_8, DISABLED, BIT_1};

	/*Call initialization function of UART driver*/
	UART_init(&UART_ConfigPtr);

	TWI_ConfigType TWI_ConfigPtr = {FirstAddress, bitRate_400Kbits};

	/*Call initialization function of TWI driver*/
	TWI_init(&TWI_ConfigPtr);

	/*Initialize the call back function of timer1 to count seconds*/
	Timer1_setCallBack(Timer1_INT);

	/*Call initialization function of Buzzer driver*/
	Buzzer_init();

	/*Call initialization function of DCMotor driver*/
	DcMotor_Init();

	while(1){
		/*Store the received command form HMI_ECU */
		g_receivedByte = UART_receiveByte();

		/*if user wants to create password or change password for the first time*/
		if(g_receivedByte == CHANGE_PASSWORD || g_receivedByte == PASSWORD1){
			savePassword();
		}

		/*if user wants to verify password for either
		 *changing password or wants to unlock the door*/
		else if(g_receivedByte == PASSWORD2){
			verifyPassword();
		}

		/*if received command is open door then start DC_Motor*/
		else if(g_receivedByte == OPEN_DOOR){
			motorOperation();
		}

		/*if received command is WRONG_TRIAL_3
		 *then this is a case of wronging 3 password trials
		 *so we turn on the Buzzer */
		else if(g_receivedByte == WRONG_TRIAL_3){
			buzzerOperation();
		}
	}

}

