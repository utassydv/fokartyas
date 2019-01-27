/*
 * communicationOUT.c
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#include <string.h>
#include "communicationOUT.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;


__IO uint32_t uwIC2Value = 0;
__IO uint32_t  uwDutyCycle = 0;


extern float szogseb;
extern float offsetszog;
extern int32_t currentX;
extern int32_t currentY;

extern uint8_t 	flagbluetooth;

void bluetoothTX(void)
{
	if (flagbluetooth == 1)
	{
		char TxData[16];
		snprintf(TxData, 16, "bluetooth\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)+1), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido

	}
	flagbluetooth = 0;
}

void bluetoothVSZ(void)
{

	if (flagbluetooth == 1)
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
	flagbluetooth = 0;
}


void bluetoothDRIVE(void)
{
	if (flagbluetooth == 1)
	{

		char TxData[100];
//		//sebesseg
//		snprintf(TxData, 20, "%ld,%ld\n",counterpres, speed);
//		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido

		//sebesseg
		int32_t szogsebki=szogseb;
		int32_t offsetszogki=offsetszog;
		int32_t currentXki =currentX;
		int32_t currentYki =currentY;

//		snprintf(TxData, 100, "%d,%d,%d,%d\n",count ,tav, pos, speed);
		snprintf(TxData, 100, "%d,%d\n",currentXki, currentYki);
		//snprintf(TxData, 100, "%d,%d,%d,%d,%d,%d\n",(int)speed,(int)epres,(int)u2,(int)u2prev,(int)u,(int)uprev);
		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido

	}
	flagbluetooth = 0;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)  //TAVIRANYITO ENGEDELYEZO JEL
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    /* Get the Input Capture value */
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

    if (uwIC2Value != 0)
    {
      uwDutyCycle = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    }
    else
    {
      uwDutyCycle = 150;
    }
  }

}
