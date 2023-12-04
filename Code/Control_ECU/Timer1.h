/******************************************************************************
 *
 * Module: Timer1
 *
 * File Name:Timer1.h
 *
 * Description: Header file for the Timer1 driver
 *
 * Author: Jana Wael
 *
 *******************************************************************************/


#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*using pre-scaler 256*/
#define OVERFLOWS_PER_MINUE_NORMAL_MODE 29

/*for the simulation to be accurate the number of ticks may be around 25000 ticks*/
#define TICKS_PER_ONE_SECOND 31249


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/*enum for Clock to select the first three bits' values for the pre-scaler in TCCR1B register*/
typedef enum{

	no_clock, prescaler_1, prescaler_8,
	prescaler_64, prescaler_256, prescaler_1024

}Timer1_Prescaler;

/* enum to set timer mode through TCCR1A register where
 *  WGM10, WGM11, WGM12 are set to 0 for normal mode (Normal_Mode = 0)
 *  WGM10, WGM11 are set to 0 and WGM12 is set to 1 in CTC_Mode (CTC_Mode = 4) */
typedef enum{

	NormalMode, CTC_Mode = 4

}Timer1_Mode;


typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

 void Timer1_init(const Timer1_ConfigType * Config_Ptr);
 void Timer1_deInit(void);
 void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
