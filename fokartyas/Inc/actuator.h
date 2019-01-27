/*
 * actuator.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

void beavatkozas(void);
void gyors(void);
void fekez(void);
void lassu(void);
void labyrinth(void);
void savvaltas(void);
void control(void);
void motorvezerles(int16_t sebesseg);
void szervovezerles(int16_t elsoszervo, int16_t hatsoszervo);
uint32_t GETuwDutyCycle(void);

#endif /* ACTUATOR_H_ */
