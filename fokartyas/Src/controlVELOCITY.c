/*
 * controlVELOCITY.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "controlVELOCITY.h"
#include "tracking.h"
#include "actuator.h"
#include "timing.h"
#include "stdlib.h"
#include "math.h"




#define KC		(4.0f)
#define ZD		(0.98f)
#define UMAX	(5000)
#define VKOVMAX	(1.5f)
#define VKOVMIN	(0.0f)

float v 		= 	0.0f;
float vlassu	=	1.5f;
float vgyors	=	2.5f;
float vfek		=	0.0f;
float vsavvalt 	=	1.0f;
float vkovet	=   0.0f;

float epres 	= 0.0f;
float upres 	= 0.0f;
float u2prev 	= 0.0f;
float uprev 	= 0.0f;
float u2 		= 0.0f;
float u 		= 0.0f;

uint32_t kivanttavolsag = 80;
float Kp = 0.0375f;
float Kd = 1.0f;
float Ki = 0.0f;
int32_t errtav = 0;
int32_t preverrtav = 0;
int32_t Integ = 0;
int32_t Der = 0;





extern TIM_HandleTypeDef htim1; 						//motor timer

void controlVELOCITYInit(void)
{
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);			//PWM Motor
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);			//PWM Motor
}

float toinkrspeed(float sebesseg)
{
	return sebesseg*700.0f;
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
	epres = toinkrspeed(v) - (float)GETspeed();

	if( fabs(GETv()) < 0.3  && fabs(epres) < 200 )		upres = 0;
	if (epres   > 0 )							upres = 	 136.8f + 0.04f*szabvPI(epres);
	if (epres   < 0 )							upres = 	-136.8f + 0.04f*szabvPI(epres);

}

void SETv(float ertek)
{
	v = ertek;
}

void velocitySETTER(void)
{
	if( GETuwDutyCycle() < 1600    /*|| GETflagSTART()  != 1*/)
	{
		SETv(0.0f);
	}
	else if (GETflaglassu() == 1)
	{
		SETv(vlassu);
	}
	else if (GETflaggyors() == 1)
	{
		SETv(vgyors);
	}
	else if (GETflagfekez() == 1)
	{
		SETv(vfek);
	}
	else if (GETflagsavvalt() == 1)
	{
		SETv(vsavvalt);
	}
	else if (GETflagSCkovet() == 1)
	{
		SETv(vkovet);
	}
}

void szabSCkovet(void)
{
	if(GETflagkovet() == 1)
	{

		errtav =GETSCtavolsag() - kivanttavolsag;
		Integ = Integ + errtav;
		Der = errtav - preverrtav;

		vkovet= 0.75 +  Kp*(float)errtav + Ki*(float)Integ + Kd* (float)Der;
		vkovet = (vkovet > VKOVMAX) ? 	VKOVMAX : vkovet;
		vkovet = (vkovet < VKOVMIN) ? 	VKOVMIN : vkovet;

		if(GETSCtavolsag() > 99) vkovet =1.5f;
		if(GETSCtavolsag() < 60) vkovet =0.0f;

		preverrtav = errtav;
		SETflagkovet(0);
	}
}

float GETv(void)
{
	return v;
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

float GETvkovet()
{
	return vkovet;
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

float GETepres()
{
	return epres;
}

