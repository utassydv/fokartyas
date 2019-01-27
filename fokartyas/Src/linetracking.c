/*
 * linetracking.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include <stdint.h>
#include "linetracking.h"
#include "stm32f4xx_hal.h"

uint32_t nullavonalszam = 0;
uint32_t egyvonalszam = 0;
uint32_t haromvonalszam = 0;

uint8_t flagharom = 0;
uint8_t flagketto = 0;
uint32_t hossz = 0;



extern uint8_t 	flagvonalszam ;
extern uint8_t count;
extern int32_t counterpres;
extern int32_t startposition;


void vonalszamlalo(void)	//vonalfigyelo
{
	if (flagvonalszam == 1)
	{
		if(count == 0) nullavonalszam++;
		if(count == 1) egyvonalszam++;
		if(count == 3) haromvonalszam++;
		flagvonalszam = 0;
	}
}

uint32_t egyutanharomhossz(void)   //csak biztosan egy vonal esetén hívható meg
{
		if(count == 3 && flagharom == 0)
		{
			startposition = counterpres;
			flagharom = 1;
		}

		if (count==1 && startposition != 0)
		{
			hossz = counterpres - startposition;
			startposition = 0;
			return hossz;
		}
		return 0;
}

uint32_t egyutankettohossz(void)
{
		if(count == 2 && flagketto == 0)
		{
			startposition = counterpres;
			flagketto = 1;
		}

		if (count==1 && startposition != 0)
		{
			hossz = counterpres - startposition;
			startposition = 0;
			return hossz;
		}
		return 0;
}


uint32_t haromutanegyhossz(void)   //csak biztosan egy vonal esetén hívható meg
{
		if(count == 1 && flagharom == 1)
		{
			startposition = counterpres;
			flagharom = 0;
		}
		if (count==3 && startposition != 0)
		{
			hossz = counterpres - startposition;
			startposition = 0;
			return hossz;
		}
	return 0;
}

uint32_t kettoutanegyhossz(void)
{
	if(count == 1 && flagketto == 1)
	{
		startposition = counterpres;
		flagketto = 0;
	}
	if (count==2 && startposition != 0)
	{
		hossz = counterpres - startposition;
		startposition = 0;
		return hossz;
	}
	return 0;
}
