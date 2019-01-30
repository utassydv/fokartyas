/*
 * controlVELOCITY.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "controlVELOCITY.h"
#include "tracking.h"

#define KC		(4.0f)
#define ZD		(0.98f)
#define UMAX	(5000)

float v 		= 	0.0f;
float vlassu	=	1.5f;
float vgyors	=	1.5f;
float vfek		=	0.0f;
float vsavvalt 	=	1.0f;

float epres 	= 0.0f;
float upres 	= 0.0f;
float u2prev 	= 0.0f;
float uprev 	= 0.0f;
float u2 		= 0.0f;
float u 		= 0.0f;

extern TIM_HandleTypeDef htim1; 						//motor timer

void controlVELOCITYInit(void)
{
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);			//PWM Motor
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);			//PWM Motor
}

float toinkrspeed(float sebesseg)
{
	return sebesseg*(float)700;
}


float szabvPI(float err)
{
	float u2, u1, u;
	u2 = ZD*u2prev + (1-ZD)*uprev;
	u1 = KC*err;

	u = u1 + u2;

	u = (u > UMAX) ? UMAX : u;
	u = (u < -UMAX) ? -UMAX : u;

	uprev = u;
	u2prev = u2;

	return u;
}

void tomotorcontrol(void)
{
	epres = toinkrspeed(v) - GETspeed();
	upres = 136.8 + 0.04*szabvPI(epres);
}

void SETv(float ertek)
{
	v = ertek;
}

float GETvlassu()
{
	return vlassu;
}

float GETvgyors()
{
	return vgyors;
}

float GETvsavvalt()
{
	return vsavvalt;
}

float GETvfek()
{
	return vfek;
}

float GETupres()
{
	return upres;
}

float GETu2prev()
{
	return u2prev;
}

void SETuprev(float ertek)
{
	uprev=ertek;
}

void SETupres(float ertek)
{
	upres=ertek;
}

void SETu2prev(float ertek)
{
	u2prev=ertek;
}


