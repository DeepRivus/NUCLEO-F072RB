/*
 * uartPrint.c
 *
 *  Created on: Feb 16, 2022
 *      Author: Deep Rivus
 */

#include "stm32f072xb.h"

void console_Init(void){


	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* GPIOA clock enabled */

	/* PA2 and PA3 are configured in Alternate function mode */
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);  /* Clear bits */
	GPIOA->MODER |= (0x02 <<GPIO_MODER_MODER2_Pos) | (0x02 <<GPIO_MODER_MODER3_Pos);

	/* PA2 and PA3 --> AF1 (USART2)*/
	GPIOA->AFR[0] &= ~(0x0000FF00); /* Clear bits */
	GPIOA->AFR[0] |=  (0x00001100);

	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;	/* USART2 clock enabled*/

	/* Clear USART2 configuration (reset state) 8-bit, 1 start, 1 stop, CTS/RTS disabled */
	USART2->CR1 = 0x00000000;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;

	/* Baud Rate = 115200
	 With OVER8=0 and Fck=12MHz, USARTDIV =   12E6/115200 = 104.1666
	 BRR = 104 -> Baud Rate = 115107.9137 -> 0.08% error

	 With OVER8=1 and Fck=12MHz, USARTDIV = 2*12E6/115200 = 208.3333
	 BRR = 208 -> Baud Rate = 115246.0984 -> 0.04% error (better)*/

	USART2->CR1 |= USART_CR1_OVER8;
	USART2->BRR = 208;

	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;	/* Enable both Transmitter and Receiver */

	USART2->CR1 |= USART_CR1_UE;	/* USART2 enabled  */

}
