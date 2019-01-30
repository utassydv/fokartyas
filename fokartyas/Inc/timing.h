/*
 * timing.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"

#ifndef TIMING_H_
#define TIMING_H_


void idozitoInit();
void idozito(uint16_t ido, uint16_t *idocount, uint8_t *flag);


uint8_t GETflagspeed(void);
uint8_t GETflagbluetooth(void);
uint8_t GETflagvonalszam(void);
uint8_t GETflagallapotgep(void);
uint8_t GETflagbeav(void);
uint8_t GETflaguartproc(void);
uint8_t GETflagangle(void);
uint8_t GETflagangleoffset(void);
uint8_t GETflagregulator(void);
uint8_t GETflagpoz(void);

void SETflagspeed(uint8_t ertek);
void SETflagbluetooth(uint8_t ertek);
void SETflagvonalszam(uint8_t ertek);
void SETflagallapotgep(int8_t ertek);
void SETflagbeav(uint8_t ertek);
void SETflaguartproc(uint8_t ertek);
void SETflagangle(uint8_t ertek);
void SETflagangleoffset(uint8_t ertek);
void SETflagpoz(uint8_t ertek);

#endif /* TIMING_H_ */
