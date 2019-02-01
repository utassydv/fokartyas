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

#include <stdlib.h>


uint8_t amelyik = 0;
uint8_t statevonalvaltas = 0;

uint32_t tavolsag;


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
				lassu();
				if(GETcount() == 2 && GETtavolsag() < 5000)
				{
					statevonalvaltas = 1;
				}
				else if(GETcount() == 2)
				{
					statevonalvaltas = 2;
				}
				break;
			case 1:

				if( GETkettohossz() > 30000)
				{
					SETamelyik(1);
					statevonalvaltas = 2;
				}
				break;

			case 2:
				if(GETcount() == 1)
				{
					SETkettohossz(0);
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






uint8_t GETamelyik()
{
	return amelyik;
}

void SETamelyik(uint8_t ertek)
{
	amelyik = ertek;
}


