/**
  ******************************************************************************
  * @file           : indicator.c
  * @brief          : Buzzer and Status LED management
  * @date			: July 6th, 2020
  * @author			: Chrisitan M. Schrader
  ******************************************************************************
  */

#include "Human_Interface/indicator.h"

void indicator_task(TIM_HandleTypeDef timerHandle, uint32_t timerChannel)
{
	struct tone *t;
	if (osMessageQueueGet(toneQueueId, t, NULL, osWaitForever) == osOK)
	{
		_piezo_config(t->freq);
		osDelay(t->length);
//		_piezo_config(f);
//		osDelay(250);
	}
	else
	{
		_piezo_halt();
		osDelay(500);
	}
}

void note(uint32_t freq, uint32_t length)
{
	struct tone t;
	t.freq = freq;
	t.length = length;
//	osMessageQueuePut(toneQueueId, &t, 1, 50);
//	osMessageQueuePut(toneQueueId, &freq, 0, 100);
}

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

void _piezo_halt()
{
	HAL_TIM_PWM_Stop(&piezo, TIM_CHANNEL_3);
}
