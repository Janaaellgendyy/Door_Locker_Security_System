 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: source file for the UART driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/
#include "uart.h"
#include "common_macros.h"
#include <avr/io.h>


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType * config_ptr){

	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 is set using ConfigPtr determined according to chosen settings
	 ***********************************************************************/
	UCSRB = (1<<TXEN) | (1<<RXEN);
	UCSRB |= (config_ptr->bit_data&0x04);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UPM1:0  is set using ConfigPtr determined according to chosen settings
	 * USBS is set using ConfigPtr determined according to chosen settings
	 * UCSZ1:0 is set using ConfigPtr determined according to chosen settings
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1) |(config_ptr->stop_bit<<3) | ((config_ptr->parity&0x03)<<4) | ((config_ptr->bit_data&0x03)<<1);

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)((F_CPU/(8UL*config_ptr->baud_rate))-1);

	UBRRH = ubrr_value >> 8;
	UBRRL = ubrr_value;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(uint8 data){
	//wait until the last data is shifted out and the transmit buffer is ready to receive new data.
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	/*
	 * put the required data in the transmit buffer (UDR).
	 * it clears UDRE bit.
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void){
	// Wait until the transmission is complete TXC = 1
	while(BIT_IS_CLEAR(UCSRA,RXC));

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str){
	uint8 i =0;
	while(Str[i]!='\0'){
		// send byte by byte
		UART_sendByte(Str[i++]);
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str){
	uint8 i = 0;
	// receive first byte
	Str[i] = UART_receiveByte();
	// check if the received byte is not #, if true store the byte in the string
	while(Str[i]!='#'){
		i++;
		Str[i] = UART_receiveByte();
	}
	// after receiving the string add null instead of # symbol
	Str[i] = '\0';
}
