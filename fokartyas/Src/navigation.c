/*
 * navigation.c
 *
 *  Created on: 1 Feb 2019
 *      Author: utassyd
 */
#include "stm32f4xx_hal.h"
#include "navigation.h"
#include "communicationvsz.h"
#include "linetracking.h"
#include "timing.h"
#include "actuator.h"
#include "controlSTEERING.h"

#include <stdlib.h>


uint8_t valtaslehetoseg = 0;
uint8_t statevonalvaltas = 0;

uint32_t tavolsag;

uint8_t randomiser(void)
{
	return GETtav()-((GETtav()/2)*2);
}



uint32_t GETtavolsag(void)
{
	return abs(GETtav()-GETtav2());
}


void vonalvaltas(void)
{
	if (GETflagvonalvaltas() == 1)
	{
		switch(statevonalvaltas)
		{
			case 0:
				labyrinth();
				if(GETcount() == 2 && GETtavolsag() < 5500)
				{
					statevonalvaltas = 1;
				}
				else if(GETcount() == 2 && GETtavolsag() > 5500 )
				{
					statevonalvaltas = 2;
				}
				break;
			case 1:
				kettohosszfv();
				if( GETkettohossz() > 30000)
				{
					SETvaltaslehetoseg(1);
					statevonalvaltas = 2;
				}
				if (GETcount() == 1)
				{
					SETkettohossz(0);
					statevonalvaltas=2;

				}
				break;

			case 2:
//				if(GETcount() == 2)
//				{
//					SETregivonal(GETvonal());
//				}
				//SETvaltaslehetoseg(0);
				if(GETcount() == 1)
				{
					SETkettohossz(0);
					SETflagketto(0);
					statevonalvaltas = 0;
				}

				break;

			default:
				statevonalvaltas = 0;
				break;
		}
	SETflagvonalvaltas(0);
	}
}






uint8_t GETvaltaslehetoseg()
{
	return valtaslehetoseg;
}

void SETvaltaslehetoseg(uint8_t ertek)
{
	valtaslehetoseg = ertek;
}

uint8_t GETstatevonalvaltas(void)
{
	return statevonalvaltas;
}
