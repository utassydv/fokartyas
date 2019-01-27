/*
 * controlVELOCITY.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "controlVELOCITY.h"

#define KC		(4.0f)
#define ZD		(0.98f)
#define UMAX	(5000)

float upres = 0.0f;
float u2prev = 0.0f;
float uprev = 0.0f;
float u2 = 0.0f;
float u = 0.0f;

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
