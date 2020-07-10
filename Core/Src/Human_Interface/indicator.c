/**
  ******************************************************************************
  * @file           : indicator.c
  * @brief          : Buzzer and Status LED management
  * @date			: July 6th, 2020
  ******************************************************************************
  */

#include "Human_Interface/indicator.h"

/**
 * @brief Main indicator task within indicator thread.
 */
void indicator_task()
{
	tone t;
	if (osMessageQueueGet(toneQueueId, &t, NULL, 100) == osOK)
	{
		_piezo_config(t.freq);
		osDelay(t.length);
	}
	else
	{
		_piezo_halt();
	}
}

/*
 * ================================================================================
 * 									API Functions
 * ================================================================================
 */

/**
 * @brief Queues a note.
 * @param freq		:Frequency of note to play in hz.
 * @param length	:Length of note in ms.
 * @retval			:Status of queue insertion.
 */
osStatus_t note(uint32_t freq, uint32_t length)
{
	tone t;
	t.freq = freq;
	t.length = length;
	return osMessageQueuePut(toneQueueId, &t, 1, 50);
}

/**
 * @brief Queues a rest.
 * @param length	:Length of rest in ms.
 * @retval			:Status of queue insertion.
 */
osStatus_t rest(uint32_t length)
{
	tone t;
	t.freq = 0;
	t.length = length;
	return osMessageQueuePut(toneQueueId, &t, 1, 50);
}

/**
 * @brief Queues a scale of notes.
 * @param freqStart		:Starting frequency.
 * @param freqEnd		:Ending frequency.
 * @param notes			:Number of notes in the scale.
 * @param length		:Length of scale in ms.
 * @param bool			:If true, the function will guarantee every note is
 * 						 queued but it may block execution.
 * @retval				:The success of the operation.
 *
 * @note	In order to queue the notes, there needs to be enough space in the
 * 			indicator queue.  If force is true, the scale can be larger than
 * 			the amount of empty space in the queue, but it will block thread
 * 			execution.
 */
bool scale(uint32_t freqStart, uint32_t freqEnd, uint32_t notes, uint32_t length, bool force)
{
	/* If there isn't enough space and force is false, return a failure. */
	if (osMessageQueueGetCapacity(toneQueueId) - osMessageQueueGetCount(toneQueueId) <= notes && !force)
	{
		return false;
	}

	int32_t nstep = abs(freqEnd - freqStart) / notes;
	uint32_t tstep = length / notes;

	/* Queue scale */
	int high = (freqStart > freqEnd) ? freqStart : freqEnd;
	int low = (freqStart < freqEnd) ? freqStart : freqEnd;
	for(int f = low; f < high; f += nstep)
	{
		if (note((freqStart < freqEnd) ? f :high - f + low, tstep) != osOK)
		{
			/* If a note fails to queue and force is false, return a failure. */
			if (!force) return false;
		}
	}
	return true;
}

/**
 * @brief Plays a series of notes representing a number.
 * @param number	:Number to indicate.
 * @retval			:The success of the operation.
 *
 * @note	In order to queue the notes, there needs to be enough space in the
 * 			indicator queue.
 *
 * @note 	A high beep denotes the start of a number.  A medium note indicates
 * 			the start of a digit.  Each subsequent low note is added together
 * 			to give you the number.  If no low notes follow a medium note, that
 * 			indicates a zero.
 *
 * 			Example: H M L L M L L L M M -> 2300
 */
bool number_tone(uint32_t number)
{
	uint32_t digit;
	uint32_t size = 0;
	uint32_t j;
	uint32_t i = 0;
	uint32_t notes = 1;

	/* Reverse digits */
	while (number != 0)
	{
		i *= 10;
		i += number%10;
		number /= 10;
		++size;
	}
	number = i;

	/* If there isn't enough space and force is false, return a failure. */
	for(j = 0; j < size; ++j)
	{
		notes += (i % 10) * 2 + 2;
		i /= 10;
	}
	if (osMessageQueueGetCapacity(toneQueueId) - osMessageQueueGetCount(toneQueueId) <= notes)
	{
		return false;
	}

	/* Queue Notes */
	if (note(HIGH_NOTE, NLONG) != osOK) return false;
	for(j = 0; j < size; ++j)
	{
		/* Next Digit */
		digit = number % 10;
		number /= 10;
		note(MED_NOTE, NMED);
		for (i = 0; i < digit; ++i)
		{
			if (note(LOW_NOTE, NSHORT) != osOK) return false;
			if (rest(NSHORT) != osOK) return false;
		}
		if (digit == 0)
		{
			if (rest(NMED) != osOK) return false;
		}
		else
		{
			if (rest(NSHORT) != osOK) return false;
		}
	}
	return true;
}

/**
 * @brief Plays a series of notes indicating a successful bootup.
 * @retval			:The success of the operation.
 */
bool boot_tone()
{
	if (note(LOW_NOTE, 400) != osOK) return false;
	if (rest(NBREAK) != osOK) return false;
	if (note(MED_NOTE, 100) != osOK) return false;
	if (rest(NBREAK) != osOK) return false;
	if (note(HIGH_NOTE, 300) != osOK) return false;
	if (rest(NMED) != osOK) return false;
	return true;
}

/**
 * @brief Plays a series of notes indicating standby.
 * @retval			:The success of the operation.
 */
bool standby_tone()
{
	if (note(LOW_NOTE, 200) != osOK) return false;
	if (rest(NBREAK) != osOK) return false;
	if (note(HIGH_NOTE, 100) != osOK) return false;
	if (rest(NMED) != osOK) return false;
	return true;
}

/**
 * @brief Silences the peizo.
 *
 * 	Dumps the tone queue and stops PWM.
 */
void silence()
{
	osMessageQueueReset	(toneQueueId);
	_piezo_halt();
}

/*
 * ================================================================================
 * 									Private Functions
 * ================================================================================
 */

/**
 * @brief Configures the piezo PWM signal.
 * @param freq	:Frequency of desired PWM signal.
 */
void _piezo_config(uint32_t freq)
{
	uint32_t t = (uint32_t) 1000000 / freq;
	HAL_TIM_PWM_Stop(&piezo, TIM_CHANNEL_3);
	TIM_OC_InitTypeDef sConfigOC;
	piezo.Init.Period = t;//1000/(freq);
	piezo.Init.Period = t;
	HAL_TIM_PWM_Init(&piezo);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = piezo.Init.Period / 2;
	HAL_TIM_PWM_ConfigChannel(&piezo, &sConfigOC, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&piezo, TIM_CHANNEL_3);
}

/**
 * @brief Stops the piezo PWM signal.
 */
void _piezo_halt()
{
	HAL_TIM_PWM_Stop(&piezo, TIM_CHANNEL_3);
}
