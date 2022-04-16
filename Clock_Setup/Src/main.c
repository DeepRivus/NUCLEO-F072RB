/**
 ******************************************************************************
 * @file           : main.c
 * @author         : DeepRivus
 * @brief          : Main program body
 ******************************************************************************
 */
#include "SystemClock_Config.h"
#include <stdint.h>
#include "stm32f072xb.h"

int main()
{
	uint8_t i = 0;

	SystemClock_Config();

	while(1)
	{
		i++;
	}

	return 0;
}
