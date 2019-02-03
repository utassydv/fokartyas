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
extern TIM_HandleTypeDef htim12;  //elsotavolsagerzekelo


__IO uint32_t uwIC2Value = 0;
__IO uint32_t  uwDutyCycle = 0;
__IO uint32_t vlmi = 0;
__IO uint32_t  SCtavolsag = 0;

uint8_t flagsavvalt	= 0;
uint8_t flaggyors	= 0;
uint8_t flaglassu	= 0;
uint8_t flagfekez	= 0;
uint8_t flagkovet	= 0;
uint8_t flagSCkovet	= 0;


uint8_t flagSTART 		= 0;
const int16_t posvalte = 1700;
const int16_t posvalth = 1850;


void actuatorInit(void)
{
	 HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);		//Taviranyito CH1
	 HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);		//Taviranyito CH2
	 HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);		//Taviranyito CH1
	 HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_2);		//Taviranyito CH2
}


void regulator(void)
{
	if (GETflagregulator() == 1)
	{
		toservo();


		velocitySETTER();


		tomotorcontrol();
		SETflagregulator(0);
	}
}

void control(void)
{
	if (GETflagbeav() == 1)
	{


		szervovezerles(GETpos(), GETposh());
		motorvezerles(GETupres());						//motor
		SETflagbeav(0);
	}
}

void szervovezerles(int16_t elsoszervo, int16_t hatsoszervo)
{

		if (flagsavvalt == 1)
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


	if(beavatkozojel >  520) 	beavatkozojel =  520;
	if(beavatkozojel < -520)	beavatkozojel = -520;

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

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
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

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
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

	if (htim->Instance == TIM3)
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
				uwDutyCycle = 1500;
			}
		}
	}

	if (htim->Instance == TIM12)
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			/* Get the Input Capture value */
			vlmi = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

			if (vlmi != 0)
			{
				SCtavolsag = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2)/58;
			}
			else
			{
				SCtavolsag = 100;
			}
		}
	}


}

uint32_t GETuwDutyCycle(void)
{
	return uwDutyCycle;
}


void SETflagSTART(uint8_t ertek)
{
	flagSTART = ertek;
}

uint32_t GETSCtavolsag(void)
{
	return SCtavolsag;
}

uint8_t GETflaglassu(void)
{
	return flaglassu;
}

void SETflaglassu(uint8_t ertek)
{
	flaglassu = ertek;
}

uint8_t GETflaggyors(void)
{
	return flaggyors;
}

void SETflaggyors(uint8_t ertek)
{
	flaggyors = ertek;
}

uint8_t GETflagfekez(void)
{
	return flagfekez;
}

void SETflagfekez(uint8_t ertek)
{
	flagfekez = ertek;
}

uint8_t GETflagsavvalt(void)
{
	return flagsavvalt;
}

void SETflagsavvalt(uint8_t ertek)
{
	flagsavvalt = ertek;
}

uint8_t GETflagSCkovet(void)
{
	return flagSCkovet;
}

void SETflagSCkovet(uint8_t ertek)
{
	flagSCkovet = ertek;
}
