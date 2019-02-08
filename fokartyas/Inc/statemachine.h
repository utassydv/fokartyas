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
void allapotgepinkr(void);
void allapotgepinkrement(void);
uint8_t GETstatelab(void);
uint8_t GETstate(void);
uint8_t GETSCstate(void);
uint8_t GETegyenescounter(void);
void randomlab (void);
uint8_t GETstaterandom(void);
uint8_t GETgyorsasagi(void);
void SETgyorsasagi(uint8_t ertek);
void racse(void);
uint8_t GETstateeloz(void);

#endif /* STATEMACHINE_H_ */
