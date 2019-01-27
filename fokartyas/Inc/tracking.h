/*
 * tracking.h
 *
 *  Created on: 20 Jan 2019
 *      Author: utassyd
 */

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


#endif /* TRACKING_H_ */
