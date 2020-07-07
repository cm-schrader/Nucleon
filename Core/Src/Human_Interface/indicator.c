/**
  ******************************************************************************
  * @file           : indicator.c
  * @brief          : Buzzer and Status LED management
  * @date			: July 6th, 2020
  ******************************************************************************
  */

#include "Human_Interface/indicator.h"

void indicator_task(TIM_HandleTypeDef timerHandle, uint32_t timerChannel)
{
	_piezo_config(440);
	osDelay(250);
	_piezo_config(880);
	osDelay(500);
	_piezo_halt();
	osDelay(1500);
}

/**
 * @brief Configures the piezo buzzer.
 * @param freq: Frequency in hz.
 */
void _piezo_config(uint32_t freq)
{
	uint32_t t = (uint32_t) 1000000 / freq;
	HAL_TIM_PWM_Stop(&piezo, TIM_CHANNEL_3);
	TIM_OC_InitTypeDef sConfigOC;
	piezo.Init.Period = t;//1000/(freq);
	HAL_TIM_PWM_Init(&piezo);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = piezo.Init.Period / 2;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&piezo, &sConfigOC, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&piezo, TIM_CHANNEL_3);
}

/**
 * @brief Turns off the piezo buzzer.
 */
void _piezo_halt()
{
	HAL_TIM_PWM_Stop(&piezo, TIM_CHANNEL_3);
}
