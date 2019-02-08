/*
 * controlSTEERING.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#include "controlSTEERING.h"
#include "communicationvsz.h"
#include "navigation.h"
#include "actuator.h"
#include <stdlib.h>
#include "tracking.h"
#include "math.h"


int32_t Ykivant = 	0;
float p			=	2.5f;
float d			=	10.0f;

float plassu	=	1.8f;  //2.7 // mielőtt elkezdtem 2.3  // 2.0-val 28s
float dlassu	=	8.0f;  //8.0  // 8.0
float scalelassu=	0.5f;

//float pgyors		=	0.8f;
//float dgyors		=	5.0f;
//float scalegyors	=	1.0f;

float pgyors		=	0.6f;   //0.8  Ez maradt
float dgyors		=	15.0f;  //15.0
float scalegyors	=	0.0f;

float peloz		=	0.8f;
float deloz	=	15.0f;
float scaleeloz	=	1.0f;


//float pgyors	=	2.7f;
//float dgyors	=	10.0f;
//float scalegyors=	0.7f;


const uint16_t pwmmide	=	1500;
const uint16_t pwmmidh 	=	1500;
const uint16_t rangee 	=	345;
const uint16_t rangeh	=	400;

int16_t pos;
int16_t posh;




int32_t hiba		= 0;
int32_t elozohiba	= 0;
int32_t beavatkozo	= 0;

uint32_t vonal		= 12799;
uint32_t regivonal	= 12799;

uint8_t kisorol = 0;
uint8_t besorol = 0;
uint8_t giroszab = 0;



extern TIM_HandleTypeDef htim10; 						//szenzor szervo timer
extern TIM_HandleTypeDef htim13; 						//elsp szervo timer
extern TIM_HandleTypeDef htim14; 						//hatso szervo timer

void controlSTEERINGInit(void)
{
	  HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);		//PWM Szervo szenzor
	  HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);		//PWM Szervo elso
	  HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);		//PWM Szervo hatso
}

int32_t toerror(uint32_t dist)
{
	int32_t tavolsag=dist;
	//if (tavolsag > 25599 || tavolsag < 0) tavolsag=12799;	//ha nemletezeo tavolsag, akkor kozep tavolsag.

	if(giroszab == 1)
	{
		tavolsag =  Ykivant + GETcurrentY() + 54000*(float)sinf(GETszog()*0.01745329252f);
	}
	else if(kisorol == 1)
	{
		tavolsag = (GETcurrentX() + 54000*(float)cosf(GETszog()*0.01745329252f)) /100 - GETcurrentY() - 54000*(float)cosf(GETszog()*0.01745329252f);
	}
	else if(besorol == 1)
	{
		tavolsag = -GETcurrentX() + 54000*(float)sinf(GETszog()*0.01745329252f) + GETcurrentY() - 54000*(float)cosf(GETszog()*0.01745329252f);
	}
	else tavolsag=tavolsag-12799;

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
	if (GETflaglassu() == 1)
	{
		pwmh=pwmmidh-scalelassu*jel*(rangeh*100000/12799)/100000; //KORM�?NY
		if(pwmh<pwmmidh-rangeh) pwmh=pwmmidh-rangeh;
		if(pwmh>pwmmidh+rangeh) pwmh=pwmmidh+rangeh;
	}
	if (GETflaggyors() == 1)
	{
		pwmh=pwmmidh+scalegyors*jel*(rangeh*1000000/12799)/100000; //KORM�?NY
		if(pwmh<pwmmidh-rangeh) pwmh=pwmmidh-rangeh;
		if(pwmh>pwmmidh+rangeh) pwmh=pwmmidh+rangeh;
	}
	if (GETflageloz() == 1)
		{
			pwmh=pwmmidh-scaleeloz*jel*(rangeh*1000000/12799)/100000; //KORM�?NY
			if(pwmh<pwmmidh-rangeh) pwmh=pwmmidh-rangeh;
			if(pwmh>pwmmidh+rangeh) pwmh=pwmmidh+rangeh;
		}
	posh	=	pwmh;

	return pwme;
}

uint32_t vonalvalasztas(void)
{
		if( GETcount() == 2)
		{
			//regivonal = vonal;
			if ( abs((int32_t)regivonal-(int32_t)GETtav()) < abs((int32_t)regivonal - (int32_t)GETtav2())) //regit koveti
			{
				vonal = GETtav();
			}
			else
			{
				vonal = GETtav2();
			}

		}
		else
		{
			vonal = GETtav();
		}
		return vonal;

}

void toservo(void)
{
	hiba 		= 	toerror(vonalvalasztas());


	if(GETvaltaslehetoseg() == 1)
	{
		//irany(mindigvalt());
		//irany(1);
		irany(randomiser());
		SETvaltaslehetoseg(0);
	}


	beavatkozo	= 	szabPD(elozohiba, hiba);
	elozohiba	=	hiba;
	toPWM(beavatkozo);
}

void irany(uint8_t merre)
{
	if (merre == 0) regivonal = GETtav2();
	if (merre == 1) regivonal = GETtav();
}

uint8_t mindigvalt(void)
{
	if ( abs((int32_t)regivonal-(int32_t)GETtav()) /*jobb*/    <     /*bal*/  abs((int32_t)regivonal - (int32_t)GETtav2())) //regit koveti
	{
					return 0;
	}
	else
	{
					return 1;
	}
}

void steeringSETTER(void)
{

	if (GETflagSCkovet() == 1)
	{

		if (GETflageloz() == 1)
		{
			SETp(GETpeloz());
			SETd(GETdeloz());
		}
		else
		{
			SETp(GETplassu());
			SETd(GETdlassu());
		}
	}
	if (GETflaglassu() == 1)
	{
		SETp(GETplassu());
		SETd(GETdlassu());
	}
	if (GETflaggyors() == 1)
	{
		SETp(GETpgyors());
		SETd(GETdgyors());
	}
	if (GETflageloz() == 1)
	{
		SETp(GETpeloz());
		SETd(GETdeloz());
	}
	if (GETflagfekez() == 1)
	{
		SETp(GETplassu());
		SETd(GETdlassu());
	}

}

float GETp(void)
{
	return p;
}
void SETp(float ertek)
{
	p = ertek;
}

float GETd(void)
{
	return d;
}

void SETd(float ertek)
{
	d = ertek;
}



float GETplassu(void)
{
	return plassu;
}
void SETplassu(float ertek)
{
	plassu = ertek;
}

float GETdlassu(void)
{
	return dlassu;
}
void SETdlassu(float ertek)
{
	dlassu = ertek;
}



float GETpgyors(void)
{
	return pgyors;
}
void SETpgyors(float ertek)
{
	pgyors = ertek;
}

float GETpeloz(void)
{
	return peloz;
}
void SETpeloz(float ertek)
{
	peloz = ertek;
}

float GETdgyors(void)
{
	return dgyors;
}
void SETdgyors(float ertek)
{
	dgyors = ertek;
}

float GETdeloz(void)
{
	return deloz;
}
void SETdeloz(float ertek)
{
	deloz = ertek;
}

uint16_t GETpwmmide(void)
{
	return pwmmide;
}

uint16_t GETpwmmidh(void)
{
	return pwmmidh;
}

int16_t GETpos(void)
{
	return pos;
}

void SETpos(int16_t ertek)
{
	pos = ertek;
}

int16_t GETposh(void)
{
	return posh;
}

void SETposh(int16_t ertek)
{
	posh = ertek;
}

uint32_t GETvonal(void)
{
	return vonal;
}

uint32_t GETregivonal(void)
{
	return regivonal;
}

void SETregivonal(uint32_t ertek)
{
	regivonal = ertek;
}

void SETkisorol(uint8_t ertek)
{
	kisorol = ertek;
}

void SETbesorol(uint8_t ertek)
{
 besorol = ertek;
}

void SETgiroszab(uint8_t ertek)
{
	giroszab = ertek;
}

int32_t GETkivantY()
{
	return Ykivant;
}

uint8_t GETkisorol(void)
{
	return kisorol;
}

uint8_t GETgiroszab(void)
{
	return giroszab;
}




