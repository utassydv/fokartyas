/*
 * controlVELOCITY.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"

#ifndef CONTROLVELOCITY_H_
#define CONTROLVELOCITY_H_

void controlVELOCITYInit(void);
float szabvPI(float err);
float toinkrspeed(float sebesseg);
void tomotorcontrol(void);
void velocitySETTER(void);

void SETv(float ertek);
float GETvlassu();
float GETvgyors();
float GETvsavvalt();

float GETvfek();
float GETupres();
float GETu2prev();
void SETuprev(float ertek);
void SETupres(float ertek);
void SETu2prev(float ertek);
uint32_t GETvonal(void);
uint32_t GETregivonal(void);
float GETv(void);



#endif /* CONTROLVELOCITY_H_ */
