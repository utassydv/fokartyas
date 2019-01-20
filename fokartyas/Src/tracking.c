/*
 * tracking.c
 *
 *  Created on: 20 Jan 2019
 *      Author: utassyd
 */
#include "tracking.h"
#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi2;

extern int32_t counterpres;
extern int32_t counterprev;
extern int32_t speed;

extern uint8_t txdata1[2];
extern uint8_t txdata2[2];
extern uint8_t rxdata1[2];
extern uint8_t rxdata2[2];




void speedpos(void)				//sebesseg es pozicio merese
{
		counterprev = counterpres;
		counterpres = TIM2->CNT;
		speed= counterpres - counterprev;
}

void angle(void)				//z elfordulas kiolvasas//////////////////
{
		 txdata1[0]		=	0b10100110; //h26
		 txdata1[1]		=	0b00000000;
		 rxdata1[0] 	=	0b00000000;
		 rxdata1[1] 	=	0b00000000;

		 txdata2[0]		=	0b10100111; //h27
		 txdata2[1]		=	0b00000000;
		 rxdata2[0] 	=	0b00000000;
		 rxdata2[1] 	=	0b00000000;

		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //CS
	  	 HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 2, HAL_MAX_DELAY);
	  	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //CD

	  	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //CS
	  	 HAL_SPI_TransmitReceive(&hspi2, txdata2, rxdata2, 2, HAL_MAX_DELAY);
	  	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //CD
}

