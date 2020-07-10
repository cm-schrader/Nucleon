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

/* Note Constants */
static const uint32_t NBREAK = 50;
static const uint32_t NSHORT = 250;
static const uint32_t NMED = 500;
static const uint32_t NLONG = 750;

static const uint32_t HIGH_NOTE = 700;
static const uint32_t MED_NOTE = 500;
static const uint32_t LOW_NOTE = 300;

/* Task Function */
void indicator_task();

/* API Functions */
osStatus_t note(uint32_t freq, uint32_t length);
osStatus_t rest(uint32_t length);
bool scale(uint32_t freqStart, uint32_t freqEnd, uint32_t notes, uint32_t length, bool force);
bool number_tone(uint32_t number);
bool boot_tone();
bool standby_tone();
//void warning_tone();				/* Queues the warning note sequence and the warning code's number. */
//void error_tone();				/* Queues the error note sequence and the error code's number. */
void silence();


/* Private Functions */
void _piezo_config();		/* Configures the PWM on TIM3 CH3. */
void _piezo_halt();

#endif /* INC_INDICATOR_H_ */
