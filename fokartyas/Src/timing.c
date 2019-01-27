/*
 * timing.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */
#include <stdint.h>
#include "timing.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

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

void idozitoInit(){
	  HAL_TIM_Base_Start_IT(&htim4);					//Idozitesekhez (1ms)
	  HAL_TIM_Base_Start_IT(&htim5);					//Idozitesekhez (1ms)
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

	  idozito( 1, &timeangleoffset, &flagangleoffset);

  }

}



