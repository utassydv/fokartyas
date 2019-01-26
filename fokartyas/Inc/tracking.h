/*
 * tracking.h
 *
 *  Created on: 20 Jan 2019
 *      Author: utassyd
 */

#ifndef TRACKING_H_
#define TRACKING_H_

void speedpos(void);
void angle(void);
void enablegyro(void);
void gyro(void);

void poz(void);
float deltax(int vpalya, float vszog);
float deltay(int vpalya, float vszog);

#endif /* TRACKING_H_ */
