/*
 * communicationvsz.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */
#include "stm32f4xx_hal.h"


#ifndef COMMUNICATIONVSZ_H_
#define COMMUNICATIONVSZ_H_

void communicationvszInit(void);
void vszRx(void);				//Vonalszenzor1 UART adatainak circ bufferbe toltese
void uartprocess(void);			//circ buffer feldolgozasa
uint8_t GETcount(void);
uint32_t GETtav(void);
uint32_t GETtav2(void);
//void vszdebugTx(void);



#endif /* COMMUNICATIONVSZ_H_ */
