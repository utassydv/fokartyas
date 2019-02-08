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
#include "controlSTEERING.h"
#include "communicationvsz.h"
#include "navigation.h"
#include "actuator.h"
#include "math.h"

uint8_t startjel;
uint32_t idomero = 0;


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
			if(GETflagSTART() == 1)
			{
				idomero++;
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
		int32_t szogki=GETszog()*100;
		int32_t szogsebki=GETszogseb();
//		int32_t gXki=GETgX();
//		int32_t gYki=GETgY();
//		int32_t gZki=GETgZ();
//		int32_t aXki=GETaX();
//		int32_t aYki=GETaY();
//		int32_t aZki=GETaZ();
		int32_t currentXki =GETcurrentX() + 54000*(float)cosf(GETszog()*0.01745329252f);  // -> cm
		int32_t currentYki =GETcurrentY() + 54000*(float)sinf(GETszog()*0.01745329252f);
		int32_t vki =GETv()*1000;
		int32_t vkikovet =GETvkovet()*1000;
		int32_t epreski =GETepres();




//		snprintf(TxData, 100, "%d,%d,%d,%d\n",count ,tav, pos, speed);
		//snprintf(TxData, 100, "%d,%d\n",currentXki,currentYki);
		//snprintf(TxData, 100, "%u\n",GETstartjel());
		//snprintf(TxData, 100, "%d\n",szogki);
		//snprintf(TxData, 100, "%d,%d,%d,%d,%d,%d\n",(int)speed,(int)epres,(int)u2,(int)u2prev,(int)u,(int)uprev);
		//snprintf(TxData, 100, "%d,%d,%d,%d,%d,%d\n",GETstatevonalvaltas(),GETcount(), GETkettohossz(), GETtav(),GETtav2(),GETtavolsag());
		//snprintf(TxData, 100, "%d, %d,%d,%d,%d,%d,%d,%d,%d\n",GETflaglassu(), GETflaggyors(), GETflagsavvalt(), GETflagfekez() , GETflagSCkovet());
		//snprintf(TxData, 100, "%d,%d,%d\n",currentXki ,currentYki,szogki);
		//snprintf(TxData, 100, "%d\n",GETuwDutyCycle());
		//snprintf(TxData, 100, "%d, %d\n",GETstaterandom(),GETkettohossz());
		snprintf(TxData, 100, "%d\n",idomero);




		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido
		SETflagbluetooth(0);

	}

}

uint8_t GETstartjel(void)
{
	return startjel;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

uint32_t GETidomero(void)
{
	return idomero;
}

