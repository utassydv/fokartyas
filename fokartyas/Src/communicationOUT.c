/*
 * communicationOUT.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include <string.h>
#include "communicationOUT.h"
#include "timing.h"
#include "stm32f4xx_hal.h"
#include "tracking.h"
#include "linetracking.h"
#include "statemachine.h"
#include "controlVELOCITY.h"
#include "communicationvsz.h"
#include "navigation.h"
#include "actuator.h"

uint8_t startjel;


extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef husart6;

void communicationOUTInit(void)
{
		HAL_UART_Receive_IT(&husart6, &startjel, 1);
}

void radioRx(void) 				//Vonalszenzor1 UART adatainak circ bufferbe toltese
{
		HAL_UART_Receive_IT(&husart6, &startjel, 1);
}

void trackRADIO(void)
{

		if (GETflagradio() == 1) {
			if(startjel == '0')
			{
				SETflagSTART(1);
			}
		SETflagradio(0);
		}
}

void bluetoothTX(void)
{
	if (GETflagbluetooth() == 1)
	{
		char TxData[16];
		snprintf(TxData, 16, "bluetooth\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)+1), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido
	}
	SETflagbluetooth(0);
}

void bluetoothVSZ(void)
{

	if (GETflagbluetooth() == 1)
	{
		uint8_t adcMeasures[32];

		for(int i=0; i<32; i++)
		{
			adcMeasures[i]=100;
		}


		char TxDatak[200];
		snprintf(TxDatak,200,"%u,",adcMeasures[0] );

		for(int i=1; i<32; i++)
		{
			char TxBuf[5];
			snprintf(TxBuf,5,"%u,", adcMeasures[i]);
			strcat(TxDatak, TxBuf);
		}
		strcat(TxDatak, "\n");

		HAL_UART_Transmit(&huart2, (uint8_t *)TxDatak, strlen(TxDatak)+1 , HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido
	}
	SETflagbluetooth(0);
}



void bluetoothDRIVE(void)
{
	if (GETflagbluetooth() == 1)
	{

		char TxData[100];
//		//sebesseg
//		snprintf(TxData, 20, "%ld,%ld\n",counterpres, speed);
//		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido

		//sebesseg
		int32_t szogki=GETszog()*1000000;
		int32_t szogsebki=GETszogseb();
		int32_t gXki=GETgX();
		int32_t gYki=GETgY();
		int32_t gZki=GETgZ();
		int32_t aXki=GETaX();
		int32_t aYki=GETaY();
		int32_t aZki=GETaZ();
		int32_t currentXki =GETcurrentX()*0.001;  // -> cm
		int32_t currentYki =GETcurrentY()*0.001;


//		snprintf(TxData, 100, "%d,%d,%d,%d\n",count ,tav, pos, speed);
		//snprintf(TxData, 100, "%d,%d\n",currentXki,currentYki);
		//snprintf(TxData, 100, "%u\n",GETstartjel());
		snprintf(TxData, 100, "%d,%d,%d,%d,%d,%d,%d\n",gXki,gYki,gZki,aXki,aYki,aZki,szogki);
		//snprintf(TxData, 100, "%d,%d,%d,%d,%d,%d\n",(int)speed,(int)epres,(int)u2,(int)u2prev,(int)u,(int)uprev);
		//snprintf(TxData, 100, "%d,%d,%d,%d,%d,%d\n",GETstatevonalvaltas(),GETcount(), GETkettohossz(), GETtav(),GETtav2(),GETtavolsag());

		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido
		SETflagbluetooth(0);

	}

}

uint8_t GETstartjel(void)
{
	return startjel;
}
