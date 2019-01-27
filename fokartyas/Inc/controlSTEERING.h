/*
 * controlSTEERING.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#ifndef CONTROLSTEERING_H_
#define CONTROLSTEERING_H_

int32_t toerror(uint32_t dist);
int32_t szabPD(int32_t elozohibajel, int32_t hibajel);
int16_t toPWM(int32_t jel);


#endif /* CONTROLSTEERING_H_ */
