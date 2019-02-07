/*
 * tracking.c
 *
 *  Created on: 20 Jan 2019
 *      Author: utassyd
 */
#include "tracking.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "timing.h"


extern SPI_HandleTypeDef hspi2;



uint8_t txdata1[3];
uint8_t rxdata1[3];

uint8_t txdata1TEMP[3];
uint8_t rxdata1TEMP[3];

uint8_t enabledata[2];
uint8_t TEMPenabledata[2];

uint16_t offsetcnt = 0;
uint16_t offsetlimit = 1500;
float offsetszog;
float szogseb;
float szog = 0;
float TEMPERATURE = 0;

int32_t counterpres	= 0;
int32_t counterprev	= 0;
int32_t speed		= 0;

float autohossz=54000;


int32_t currentX = 0;
int32_t currentY = 0;

extern TIM_HandleTypeDef htim2; 						//encoder timer

void trackingInit(void)
{
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);	//Inkrementalis ado
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);	//Inkrementalis ado

	//Z tengely koruli szogsebesseg olvasasara
	txdata1[0]		=	0b10100110; //OUTZ_L_G (26h)
	txdata1[1]		=	0b00000000;
	txdata1[2]		=	0b00000000;

	//temp olvasasara
	txdata1TEMP[0]		=	 0b10100000; //(20h)
	txdata1TEMP[1]		=	0b00000000;
	txdata1TEMP[2]		=	0b00000000;

	//giroszkor engedelyezese
	enabledata[0]	= 0b00010001; //CTRL2_G (11h)
	enabledata[1]	= 0b01010000; //208Hz....


	TEMPenabledata[0]	= 0b00010011; //h13
	TEMPenabledata[1]	= 0b00010000;

	enablegyro();
}


//GIROSZKOP ENGEDELYEZESE, CTRL2_G (11h)
void enablegyro(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, enabledata, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, TEMPenabledata, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

//ADAT KIOLVASAS GIROSZKOPBOL
void gyro(void)
{
	if (offsetcnt < offsetlimit)	//OFFSET atlag db. szam
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		if (offsetcnt > 500)
		{
		gyrooffset();
		}
		else
		{
			if(GETflagangleoffset()==1)
			{
			offsetcnt++;
			SETflagangleoffset(0);
			}
		}
	}

	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		angle();
		poz();
	}

	//temp();

}


//GIROSZKOP OFFSETELESE
void gyrooffset(void)
{
	if (GETflagangleoffset() == 1)
	{
		int16_t adat;

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
		HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 3, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


		adat 	= rxdata1[2] << 8;
		adat 	|=  rxdata1[1];
		szogseb	= (float)adat*8.61262521018907f;  //4.3140960937-< 125dpsnel,, 8.61262521018907-250dps


		offsetszog = 0.001f*szogseb+offsetszog; //Štlag

		offsetcnt++;
		SETflagangleoffset(0);
	}
}

void angle(void)				//z elfordulas kiolvasas//////////////////
{
	if (GETflagangle() == 1)
	{
		int16_t adat;


		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
		HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 3, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


		adat 	= rxdata1[2] << 8;
		adat 	|=  rxdata1[1];

		szogseb=((float)adat*8.61262521018907f-offsetszog)/200000.0f;  // 4.3140960937f
		szog = szog+szogseb;

//		if (szog >= 180.0f) szog = szog-360.0f;
//		if (szog <= -180.0f) szog = szog+360.0f;



		SETflagangle(0);
	}
}

void temp(void)				//z elfordulas kiolvasas//////////////////
{
	if (GETflagangle() == 1)
	{
		int16_t adattemp;


		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
		HAL_SPI_TransmitReceive(&hspi2, txdata1TEMP, rxdata1TEMP, 3, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


		adattemp 	= rxdata1TEMP[2] << 8;
		adattemp 	|=  rxdata1TEMP[1];

		TEMPERATURE	=	(float)adattemp;

		szog = szog+szogseb;

//		if (szog >= 180.0f) szog = szog-360.0f;
//		if (szog <= -180.0f) szog = szog+360.0f;



		SETflagangle(0);
	}
}


void poz(void)
{

	if (GETflagpoz() == 1)
	{
		currentX = currentX + deltax(speed,szog);
		currentY = currentY + deltay(speed,szog);
		SETflagpoz(0);
	}
}

float deltax(int vpalya, float vszog)
{

	float uthossz=((float)vpalya)*0.7142857143f;   //     /1.40

	float kosz=(float)cosf(vszog*0.01745329252f);
	return uthossz*kosz;
}

float deltay(int vpalya, float vszog)
{
	float uthossz=((float)vpalya)*0.7142857143f;    //   /1.40

	float szin=(float)sinf(vszog*0.01745329252f);
	return uthossz*szin;
}


//SEBBESSEG ES POZICIO MERESE
void speedpos(void)
{
	counterprev = counterpres;
	counterpres = TIM2->CNT;
	speed= counterpres - counterprev;
}

void savelocation(pont2D location)
{
	location.x = currentX;
	location.y = currentY;
}

float GETszogseb(void)
{
	return szogseb;
}

int32_t GETcurrentX(void)
{
	return currentX;
}

int32_t GETcurrentY(void)
{
	return currentY;
}

int32_t GETcounterpres(void)
{
	return counterpres;
}

int32_t GETcounterprev(void)
{
	return counterprev;
}

int32_t GETspeed(void)
{
	return speed;
}


float GEToffsetszog(void)
{
	return offsetszog;
}

float GETszog(void)
{
	return szog;
}

void SETszog(float ertek)
{
	szog=ertek;
}


void SETcurrentX(int32_t ertek)
{
	currentX=ertek;
}

void SETcurrentY(int32_t ertek)
{
	currentY=ertek;
}

uint16_t GEToffsetcnt(void)
{
	return offsetcnt;
}

float GETTEMPERATURE(void)
{
	return TEMPERATURE;
}






