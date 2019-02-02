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
#include "MadgwickAHRS.h"


extern SPI_HandleTypeDef hspi2;


uint8_t txdata1[13];
uint8_t rxdata1[13];

uint8_t enabledata[3];

uint8_t orient[2];


uint16_t offsetcnt = 0;
uint16_t offsetlimit = 6200;
float offsetszogsebX = 0;
float offsetszogsebY = 0;
float offsetszogsebZ = 0;
float offsetaX = 0;
float offsetaY = 0;
float offsetaZ = 0;
float szogseb;
float szog = 0;

int32_t counterpres	= 0;
int32_t counterprev	= 0;
int32_t speed		= 0;

int32_t currentX = 0;
int32_t currentY = 0;

float gX;
float gY;
float gZ;
float aX;
float aY;
float aZ;

extern TIM_HandleTypeDef htim2; 						//encoder timer

void trackingInit(void)
{
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);	//Inkrementalis ado

	//Z tengely koruli szogsebesseg olvasasara
	txdata1[0]		=	0b10100010; // (22h)  00100010
	txdata1[1]		=	0b00000000;
	txdata1[2]		=	0b00000000;
	txdata1[3]		=	0b00000000;
	txdata1[4]		=	0b00000000;
	txdata1[5]		=	0b00000000;
	txdata1[6]		=	0b00000000;
	txdata1[7]		=	0b00000000;
	txdata1[8]		=	0b00000000;
	txdata1[9]		=	0b00000000;
	txdata1[10]		=	0b00000000;
	txdata1[11]		=	0b00000000;
	txdata1[12]		=	0b00000000;

	//giroszkop és acc engedelyezese
	enabledata[0]	= 0b00010000; //CTRL1_XL (10h), CTRL2_G (11h)
	enabledata[1]	= 0b01010000; //208Hz....
	enabledata[2]	= 0b01010000; //208Hz....

	orient[0]	= 0b00001011;
	orient[1]	= 0b00000000;


	enablegyro();
}


//GIROSZKOP ENGEDELYEZESE, CTRL2_G (11h)
void enablegyro(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, enabledata, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, orient, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

//ADAT KIOLVASAS GIROSZKOPBOL
void gyro(void)
{
	if (offsetcnt < offsetlimit)	//OFFSET atlag db. szam
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		if (offsetcnt > 1200)
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
		HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 13, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD

		adat 	= rxdata1[2] << 8;
		adat 	|=  rxdata1[1];
		gX	= (float)adat*8.75f;  //4.3140960937-< 125dpsnel
		offsetszogsebX = 0.0002f*gX+offsetszogsebX; //átlag

		adat 	= rxdata1[4] << 8;
		adat 	|=  rxdata1[3];
		gY	= (float)adat*8.75f;  //4.3140960937-< 125dpsnel
		offsetszogsebY = 0.0002f*gY+offsetszogsebY; //átlag

		adat 	= rxdata1[6] << 8;
		adat 	|=  rxdata1[5];
		gZ	= (float)adat*8.75f;  //4.3140960937-< 125dpsnel
		offsetszogsebZ = 0.0002f*gZ+offsetszogsebZ; //átlag

		adat 	= rxdata1[8] << 8;
		adat 	|=  rxdata1[7];
		aX	= (float)adat*0.061f;
		offsetaX = 0.0002f*aX+offsetaX; //átlag

		adat 	= rxdata1[10] << 8;
		adat 	|=  rxdata1[9];
		aY	= (float)adat*0.061f;
		offsetaY = 0.0002f*aY+offsetaY; //átlag

		adat 	= rxdata1[12] << 8;
		adat 	|=  rxdata1[11];
		aZ	= (float)adat*0.061f;
		offsetaZ = 0.0002f*aZ+offsetaZ; //átlag


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
		HAL_SPI_TransmitReceive(&hspi2, txdata1, rxdata1, 13, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD


		adat 	= rxdata1[2] << 8;
		adat 	|=  rxdata1[1];
		gX=((float)adat*8.75f-offsetszogsebX)*0.001f;  // 4.3140960937f //8.61262521018907


		adat 	= rxdata1[4] << 8;
		adat 	|=  rxdata1[3];
		gY=((float)adat*8.75f-offsetszogsebY)*0.001f;  // 4.3140960937f //8.61262521018907


		adat 	= rxdata1[6] << 8;
		adat 	|=  rxdata1[5];
		gZ=((float)adat*8.75f-offsetszogsebZ)*0.001f;  // 4.3140960937f //8.61262521018907


		adat 	= rxdata1[8] << 8;
		adat 	|=  rxdata1[7];
		aX	= (float)adat*0.061f-offsetaX;

		adat 	= rxdata1[10] << 8;
		adat 	|=  rxdata1[9];
		aY	= (float)adat*0.061f-offsetaY;

		adat 	= rxdata1[12] << 8;
		adat 	|=  rxdata1[11];
		aZ	= (float)adat*0.061f-offsetaZ;

//		if (szog >= 180.0f) szog = szog-360.0f;
//		if (szog <= -180.0f) szog = szog+360.0f;

		MadgwickAHRSupdateIMU(gX*0.01745329252f, gY*0.01745329252f, gZ*0.01745329252f, aX, aY, aZ);

		SETflagangle(0);
	}
}

//void temp(void)				//z elfordulas kiolvasas//////////////////
//{
//	if (GETflagangle() == 1)
//	{
//		int16_t adattemp;
//
//
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); 			//CS
//		HAL_SPI_TransmitReceive(&hspi2, txdata1TEMP, rxdata1TEMP, 3, HAL_MAX_DELAY);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); 				//CD
//
//
//		adattemp 	= rxdata1TEMP[2] << 8;
//		adattemp 	|=  rxdata1TEMP[1];
//
//		TEMPERATURE	=	(float)adattemp;
//
//		szog = szog+szogseb;
//
////		if (szog >= 180.0f) szog = szog-360.0f;
////		if (szog <= -180.0f) szog = szog+360.0f;
//
//
//
//		SETflagangle(0);
//	}
//}


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

	float kosz=(float)cosf(vszog);
	return uthossz*kosz;
}

float deltay(int vpalya, float vszog)
{
	float uthossz=((float)vpalya)*0.7142857143f;    //   /1.40

	float szin=(float)sinf(vszog);
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



float GETszog(void)
{
	return szog;
}

uint16_t GEToffsetcnt(void)
{
	return offsetcnt;
}

void SETszog(float ertek)
{
	szog= ertek;
}

float GETgX(void)
{
	return gX;
}
float GETgY(void)
{
	return gY;
}
float GETgZ(void)
{
	return gZ;
}

float GETaX(void)
{
	return aX;
}
float GETaY(void)
{
	return aY;
}
float GETaZ(void)
{
	return aZ;
}





