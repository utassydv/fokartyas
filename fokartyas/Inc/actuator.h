/*
 * actuator.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

void actuatorInit(void);
void regulator(void);

void gyors(void);
void fekez(void);
void lassu(void);
void labyrinth(void);
void savvaltas(void);
void SCkovet(void);

void control(void);
void motorvezerles(float sebesseg);
void szervovezerles(int16_t elsoszervo, int16_t hatsoszervo);
uint32_t GETuwDutyCycle(void);
uint8_t GETflagsavvaltas(void);
void SETflagsavvaltas(uint8_t ertek);
void SETflagSTART(uint8_t ertek);
uint8_t GETflagSTART(void);
uint32_t GETSCtavolsag(void);


uint8_t GETflaglassu(void);
void SETflaglassu(uint8_t ertek);
uint8_t GETflaggyors(void);
void SETflaggyors(uint8_t ertek);
uint8_t GETflagfekez(void);
void SETflagfekez(uint8_t ertek);
uint8_t GETflagsavvalt(void);
void SETflagsavvalt(uint8_t ertek);
uint8_t GETflagSCkovet(void);

void SETflagSCkovet(uint8_t ertek);



#endif /* ACTUATOR_H_ */
