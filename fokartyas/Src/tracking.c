/*
 * tracking.c
 *
 *  Created on: 20 Jan 2019
 *      Author: utassyd
 */
#include "tracking.h"
#include "stm32f4xx_hal.h"
#include "math.h"

extern SPI_HandleTypeDef hspi2;

extern int32_t counterpres;
extern int32_t counterprev;
extern int32_t speed;

extern uint8_t txdata1[3];
extern uint8_t rxdata1[3];

uint8_t enabledata[2];
uint8_t cplflag	=	0;
float szogseb;

extern uint8_t flagangle;
extern uint8_t flagangleoffset;
uint16_t offsetcnt = 0;

extern int32_t currentX;
extern int32_t currentY;


float offsetszog;


void speedpos(void)				//sebesseg es pozicio merese
{
		counterprev = counterpres;
		counterpres = TIM2->CNT;
		speed= counterpres - counterprev;
}

void enablegyro(void)
{
	enabledata[0]	= 0b00010001; //00010000 0x10
	enabledata[1]	= 0b01000000;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
	HAL_SPI_Transmit(&hspi2, enabledata, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 			//CD
}

void gyrooffset(void)
{
	if (flagangleoffset == 1)
	{
		int16_t adat;

		txdata1[0]		=	0b10100110; //h26 0x11
		txdata1[1]		=	0b00000000;
		txdata1[2]		=	0b00000000;


		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
		HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 3, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


		adat 	= rxdata1[2] << 8;
		adat 	|=  rxdata1[1];
		szogseb	= (float)adat*8.75f;

		offsetszog = 0.001f*szogseb+0.999f*offsetszog;

		offsetcnt++;
		flagangleoffset = 0;
	}
}

void angle(void)				//z elfordulas kiolvasas//////////////////
{
	if (flagangle == 1) {
		int16_t adat;

		txdata1[0]		=	0b10100110; //h26 0x11
		txdata1[1]		=	0b00000000;
		txdata1[2]		=	0b00000000;



		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
		HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 3, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


		adat 	= rxdata1[2] << 8;
		adat 	|=  rxdata1[1];
		szogseb	= (float)adat*8.75-offsetszog;
		flagangle = 0;

	}
}

void gyro(void)
{
	if (offsetcnt < 1000)
	{
		gyrooffset();
	}
	else angle();
	poz();
}

void poz(void)
{
	currentX = currentX + deltax(speed,szogseb);
	currentY = currentY + deltay(speed,szogseb);
}

float deltax(int vpalya, float vszog)
{
	float valto=M_PI/180;
	float uthossz=(((float)vpalya)*200)/140;
	float szog=((vszog/1000)*200)*valto;
	float szin=(float)sin((double)szog);
	return uthossz*szin;
}

float deltay(int vpalya, float vszog)
{
	float valto=M_PI/180;
	float uthossz=(((float)vpalya)*200)/140;
	float szog=((vszog/1000)*200)*valto;
	float kosz=(float)cos((double)szog);
	return uthossz*kosz;
}

