/*
 * systemclk_Config.c
 *
 *  Created on: Feb 16, 2022
 *      Author: Deep Rivus
 */


#include "stm32f072xb.h"

/*
 * 	Clock configuration for the Nucleo STM32F072RB board
 * 	HSE input Bypass Mode 			-> 8MHz
 * 	SYSCLK, AHB  					-> 48MHz
 * 	APB1/4							-> 12Mhz
 *  PA8 as MCO with /16 prescaler 	-> 3MHz
 *
 */

void SystemClock_Config()
{
	uint32_t	HSE_Status;
	uint32_t	PLL_Status;
	uint32_t	SW_Status;
	uint32_t	timeout = 0;

	timeout = 1000000;

	/* The high speed external clock (HSE) is started in Bypass Mode */
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;

	/* Wait until the high speed external clock (HSE) is ready */
	do
	{
		HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
		timeout--;
	} while ((HSE_Status == 0) && (timeout > 0));

	/* Select HSE as phase-locked loop (PLL) input source */
	RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;
	RCC->CFGR |= (2u << RCC_CFGR_PLLSRC_Pos);

	/* Set phase-locked loop (PLL) PREDIV to /1 */
	RCC->CFGR2 = 0x00;

	/* Set PLL multiplication factor to x 6 */
	RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
	RCC->CFGR |= (0x04 << RCC_CFGR_PLLMUL_Pos);

	/* Enable the PLLON bit */
	RCC-> CR |= RCC_CR_PLLON;

	/* Wait until the phase-locked loop (PLL) is ready */
	do
	{
		PLL_Status = RCC->CR & RCC_CR_PLLRDY_Msk;
		timeout--;
	} while ((PLL_Status == 0) && (timeout > 0));

	/*Set Advanced High-performance Bus (AHB) prescaler to /1*/
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	/*Set the Advanced Peripheral Bus 1 (APB1)  prescaler to /4 */
	RCC->CFGR &= ~RCC_CFGR_PPRE_DIV4_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV4;// PCLK (APB1) -> 12 MHz

	/* Enable FLASH Prefetch Buffer and set Flash Latency */
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	/* --- Until this point, MCU is clocked by HSI at 8MHz ---*/
	/* --- Switching to PLL at 48MHz ---*/

	/* Select the phase-locked loop (PLL) as system clock source */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait until PLL becomes main switch input
	do
	{
		SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
		timeout--;
	} while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));

	/*--- Use PA8 as MCO output at 48/16 = 3MHz ---*/

	/* Set Microcontroller clock output (MCO) source as SYSCLK (48MHz) */
	RCC->CFGR &= ~RCC_CFGR_MCO_Msk;
	RCC->CFGR |=  RCC_CFGR_MCOSEL_SYSCLK;

	/* Set Microcontroller clock output (MCO) prescaler to /16 -> 3MHz */
	RCC->CFGR &= ~RCC_CFGR_MCOPRE_Msk;
	RCC->CFGR |=  RCC_CFGR_MCOPRE_DIV16;

	/* Enable GPIOA clock */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	/* Configure PA8 as Alternate function */
	GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
	GPIOA->MODER |= (0x02 <<GPIO_MODER_MODER8_Pos);

	/* Set to AF0 (MCO output) */
	GPIOA->AFR[1] &= ~(0x0000000F);
	GPIOA->AFR[1] |=  (0x00000000);


	/* Update SystemCoreClock global variable */
	SystemCoreClockUpdate();
}
