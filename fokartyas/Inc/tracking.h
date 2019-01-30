/*
 * tracking.h
 *
 *  Created on: 20 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"

#ifndef TRACKING_H_
#define TRACKING_H_

typedef struct pont2D {
    int32_t x, y;
} pont2D;

void trackingInit(void);
void enablegyro(void);
void gyro(void);
void gyrooffset(void);
void angle(void);
void poz(void);
void temp(void);
float deltax(int vpalya, float vszog);
float deltay(int vpalya, float vszog);
void speedpos(void);
void savelocation(pont2D location);
float GETszogseb(void);
int32_t GETcurrentX(void);
int32_t GETcurrentY(void);
int32_t GETcounterpres(void);
int32_t GETcounterprev(void);
int32_t GETspeed(void);
float GEToffsetszog(void);
float GETszog(void);
uint16_t GEToffsetcnt(void);
float GETTEMPERATURE(void);


#endif /* TRACKING_H_ */
