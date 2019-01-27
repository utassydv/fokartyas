/*
 * controlSTEERING.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "controlSTEERING.h"


float p			=	2.5f;
float d			=	10.0f;

const uint16_t pwmmide	=	1500;
const uint16_t pwmmidh 	=	1500;
const uint16_t rangee 	=	345;
const uint16_t rangeh	=	400;

int16_t pos;
int16_t posh;
const int16_t posvalte = 1700;
const int16_t posvalth = 1850;
uint8_t flaggyors;
uint8_t flaglassu;

float plassu	=	2.7f;
float dlassu	=	10.0f;
float vlassu	=	1.5f;
float scalelassu=	0.7f;

float pgyors	=	0.8f;
float dgyors	=	5.0f;
float vgyors	=	2.5f;
float vfek		=	0.0f;
float scalegyors=	1.0f;

float vsavvalt 	=	1.0f;

int32_t toerror(uint32_t dist)
{
	int32_t tavolsag=dist;
	if (tavolsag > 25599 || tavolsag < 0) tavolsag=12799;	//ha nemletezeo tavolsag, akkor kozep tavolsag.
	tavolsag=tavolsag-12799;

	return tavolsag;
}


int32_t szabPD(int32_t elozohibajel, int32_t hibajel)
{
	int32_t beavatkozo=0;

	beavatkozo= (p*(float)hibajel) + d*(((float)hibajel- (float)elozohibajel));


	return beavatkozo;
}


int16_t toPWM(int32_t jel)
{
//ELSO
	int16_t pwme=pwmmide+jel*((rangee*100000)/12799)/100000; //KORM�?NY
	if(pwme<pwmmide-rangee) pwme=pwmmide-rangee;
	if(pwme>pwmmide+rangee) pwme=pwmmide+rangee;

	pos		=	pwme;
//HATSO
	int16_t pwmh;
	if (flaglassu == 1)
	{
		pwmh=pwmmidh+scalelassu*jel*(rangeh*100000/12799)/100000; //KORM�?NY
		if(pwmh<pwmmidh-rangeh) pwmh=pwmmidh-rangeh;
		if(pwmh>pwmmidh+rangeh) pwmh=pwmmidh+rangeh;
	}
	if (flaggyors == 1)
	{
		pwmh=pwmmidh-scalegyors*jel*(rangeh*1000000/12799)/100000; //KORM�?NY
		if(pwmh<pwmmidh-rangeh) pwmh=pwmmidh-rangeh;
		if(pwmh>pwmmidh+rangeh) pwmh=pwmmidh+rangeh;
	}
	posh	=	pwmh;

	return pwme;
}
