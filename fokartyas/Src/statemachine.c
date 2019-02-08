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
#include "communicationOUT.h"
#include "controlSTEERING.h"
#include "navigation.h"

uint8_t gyorsasagi 				= 0;
uint8_t state 					= 0;
uint8_t statelab 				= 0;
uint8_t staterandom				= 0;
uint8_t stateeloz				= 0;
uint8_t SCstate 				= 0;
pont2D endlocation 				= { 0 , 0 };

uint8_t elozesflag=0;

uint8_t egyenescounter = 0;
uint8_t biztonsag=0;
uint32_t distance = 0;


void randomlab (void)
{
	if (GETflagallapotgep() == 1)
		{
		switch(staterandom)
		{
			case 0:
				labyrinth();
				if(GETcount() == 2 && GETtavolsag() < 5500)
				{
					SETflagketto(0);
					distance=GETcounterpres();
					staterandom = 1;
				}
				else if(GETcount() == 2 && GETtavolsag() > 5500 )
				{
					staterandom = 2;
				}

				break;


			case 1:
				egyutankettohossz();
				if(GEThossz() > 15000 && GEThossz() < 30000 && GETidomero() > 600) //leghosszabb sávváltót lát
				{
					staterandom = 3;
					distance=GETcounterpres();
					SEThossz(0);
				}
				else if( GEThossz() > 3000 && GEThossz() < 15000 && GETidomero() > 600) //legrovidebb
				{
					staterandom = 7;
					distance=GETcounterpres();
					SEThossz(0);
				}
				else if(GETcounterpres() - distance > 33000)
				{
					staterandom=6;
					SEThossz(0);
				}
				break;

			case 2:
				if(GETcount() == 1)
				{
					SETkettohossz(0);
					SETflagketto(0);
					staterandom = 0;
				}
				break;

			case 3:
				kettoutanegyhossz();
				if( GEThossz() > 13000 && GEThossz() < 30000) //leghosszabb savvalto lyukat lat
				{
					SEThossz(0);
					SETnullavonalszam(0);
					staterandom = 4;
				}
				else if(GETcounterpres() - distance > 33000)
				{
					staterandom=0;
					SEThossz(0);
				}
				break;

			case 4:
				visszasavvaltas();
				if(GETnullavonalszam() >= 80)			//amig el nem hagyjuk a vonalat
				{
					SETegyvonalszam(0);
					staterandom = 5;
				}
				break;
			case 5:
				if(GETegyvonalszam() >= 4)			//amíg meg nem jövünk a vonalra
				{
					lassu();
					//staterandom = ?;   				safetycar állapotgép kezdete
				}
				break;


			case 6:

					SETvaltaslehetoseg(1);
					staterandom = 2;

				break;
			case 7:
				kettoutanegyhossz();
				if(GEThossz() > 3000 && GEThossz() < 15000) //legrovidebb savvalto lyukat lat
				{

					staterandom = 8;
				}
				else if(GETcounterpres() - distance > 33000)
				{
					staterandom=0;
					SEThossz(0);
				}
				break;
			case 8:
				distance=GETcounterpres();
				visszafordul();
				if(GETnullavonalszam() >= 80)			//amig el nem hagyjuk a vonalat
				{
					SETegyvonalszam(0);
					staterandom = 9;
				}
				break;
			case 9:
				if(GETcounterpres() - distance > 140000)
					staterandom=10;
				break;
			case 10:
				if(GETegyvonalszam() >= 3)			//amíg meg nem jövünk a vonalra
				{
					lassu();
					//staterandom = ?;   				safetycar állapotgép kezdete
				}
				break;

			default:
				statelab = 0;
				break;
		}
		SETflagallapotgep(0);
		}
}



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
				visszasavvaltas();
				if(GETnullavonalszam() >= 80)			//amig el nem hagyjuk a vonalat
				{
					SETegyvonalszam(0);
					statelab = 4;
				}
				break;

			case 4:
				if(GETegyvonalszam() >= 4)			//amíg meg nem jövünk a vonalra
				{
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

void allapotgepelozes(void)
{
	if (GETflagallapotgep() == 1 && elozesflag == 1)
	{
		switch(stateeloz)
		{
			case 0:
				lassu();
				SETstartposition(GETcounterpres());
				stateeloz=1;

				break;


			case 1:
				if( GETcounterpres() - GETstartposition() > 100000)
				{
					stateeloz=2;
					SETszog(0.0f);
					savvaltas();
					SETstartposition(GETcounterpres());
				}
				break;

			case 2:

				if(GETcounterpres() - GETstartposition() > 240000)
				{
					stateeloz=3;
					SETcurrentY(0.0f);
					SETstartposition(GETcounterpres());
				}
				break;

			case 3:
				SETgiroszab(1);
				eloz();
				SETflagsavvalt(0);
				SETplassu(1.0f);
				SETdlassu(15.0f);
				if(GETcounterpres() - GETstartposition() > 750000)
				{
					stateeloz=4;
					SETstartposition(GETcounterpres());
					biztonsag=0;
					SETgiroszab(0);
					lassu();
				}
				break;

			case 4:
				visszasavvaltas();

				if(GETcount() > 0)
					biztonsag++;
				if(biztonsag==2)
				{
					SETplassu(2.7f);
					SETdlassu(8.0f);
					stateeloz=5;
					SETstartposition(GETcounterpres());
				}
				break;

			case 5:
				lassu();
				if(GETcounterpres() - GETstartposition() > 700000)
				{
					stateeloz=6;
				}
				break;

			case 6:
				gyors();
				SETflagSCkovet(1);
				elozesflag=0;
				SEThossz(0);
				SCstate = 6;
				SETharomvonalszam(0);
				break;


			default:
				stateeloz = 0;
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
							SCkovet();
							if (GETcount() == 3 )													//ha haromvonal, nezzok az elejet
							{
								SETstartposition(GETcounterpres());
								SCstate = 1;
							}
							break;

						case 1:
							if(GETcounterpres() - GETstartposition() > 8000) //  ha xcm-en belül
							{
								if (GETharomvonalszam() >=4)					// min.6x volt 6 vonal
								{
									SCstate = 3;
								}
								else											//különben hiba volt, vissza
								{
									SETharomvonalszam(0);
									SCstate = 0;
								}
							}
							break;
						case 3:
							if (GETcounterpres() - GETstartposition() >  2117000) 					//kovetkezo fekezo sav elsoharmada!!!!!!!
								{
									SETegyvonalszam(0);
									SCstate = 4;

								}
							break;
						case 4:
							if (GETcount() == 1 )													//ha egyvonal, nezzok az elejet
							{
								SETstartposition(GETcounterpres());
								SCstate = 5;
							}
							break;

						case 5:
							if(GETcounterpres() - GETstartposition() > 8000) //  ha xcm-en belül
							{
								if (GETegyvonalszam() >=4)					// min.6x volt 6 vonal
								{
									SCstate =6;
								}
								else											//különben hiba volt, vissza
								{
									SETegyvonalszam(0);
									SCstate = 4;
								}
							}
							break;
						case 6:
							if (GETcounterpres()  - GETstartposition() > 759000) 						//elozes eleje hossz!!!!!!!!!
							{
								SETstartposition(GETcounterpres());
								SCstate = 7;
							}
							break;

						case 7:
							if( GETcounterpres() - GETstartposition() > 100000)		//egyenesbe allas
							{

								SETszog(0.0f);
								savvaltas();
								SETstartposition(GETcounterpres());
								SCstate = 8;
							}
							break;

						case 8:
							if(GETcounterpres() - GETstartposition() > 180000)		//kimeno hossz
							{
								SETcurrentY(0.0f);
								SETstartposition(GETcounterpres());
								SCstate = 9;

							}
							break;

						case 9:
							SETgiroszab(1);
							eloz();
							SETflagsavvalt(0);
							if(GETcounterpres() - GETstartposition() > 500000)		//elozesi hossz!!!!!!!
							{
								biztonsag=0;
								SETgiroszab(0);
								lassu();
								SCstate = 10;

								SETstartposition(GETcounterpres());

							}
							break;

						case 10:
							visszasavvaltas();
							if(GETcount() > 0)
								biztonsag++;
							else biztonsag = 0;
							if(biztonsag==3)
							{
								SETstartposition(GETcounterpres());
								lassu();
								SCstate = 11;
							}
							break;

						case 11:
							if(GETcounterpres() - GETstartposition() > 2850000)
							{
								SETflagSCkovet(0);
							}
							break;

						default:
							SCstate = 0;
							state = 0;
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
						if (GETharomvonalszam() >= 6)					// min.6x volt 3 vonal
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

uint8_t GETstaterandom(void)
{
	return staterandom;
}


uint8_t GETstate(void)
{
	return state;
}

uint8_t GETSCstate(void)
{
	return SCstate;
}

uint8_t GETegyenescounter(void)
{
	return egyenescounter;
}

