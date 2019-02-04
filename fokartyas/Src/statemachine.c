/*
 * statemac.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include <statemachine.h>
#include "stm32f4xx_hal.h"
#include "timing.h"
#include "actuator.h"
#include "linetracking.h"
#include "tracking.h"
#include "communicationvsz.h"

uint8_t gyorsasagi = 0;
uint8_t state 					= 0;
uint8_t statelab 				= 0;
uint8_t SCstate 				= 0;
pont2D endlocation = { 0 , 0 };

uint8_t egyenescounter = 0;


void allapotgeplab(void)
{
	if (GETflagallapotgep() == 1)
	{
		switch(statelab)
		{
			case 0:
				labyrinth();
				egyutankettohossz();
				if(GEThossz() > 13000 && GEThossz() < 30000 ) //leghosszabb sávváltót lát
				{
					statelab = 1;
					SEThossz(0);
				}
				else if( GEThossz() > 5000 && GEThossz() < 13000 ) //legrovidebb
				{
					statelab = 2;
					SEThossz(0);
				}

				break;


			case 1:
				kettoutanegyhossz();
				if( GEThossz() > 13000 && GEThossz() < 30000) //leghosszabb savvalto lyukat lat
				{
					SEThossz(0);
					SETnullavonalszam(0);
					statelab = 3;
				}
				break;

			case 2:
				kettoutanegyhossz();
				if(GEThossz() > 5000 && GEThossz() < 13000) //legrovidebb savvalto lyukat lat
				{
					savelocation(endlocation);
					statelab = 0;
				}
				break;

			case 3:
				SETflagsavvaltas(1);
				savvaltas();
				if(GETnullavonalszam() >= 80)			//amig el nem hagyjuk a vonalat
				{
					SETegyvonalszam(0);
					statelab = 4;
				}
				break;

			case 4:
				if(GETegyvonalszam() >= 4)			//amíg meg nem jövünk a vonalra
				{
					SETflagsavvaltas(0);
					lassu();
					statelab = 0;
				}
				break;

			default:
				statelab = 0;
				break;
		}
	SETflagallapotgep(0);
	}
}

void SCallapotgep(void)
{

	if (GETflagSCkovet() == 1)
	{
		if (GETflagallapotgep() == 1)
		{
			switch(SCstate)
					{
						case 0:
							egyutanharomhossz();
							if(GEThossz() > 5000 && GEThossz() < 18000 )  //elso szaggatott haromvonalfigyelo
							{
								SCstate = 1;
								SEThossz(0);
							}
							break;

						case 1:
							haromutanegyhossz();
							if(GEThossz() > 5000 && GEThossz() < 20000)  //elso szaggatott egyvonalfigyelo, utana gyorsít
							{
								SCstate = 2;
								SETstartposition(GETcounterpres());
								SEThossz(0);
							}
							break;

						case 2:
							if (GETcounterpres() - GETstartposition() > 280000) //14000 x 2(m) // ennyi távolsagig nem figyeljuk a haromvonalat
							{
								SETharomvonalszam(0);
								egyenescounter++;
								if (egyenescounter == 2) 		SCstate = 8; //elozes
								else if (egyenescounter == 4) 	SETflagSCkovet(0);
								else							SCstate = 3;
							}
							break;

						case 3:
							if(GETcount() == 3)													//ha haromvonal, nezzok az elejet
							{
								SETstartposition(GETcounterpres());
								SCstate = 4;
							}
							break;

						case 4:
							if(GETcounterpres() - GETstartposition() > 42000) //  ha 30cm-en belül
							{
								if (GETharomvonalszam() >= 6)					// min.6x volt 6 vonal
								{
									state = 5;
								}
								else											//különben hiba volt, vissza
								{
									SETharomvonalszam(0);
									state = 3;
								}
							}
							break;

						case 5:
							if(GETcounterpres() - GETstartposition() > 280000)//2m kesobbb lassií
							{
								lassu() ;
								SETegyvonalszam(0);
								SCstate = 6;
							}
							break;
						case 6:
							if(GETcount() == 1)									//ha 1 vonal
							{
								SETstartposition(GETcounterpres());
								SCstate = 7;
							}
							break;
						case 7:
							if(GETcounterpres() - GETstartposition() > 28000) // 20cm-en belül
							{
								if (GETegyvonalszam() >= 4)					//4x van 1 vonal, akkor lassu szakasz, indul elolrol az allapotgep
								{
									SEThossz(0);
									SCstate = 0;
								}
								else										//hiba volt, vissza
								{
									SETegyvonalszam(0);
									SCstate = 6;
								}
							}
							break;

						case 8:
							SCstate = 0;
							break;

						default:
							SCstate = 0;
							break;
					}




			SETflagallapotgep(0);
		}
	}

}

void allapotgep(void)
{

	if (GETflagSCkovet() == 0)
	{
		if (GETflagallapotgep() == 1)
		{
			switch(state)
			{
				case 0:
					lassu();
					egyutanharomhossz();
					if(GEThossz() > 5000 && GEThossz() < 18000 )  //elso szaggatott haromvonalfigyelo
					{
						state = 1;
						SEThossz(0);
					}
					break;

				case 1:
					haromutanegyhossz();
					if(GEThossz() > 5000 && GEThossz() < 20000)  //elso szaggatott egyvonalfigyelo, utana gyorsít
					{
						state = 2;
						SETstartposition(GETcounterpres());
						SEThossz(0);
						gyors();
					}
					break;

				case 2:
					if (GETcounterpres() - GETstartposition() > 280000) //14000 x 2(m) // ennyi távolsagig nem figyeljuk a haromvonalat
					{
						SETharomvonalszam(0);
						state = 3;
					}
					break;

				case 3:
					if(GETcount() == 3)													//ha haromvonal, nezzok az elejet
					{
						SETstartposition(GETcounterpres());
						state = 4;
					}
					break;

				case 4:
					if(GETcounterpres() - GETstartposition() > 42000) //  ha 30cm-en belül
					{
						if (GETharomvonalszam() >= 6)					// min.6x volt 6 vonal
						{
							state = 5;
						}
						else											//különben hiba volt, vissza
						{
							SETharomvonalszam(0);
							state = 3;
						}
					}
					break;

				case 5:
					if(GETcounterpres() - GETstartposition() > 280000)//2m kesobbb lassií
					{
						lassu() ;
						SETegyvonalszam(0);
						state = 6;
					}
					break;
				case 6:
					if(GETcount() == 1)									//ha 1 vonal
					{
						SETstartposition(GETcounterpres());
						state = 7;
					}
					break;
				case 7:
					if(GETcounterpres() - GETstartposition() > 28000) // 20cm-en belül
					{
						if (GETegyvonalszam() >= 4)					//4x van 1 vonal, akkor lassu szakasz, indul elolrol az allapotgep
						{
							SEThossz(0);
							state = 0;
						}
						else										//hiba volt, vissza
						{
							SETegyvonalszam(0);
							state = 6;
						}
					}
					break;

				default:
					state = 0;
					break;
			}
		SETflagallapotgep(0);
		}
	}
}

uint8_t GETstatelab(void)
{
	return statelab;
}
