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
#include "cmsis_os.h"
#include "stdlib.h"
#include "stdbool.h"

#ifndef INC_INDICATOR_H_
#define INC_INDICATOR_H_

osMessageQueueId_t toneQueueId;
TIM_HandleTypeDef piezo;

typedef struct tone {
	uint32_t freq;
	uint32_t length;
} tone;

/* Task Function */
void indicator_task();

/* API Functions */
osStatus_t note(uint32_t freq, uint32_t length);
osStatus_t rest(uint32_t length);
bool scale(uint32_t freqStart, uint32_t freqEnd, uint32_t notes, uint32_t length, bool force);
void number();				/* Queues a series of notes corresponding to a number. */
void boot();				/* Queues the boot up note sequence. */
void standby();				/* Queues the standby note sequence. */
void warning();				/* Queues the warning note sequence and the warning code's number. */
void error();				/* Queues the error note sequence and the error code's number. */
void silence();				/* Clears the queue and stops any currently playing notes. */


/* Hardware Functions */
void _piezo_config();		/* Configures the PWM on TIM3 CH3. */
void _piezo_halt();

#endif /* INC_INDICATOR_H_ */
