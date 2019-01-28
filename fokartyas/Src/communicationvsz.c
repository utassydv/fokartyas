/*
 * communicationvsz.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include "communicationvsz.h"
#include "stm32f4xx_hal.h"
#include "timing.h"

extern UART_HandleTypeDef huart4;
//extern UART_HandleTypeDef huart2;

uint8_t szaml=0;
uint8_t RxBuff;
uint8_t data[64];
uint8_t feldvege=0;
uint8_t olveleje=0;
char posarray[64];

uint8_t count 	= 0;
uint32_t tav 	= 12799;


void communicationvszInit(void)
{
		HAL_UART_Receive_IT(&huart4, &RxBuff, 1);
}




void vszRx(void) 				//Vonalszenzor1 UART adatainak circ bufferbe toltese
{
	    data[szaml]=RxBuff;
	    if(RxBuff == '\0')
	    {
	  	  feldvege=szaml;
	    }

	    if(szaml == 63)
	    {
	  	  szaml=0;
	    }
	    else
	    {
	  	  szaml++;
	    }
	    HAL_UART_Receive_IT(&huart4, &RxBuff, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		/* Prevent unused argument(s) compilation warning */
		UNUSED(huart);
		vszRx();
		//vszdebugTx();
}

void uartprocess(void)
{
	if (GETflaguartproc() == 1)
	{
		//UART ERTELMEZES
		//Egy tombbe toltes
		uint8_t x=0;
		while(olveleje != feldvege)
		{
			if(olveleje == 63)
			{
				olveleje=0;
			}
			else
			{
				olveleje++;
			}
			posarray[x++]= data[olveleje];
		}
		olveleje =feldvege;

		//tomb szetbontas
		 sscanf(posarray, "%d,%d\0", &count, &tav); // "1,15315'\0'"

		 SETflaguartproc(0);
	}
}

uint8_t GETcount(void)
{
	return count;
}

uint32_t GETtav(void)
{
	return tav;
}


//void vszdebugTx(void)// ezt nem hasznaljuk, a vonalszenzornak a hulyesegeinek megoldasara lehet jo
//{
//	HAL_UART_Transmit(&huart2, (uint8_t *)TxDatak, strlen(TxDatak)+1 , HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido
//	HAL_UART_Receive_IT(&huart4, (uint8_t*)TxDatak, 200);
//}
