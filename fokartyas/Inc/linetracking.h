/*
 * linetracking.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"

#ifndef LINETRACKING_H_
#define LINETRACKING_H_

void vonalszamlalo(void);	//vonalfigyelo
uint32_t egyutanharomhossz(void);   //csak biztosan egy vonal esetén hívható meg
uint32_t egyutankettohossz(void);
uint32_t haromutanegyhossz(void);  //csak biztosan egy vonal esetén hívható meg
uint32_t kettoutanegyhossz(void);
uint32_t GETnullavonalszam(void);
uint32_t GETegyvonalszam(void);
uint32_t GETharomvonalszam(void);
void SETnullavonalszam(uint32_t ertek);
void SETegyvonalszam(uint32_t ertek);
void SETharomvonalszam(uint32_t ertek);
uint32_t GEThossz(void);
void SEThossz(uint32_t ertek);
int32_t GETstartposition(void);
void SETstartposition(int32_t ertek);
int32_t GETkettohossz(void);
void kettohosszfv(void);
void SETkettohossz(int32_t ertek);
void SETflagketto(uint8_t ertek);




#endif /* LINETRACKING_H_ */
