/******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: Timer1.c
 *
 * Description: Source file for the Timer1 driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include "Timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

static volatile void(*g_callBackPtr)(void)= NULL_PTR;


/*
 * Timer1 used to count seconds in CTC mode.
 * ORCR1A = TICKS_PER_ONE_SECOND
 */

ISR(TIMER1_COMPA_vect){

	if (g_callBackPtr!=NULL_PTR)
		g_callBackPtr();
}
 void Timer1_setCallBack(void(*a_ptr)(void)){

	 g_callBackPtr = a_ptr;

 }

 void Timer1_init(const Timer1_ConfigType * Config_Ptr){

	 /*
	 * set the pre-scaler value to the chosen one.
	 * set WGM12 value according to the mode.
	 */
	 TCCR1B = (Config_Ptr -> prescaler) | ((Config_Ptr -> mode << 1)& 0x08);

	 /*set WGM10 and WGM11 values.*/
	 TCCR1A = (1<<FOC1A) | (1<<FOC1B) |(1<<COM1A1);

	TCNT1 = Config_Ptr -> initial_value;

	/*Check for the selected mode*/
	if(Config_Ptr -> mode == NormalMode){

		TIMSK |= (1 << TOIE1);

	/*compare value is added only in compare mode*/
	}else if(Config_Ptr -> mode == CTC_Mode){

		OCR1A = Config_Ptr ->compare_value;

		SET_BIT(TIMSK, OCIE1A);

	}




}

ISR(TIMER1_OVF_vect) {
	if (g_callBackPtr != NULL_PTR)
		g_callBackPtr();

}
 /*Clear all registers of timer */
 void Timer1_deInit(void){

	 TCNT1 = 0;

	 OCR1A = 0;

	 TCCR1B = 0;

	 TCCR1B = 0;


 }


