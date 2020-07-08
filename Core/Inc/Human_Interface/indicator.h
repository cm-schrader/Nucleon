/**
  ******************************************************************************
  * @file           : indicator.c
  * @brief          : Buzzer and Status LED management
  * @date			: July 6th, 2020
  *
  * 	Interfaces with the Status LED and Piezo buzzer.
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"

#ifndef INC_INDICATOR_H_
#define INC_INDICATOR_H_

/* Task Function */
void indicator_task(TIM_HandleTypeDef timerHandle, uint32_t timerChannel);		/* Main indicator task */

/* API Functions */
void note();				/* Queues a note. */
void scale();				/* Queues a scale of notes. */
void rest();				/* Queues a period of silence. */
void number();				/* Queues a series of notes corresponding to a number. */
void boot();				/* Queues the boot up note sequence. */
void standby();				/* Queues the standby note sequence. */
void warning();				/* Queues the warning note sequence and the warning code's number. */
void error();				/* Queues the error note sequence and the error code's number. */
void silence();				/* Clears the queue and stops any currently playing notes. */


/* Hardware Functions */
void _piezo_config();		/* Configures the PWM on TIM3 CH3. */

#endif /* INC_INDICATOR_H_ */
