/*
 * statemac.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

void SCallapotgep(void);
void allapotgeplab(void);
void allapotgep(void);
uint8_t GETstatelab(void);
uint8_t GETstate(void);
uint8_t GETSCstate(void);
uint8_t GETegyenescounter(void);

#endif /* STATEMACHINE_H_ */
