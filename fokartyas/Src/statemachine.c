/*
 * statemachine.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */
#include <stdint.h>
#include "statemachine.h"
#include "stm32f4xx_hal.h"

void allapotgeplab(void)
{
	if (flagallapotgep == 1)
	{
		switch(statelab)
		{
			case 0:
				labyrinth();
				egyutankettohossz();
				if(hossz > 13000 && hossz < 30000 ) //leghosszabb sávváltót lát
				{
					statelab = 1;
					hossz = 0;
				}
				else if(hossz > 5000 && hossz < 13000 ) //legrovidebb
				{
					statelab = 2;
					hossz = 0;
				}

				break;


			case 1:
				kettoutanegyhossz();
				if(hossz > 13000 && hossz < 30000) //leghosszabb savvalto lyukat lat
				{
					hossz = 0;
					nullavonalszam = 0;
					statelab = 3;
				}
				break;

			case 2:
				kettoutanegyhossz();
				if(hossz > 5000 && hossz < 13000) //legrovidebb savvalto lyukat lat
				{
					savelocation(endlocation);
					statelab = 0;
				}
				break;

			case 3:
				flagsavvaltas = 1;
				savvaltas();
				if(nullavonalszam >= 20)			//amig el nem hagyjuk a vonalat
				{
					egyvonalszam = 0;
					statelab = 4;
				}
				break;

			case 4:
				if(egyvonalszam >= 4)			//amíg meg nem jövünk a vonalra
				{
					flagsavvaltas = 0;
					lassu();
					statelab = 0;
				}
				break;

			default:
				statelab = 0;
				break;
		}
	flagallapotgep = 0;
	}
}

void allapotgep(void)
{
	if (flagallapotgep == 1)
	{
		switch(state)
		{// lassu, ha 3 vonal -> state 1

			case 0:
				lassu();
				egyutanharomhossz();
				if(hossz > 5000 && hossz < 18000 )
				{
					state = 1;
					hossz = 0;
				}
				break;

			case 1:
				haromutanegyhossz();
				if(hossz > 5000 && hossz < 20000) {
					state = 2;
					startposition = counterpres;
					hossz = 0;
					gyors();
				}
				break;

			case 2:
				if (counterpres - startposition > 280000) //14000 x 2(m)
				{
					haromvonalszam = 0;
					state = 3;
				}
				break;

			case 3:
				if(count == 3)
				{
					startposition = counterpres;
					state = 4;
				}
				break;

			case 4:
				if(counterpres - startposition > 42000) // 30cm-en belül
				{
					if (haromvonalszam >= 6) state = 5;
					else
					{
						haromvonalszam = 0;
						state = 3;
					}
				}
				break;

			case 5:
				if(counterpres - startposition > 280000)//2m
				{
					lassu() ;
					egyvonalszam = 0;
					state = 6;
				}
				break;
			case 6:
				if(count == 1)
				{
					startposition = counterpres;
					state = 7;
				}
				break;
			case 7:
				if(counterpres - startposition > 28000) // 2ű0cm-en belül
				{
					if (egyvonalszam >= 4)
					{
						hossz = 0;
						state = 0;
					}
					else
					{
						egyvonalszam = 0;
						state = 6;
					}
				}
				break;

			default:
				state = 0;
				break;
		}
	flagallapotgep = 0;
	}
}
