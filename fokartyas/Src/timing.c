/*
 * timing.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */
#include "stm32f4xx_hal.h"
#include "timing.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;

uint16_t 	timespeed 			= 0;
uint8_t 	flagspeed 			= 0;
uint16_t 	timevonalszam 		= 0;
uint8_t 	flagvonalszam 		= 0;
uint16_t 	timebeav 			= 0;
uint8_t 	flagbeav			= 0;
uint16_t 	timebluetooth 		= 0;
uint8_t 	flagbluetooth 		= 0;
uint16_t 	timeallapotgep 		= 0;
uint8_t 	flagallapotgep 		= 0;
uint16_t 	timeuartproc 		= 0;
uint8_t 	flaguartproc 		= 0;
uint16_t 	timeangle 			= 0;
uint8_t 	flagangle			= 0;
uint16_t 	timeangleoffset 	= 0;
uint8_t 	flagangleoffset		= 0;
uint16_t 	timeregulator	 	= 0;
uint8_t 	flagregulator		= 0;
uint16_t 	timepoz	 			= 0;
uint8_t 	flagpoz				= 0;
uint16_t 	timevonalvaltas	 	= 0;
uint8_t 	flagvonalvaltas		= 0;
uint16_t 	timeradio		 	= 0;
uint8_t 	flagradio			= 0;
uint16_t 	timekovet		 	= 0;
uint8_t 	flagkovet			= 0;

uint32_t tavszenzorcnt = 0;

void idozitoInit(){
	  HAL_TIM_Base_Start_IT(&htim4);					//Idozitesekhez (1ms)
	  HAL_TIM_Base_Start_IT(&htim5);					//Idozitesekhez (1ms)
	  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
}

void idozito( uint16_t ido, uint16_t *idocount, uint8_t *flag)
{
	  *idocount = *idocount +1;
	  if(*idocount == ido) //x*1ms-kent sebesseg meres
	  {
		  *flag = 1;
		  *idocount = 0;
	  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  UNUSED(htim);

  if(htim->Instance == TIM4)	//1ms
  {

	  idozito( 5, &timespeed, &flagspeed); 				//idozites sebesseg meres 	(ido(ms), szamlalo, flag)

	  idozito( 5, &timebluetooth, &flagbluetooth);		//idozites bluetooth(ido	(ms), szamlalo, flag)

	  idozito( 5, &timevonalszam, &flagvonalszam); 		//idozites vonalszamlalas 	(ido(ms), szamlalo, flag)

	  idozito( 5, &timeallapotgep, &flagallapotgep);	//idozites allapotgep		(ido(ms), szamlalo, flag)

	  idozito( 5, &timebeav, &flagbeav);				//idozites beavatkzosas		(ido(ms), szamlalo, flag)

	  idozito( 5, &timeuartproc, &flaguartproc); 		//idozites uart circ buff feldolg 	(ido(ms), szamlalo, flag)

	  idozito( 5, &timeangle, &flagangle); 				//szogseb meres 	(ido(ms), szamlalo, flag)

	  idozito( 5, &timeangleoffset, &flagangleoffset);

	  idozito( 5, &timeregulator, &flagregulator);

	  idozito( 5, &timepoz, &flagpoz);

	  idozito( 5, &timevonalvaltas, &flagvonalvaltas);

	  idozito( 100, &timeradio, &flagradio);

	  idozito( 50, &timekovet, &flagkovet);

  }


}

//GETTEREK

uint8_t GETflagspeed(void)
{
	return flagspeed;
}

uint8_t GETflagbluetooth(void)
{
	return flagbluetooth;
}

uint8_t GETflagvonalszam(void)
{
	return flagvonalszam;
}

uint8_t GETflagallapotgep(void)
{
	return flagallapotgep;
}

uint8_t GETflagbeav(void)
{
	return flagbeav;
}

uint8_t GETflaguartproc(void)
{
	return flaguartproc;
}

uint8_t GETflagangle(void)
{
	return flagangle;
}

uint8_t GETflagangleoffset(void)
{
	return flagangleoffset;
}

uint8_t GETflagregulator(void)
{
	return flagregulator;
}

uint8_t GETflagpoz(void)
{
	return flagpoz;
}

uint8_t GETflagvonalvaltas(void)
{
	return flagvonalvaltas;
}

uint8_t GETflagradio(void)
{
	return flagradio;
}

uint8_t GETflagkovet(void)
{
	return flagkovet;
}
//SETTEREK

void SETflagspeed(uint8_t ertek)
{
	flagspeed = ertek;
}

void SETflagbluetooth(uint8_t ertek)
{
	flagbluetooth = ertek;
}

void SETflagvonalszam(uint8_t ertek)
{
	flagvonalszam = ertek;
}

void SETflagallapotgep(int8_t ertek)
{
	flagallapotgep = ertek;
}

void SETflagbeav(uint8_t ertek)
{
	flagbeav = ertek;
}

void SETflaguartproc(uint8_t ertek)
{
	flaguartproc = ertek;
}

void SETflagangle(uint8_t ertek)
{
	flagangle = ertek;
}

void SETflagangleoffset(uint8_t ertek)
{
	flagangleoffset = ertek;
}

void SETflagregulator(uint8_t ertek)
{
	flagregulator = ertek;
}

void SETflagpoz(uint8_t ertek)
{
	flagpoz = ertek;
}

void SETflagvonalvaltas(uint8_t ertek)
{
	flagvonalvaltas = ertek;
}

void SETflagradio(uint8_t ertek)
{
	flagradio = ertek;
}

void SETflagkovet(uint8_t ertek)
{
	flagkovet = ertek;
}




