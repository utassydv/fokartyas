/*
 * actuator.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "actuator.h"

extern uint8_t 	flagbeav;
extern uint32_t tav;

void beavatkozas(void)
{
	if (flagbeav == 1)
		{

			hiba 		= 	toerror(tav);
			beavatkozo	= 	szabPD(elozohiba, hiba);
			elozohiba	=	hiba;
			toPWM(beavatkozo);


			epres = toinkrspeed(v) - speed;
			upres = 136.8 + 0.04*szabvPI(epres);



			control();
			flagbeav = 0;
		}
}

void gyors(void)
{
	flaggyors = 1;
	flaglassu = 0;
	p= pgyors;
	d= dgyors;
	v= vgyors;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

void fekez(void)
{
	p= plassu;
	d= dlassu;
	v= vfek;
}

void lassu(void)
{
	flaglassu = 1;
	flaggyors = 0;
	p= plassu;
	d= dlassu;
	v= vlassu;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

void labyrinth(void)
{
	lassu();
}

void savvaltas()
{
	v = vsavvalt;
}

void control(void)
{
	if( engedelyezo(uwDutyCycle) == 0)
				{
					upres		=   0;
					u2prev = 0.0f;
					uprev = 0.0f;
				}

	if(flagsavvaltas == 0)
	{
	htim13.Instance->CCR1 	= pos; 		//elso szervo
	htim14.Instance->CCR1 	= pwmmidh; 	//hatso szervo

	htim10.Instance->CCR1 	= 1500; 	//szenzor szervo
	}
	if(flagsavvaltas == 1)
	{
	htim13.Instance->CCR1 	= posvalte; 		//elso szervo
	htim14.Instance->CCR1 	= posvalth; 	//hatso szervo
	htim10.Instance->CCR1 	= 1500; 	//szenzor szervo
	}

//	htim13.Instance->CCR1 	= 1500; 		//elso szervo
//	htim14.Instance->CCR1 	= 1500; 	//hatso szervo

	velocity(upres);						//motor
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}



void velocity(int16_t sebesseg)
{
	uint16_t csat1;
	uint16_t csat2;


	if(sebesseg > 524) 		sebesseg = 524;
	if(sebesseg < -523) 	sebesseg = -523;

	csat1 = 525 + sebesseg;
	csat2 = 525 - sebesseg;

	htim1.Instance->CCR1 	= csat1;
	htim1.Instance->CCR3 	= csat2;
}

