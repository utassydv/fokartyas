/*
 * linetracking.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "timing.h"
#include "linetracking.h"
#include "tracking.h"
#include "communicationvsz.h"
#include "stm32f4xx_hal.h"

uint32_t nullavonalszam = 0;
uint32_t egyvonalszam = 0;
uint32_t haromvonalszam = 0;

uint8_t flagharom = 0;
uint8_t flagketto = 0;
uint32_t hossz = 0;

int32_t kettohossz = 0;

int32_t startposition;



void vonalszamlalo(void)	//vonalfigyelo
{
	if (GETflagvonalszam() == 1)
	{
		if(GETcount() == 0) nullavonalszam++;
		if(GETcount() == 1) egyvonalszam++;
		if(GETcount() == 3) haromvonalszam++;
		SETflagvonalszam(0);
	}
}

uint32_t egyutanharomhossz(void)   //csak biztosan egy vonal esetén hívható meg
{
		if(GETcount() == 3 && flagharom == 0)
		{
			startposition = GETcounterpres();
			flagharom = 1;
		}

		if (GETcount()==1 && startposition != 0)
		{
			hossz = GETcounterpres() - startposition;
			startposition = 0;
			return hossz;
		}
		return 0;
}

uint32_t egyutankettohossz(void)
{
		if(GETcount() == 2 && flagketto == 0)
		{
			startposition = GETcounterpres();
			flagketto = 1;
		}

		if (GETcount()==1 && startposition != 0)
		{
			hossz = GETcounterpres() - startposition;
			startposition = 0;
			return hossz;
		}
		return 0;
}


uint32_t haromutanegyhossz(void)   //csak biztosan egy vonal esetén hívható meg
{
		if(GETcount() == 1 && flagharom == 1)
		{
			startposition = GETcounterpres();
			flagharom = 0;
		}
		if (GETcount()==3 && startposition != 0)
		{
			hossz = GETcounterpres() - startposition;
			startposition = 0;
			return hossz;
		}
	return 0;
}

uint32_t kettoutanegyhossz(void)
{
	if(GETcount() == 1 && flagketto == 1)
	{
		startposition = GETcounterpres();
		flagketto = 0;
	}
	if (GETcount()==2 && startposition != 0)
	{
		hossz = GETcounterpres() - startposition;
		startposition = 0;
		return hossz;
	}
	return 0;
}

void kettohosszfv(void)
{
		if(GETcount() == 2 && flagketto == 0)
		{
			startposition = GETcounterpres();
			flagketto = 1;
		}

		if (GETcount()==2 && startposition != 0)
		{
			kettohossz = GETcounterpres() - startposition;
		}
}

int32_t GETkettohossz(void)
{
	return kettohossz;
}
void SETkettohossz(int32_t ertek)
{
	kettohossz = ertek;
}

uint32_t GETnullavonalszam(void)
{
	return nullavonalszam;
}

uint32_t GETegyvonalszam(void)
{
	return egyvonalszam;
}

uint32_t GETharomvonalszam(void)
{
	return haromvonalszam;
}


void SETnullavonalszam(uint32_t ertek)
{
	nullavonalszam = ertek;
}
void SETegyvonalszam(uint32_t ertek)
{
	egyvonalszam = ertek;
}
void SETharomvonalszam(uint32_t ertek)
{
	haromvonalszam = ertek;
}



uint32_t GEThossz(void)
{
	return hossz;
}

void SEThossz(uint32_t ertek)
{
	hossz = ertek;
}

int32_t GETstartposition(void)
{
	return startposition;
}

void SETstartposition(int32_t ertek)
{
	startposition = ertek;
}

void SETflagketto(uint8_t ertek)
{
	flagketto = ertek;
}

void SETflagharom(uint8_t ertek)
{
	flagharom = ertek;
}



