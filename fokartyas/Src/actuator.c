/*
 * actuator.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "actuator.h"
#include "controlSTEERING.h"
#include "controlVELOCITY.h"
#include "timing.h"


extern TIM_HandleTypeDef htim13; //elsoszervo
extern TIM_HandleTypeDef htim14; //hatsoszervo
extern TIM_HandleTypeDef htim10; //szenzorszervo
extern TIM_HandleTypeDef htim1;  //motor
extern TIM_HandleTypeDef htim3;  //taviranyito


__IO uint32_t uwIC2Value = 0;
__IO uint32_t  uwDutyCycle = 0;

uint8_t flagsavvaltas = 0;
const int16_t posvalte = 1700;
const int16_t posvalth = 1850;

void actuatorInit(void)
{
	 HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);		//Taviranyito CH1
	 HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);		//Taviranyito CH2
}


void regulator(void)
{
	if (GETflagregulator() == 1)
	{
		toservo();
		tomotorcontrol();
		SETflagregulator(0);
	}
}

void control(void)
{
	if (GETflagbeav() == 1)
	{
		if( uwDutyCycle < 160)
		{
			SETupres(0);
			SETu2prev(0.0f);
			SETuprev(0.0f);
		}

		szervovezerles(GETpos(), GETposh());
		motorvezerles(GETupres());						//motor
		SETflagbeav(0);
	}
}

void szervovezerles(int16_t elsoszervo, int16_t hatsoszervo)
{

		if (flagsavvaltas == 1)
		{
			htim13.Instance->CCR1 	= posvalte; 		//elso szervo
			htim14.Instance->CCR1 	= posvalth; 	//hatso szervo
			htim10.Instance->CCR1 	= 1500; 			//szenzor szervo
		}
		else
		{
			htim13.Instance->CCR1 	= elsoszervo; 		//elso szervo
			htim14.Instance->CCR1 	= GETpwmmidh(); 	//hatso szervo
			htim10.Instance->CCR1 	= 1500; 			//szenzor szervo
		}




		//KOZEPRE ALLITO
		//	htim13.Instance->CCR1 	= 1500; 		//elso szervo
		//	htim14.Instance->CCR1 	= 1500; 	//hatso szervo
}

void motorvezerles(int16_t beavatkozojel)
{
	uint16_t csat1;
	uint16_t csat2;


	if(beavatkozojel > 524) 	beavatkozojel = 524;
	if(beavatkozojel < -523)	beavatkozojel = -523;

	csat1 = 525 + beavatkozojel;
	csat2 = 525 - beavatkozojel;

	htim1.Instance->CCR1 	= csat1;
	htim1.Instance->CCR3 	= csat2;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void gyors(void)
{
	SETflaggyors(1);
	SETflaglassu(0);

	SETp(GETpgyors());
	SETd(GETdgyors());

	SETv(GETvgyors());

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

void fekez(void)
{
	SETp(GETplassu());
	SETd(GETdlassu());

	SETv(GETvfek());
}

void lassu(void)
{
	SETflaggyors(0);
	SETflaglassu(1);

	SETp(GETplassu());
	SETd(GETdlassu());

	SETv(GETvlassu());

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

void labyrinth(void)
{
	lassu();
}

void savvaltas()
{
	SETv(GETvsavvalt());
}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)  //TAVIRANYITO ENGEDELYEZO JEL
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    /* Get the Input Capture value */
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

    if (uwIC2Value != 0)
    {
      uwDutyCycle = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    }
    else
    {
      uwDutyCycle = 150;
    }
  }

}

uint32_t GETuwDutyCycle(void)
{
	return uwDutyCycle;
}

uint8_t GETflagsavvaltas(void)
{
	return flagsavvaltas;
}

void SETflagsavvaltas(uint8_t ertek)
{
	flagsavvaltas = ertek;
}
