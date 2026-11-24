/*
 * app.c
 *
 *  Created on: 24 Nov 2026
 *      Author: ksvaz
 */

#include "app.h"
#include "main.h"
#include "taimeris.h"

#define APP_BLINK_PERIOD 500 // ms

Timer_t APP_BLINKY_TIMER;

void App_Init()
{
	// PC13 gaismas diode ir active-low: GPIO_PIN_RESET = ieslēgta
	HAL_GPIO_WritePin(LEDBUILTIN_GPIO_Port, LEDBUILTIN_Pin, GPIO_PIN_SET);

	// Taimeru inicializācija
	uint32_t currentTime = HAL_GetTick();
	InitTimer(&APP_BLINKY_TIMER, APP_BLINK_PERIOD, 0, 0, 0);
	EnableTimer(&APP_BLINKY_TIMER, currentTime);
}

void App_Loop()
{
	uint32_t currentTime = HAL_GetTick();
	if (CheckTimer(&APP_BLINKY_TIMER, currentTime))
	{
		HAL_GPIO_TogglePin(LEDBUILTIN_GPIO_Port, LEDBUILTIN_Pin);
	}
}

// Kļūdas stāvoklis: PC13 ātri mirgo bezgalīgi (~5 Hz).
// Strādā arī tad, ja takts iestatīšana neizdevās un MX_GPIO_Init() vēl nav izsaukts,
// jo raksta tieši reģistros un gaida ar tukšu ciklu (SysTick nav vajadzīgs).
void App_Error()
{
	__disable_irq();
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // GPIOC takts
	GPIOC->CRH = (GPIOC->CRH & ~(0xFU << 20)) | (0x2U << 20); // PC13 push-pull izeja, 2 MHz
	while (1)
	{
		GPIOC->ODR ^= GPIO_ODR_ODR13;
		for (volatile uint32_t i = 0; i < 100000; i++);
	}
}
