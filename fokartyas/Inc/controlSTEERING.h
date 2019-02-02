/*
 * controlSTEERING.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"

#ifndef CONTROLSTEERING_H_
#define CONTROLSTEERING_H_


void controlSTEERINGInit(void);
int32_t toerror(uint32_t dist);
int32_t szabPD(int32_t elozohibajel, int32_t hibajel);
int16_t toPWM(int32_t jel);
void toservo(void);
uint32_t vonalvalasztas(void);
void irany(uint8_t merre);

float GETp(void);
void SETp(float ertek);
float GETd(void);
void SETd(float ertek);
float GETplassu(void);
void SETplassu(float ertek);
float GETdlassu(void);
void SETdlassu(float ertek);
float GETpgyors(void);
void SETpgyors(float ertek);
float GETdgyors(void);
void SETdgyors(float ertek);
uint16_t GETpwmmide(void);
uint16_t GETpwmmidh(void);
int16_t GETpos(void);
void SETpos(int16_t ertek);
int16_t GETposh(void);
void SETposh(int16_t ertek);
uint8_t GETflaglassu(void);
void SETflaglassu(uint8_t ertek);
uint8_t GETflaggyors(void);
void SETflaggyors(uint8_t ertek);
void SETregivonal(uint32_t ertek);
uint32_t GETvonal(void);
uint8_t mindigvalt(void);

#endif /* CONTROLSTEERING_H_ */
