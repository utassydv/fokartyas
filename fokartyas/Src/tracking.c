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

extern uint8_t txdata1[3];
extern uint8_t rxdata1[3];

uint8_t enabledata[2];
uint8_t cplflag	=	0;
float szogseb;





void speedpos(void)				//sebesseg es pozicio merese
{
		counterprev = counterpres;
		counterpres = TIM2->CNT;
		speed= counterpres - counterprev;
}

void enablegyro(void)
{
	enabledata[1]	= 0b00010001;
	enabledata[1]	= 0b01000000;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
	HAL_SPI_Transmit(&hspi2, enabledata, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 			//CD
}

void angle(void)				//z elfordulas kiolvasas//////////////////
{
	uint16_t adat;

	txdata1[0]		=	0b10100110; //h26
	txdata1[1]		=	0b00000000;
	txdata1[2]		=	0b00000000;



	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
	HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


	adat 	= rxdata1[2] << 8;
	adat 	|=  rxdata1[1];
	szogseb	= (float)adat*8.75;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  cplflag=1;
}
