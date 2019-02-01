/*
 * communicationOUT.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "stm32f4xx_hal.h"
#ifndef COMMUNICATIONOUT_H_
#define COMMUNICATIONOUT_H_

void bluetoothTX(void);
void bluetoothVSZ(void);
void bluetoothDRIVE(void);
void communicationOUTInit(void);
void radioRx(void);
uint8_t GETstartjel(void);

#endif /* COMMUNICATIONOUT_H_ */
