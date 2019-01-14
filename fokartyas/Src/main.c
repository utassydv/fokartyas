
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

#include <string.h>

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


float p			=	2.5f;
float d			=	10.0f;
int16_t v 		=	1500;

float plassu	=	2.5f;
float dlassu	=	10.0f;
uint16_t vlassu	=	1625;
float scalelassu=	0;

float pgyors	=	0.25f;
float dgyors	=	5.0f;
uint16_t vgyors	=	1710;
uint16_t vfek	=	1050;
float scalegyors=	0;

uint32_t counterpres=0;
uint32_t counterprev=0;
int32_t speed=0;

uint16_t 	timespeed 			= 0;
uint8_t 	flagspeed 			= 0;
uint16_t 	timevonalszam 		= 0;
uint8_t 	flagvonalszam 		= 0;
uint16_t 	timebeav 			= 0;
uint8_t 	flagbeav			= 0;
uint16_t 	timebluetooth 		= 0;
uint8_t 	flagbluetooth 		= 0;
uint16_t 	timeallapotgep 		= 0;
uint8_t 	flagallapotgep 		= 0;
uint16_t 	timeuartproc 		= 0;
uint8_t 	flaguartproc 		= 0;

uint32_t cntbeav = 0;
uint8_t state = 0;

uint8_t RxBuff;
uint8_t count;    //vonal db szám
uint32_t tav;
int16_t pos;
int16_t posh;

uint8_t szaml=0;
uint8_t olveleje=0;
uint8_t feldvege=0;
char posarray[64];
uint8_t data[64];

int32_t hiba=0;
int32_t elozohiba=0;
int32_t beavatkozo=0;

uint32_t egyvonalszam = 0;
uint32_t haromvonalszam = 0;


__IO uint32_t uwIC2Value = 0;
__IO uint32_t  uwDutyCycle = 0;

char TxDatak[200];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM13_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM14_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void speedpos(void);
void vonalszamlalo(void);
void idozito(uint16_t ido, uint16_t *idocount, uint8_t *flag);
void uartprocess(void);
void allapotgep(void);
int16_t toPWM(int32_t jel);
int32_t szabPD(int32_t elozohibajel, int32_t hibajel);
int32_t toerror(uint32_t tavolsag);
void lassu(void);
void gyors(void);
void fekez(void);
void control(void);
void bluetoothTX(void);
void bluetoothVSZ(void);
void bluetoothDRIVE(void);
uint8_t engedelyezo(uint32_t pwminput);
void vszRx(void);
void vszdebugTx(void);
void velocity(int16_t sebesseg);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  //TimHandle.Instance = TIM1;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM13_Init();
  MX_TIM4_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);			//PWM Motor
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);			//PWM Motor
  HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);		//PWM Szervo elso
  HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);		//PWM Szervo hatso
  HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);	//Inkrementalis ado
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);		//Taviranyito CH1
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);		//Taviranyito CH2
  HAL_TIM_Base_Start_IT(&htim4);					//Idozitesekhez (1ms)

  HAL_UART_Receive_IT(&huart4, &RxBuff, 1);			//Vonalszenzor1 kommunikacio
 //HAL_UART_Receive_IT(&huart4, (uint8_t*)TxDatak, 200);			//Vonalszenzordebug kommunikacio

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
{
	speedpos();			//sebesseg es pozicio meres

	vonalszamlalo(); 	//vonalszam figyeles

	uartprocess(); 		//UART feldolgozasa

	allapotgep();		//state megadasa

	if (flagbeav == 1)
	{
		velocity(100);


		hiba 		= 	toerror(tav);
		beavatkozo	= 	szabPD(elozohiba, hiba);
		elozohiba	=	hiba;
		pos 		= 	toPWM(beavatkozo);
		posh		=	1500;

		if( engedelyezo(uwDutyCycle) == 0)
		{
			pos		=	1500;
			posh	=	1500;
			v		=   1500;
		}

		control();
		flagbeav = 0;
	}




	//bluetoothTX();
	bluetoothVSZ();
	//bluetoothDRIVE();


}

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */



  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 839;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xFFFF;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchronization(&htim1, &sSlaveConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim3.Init.Period = 1049;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 525;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 83;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM13 init function */
static void MX_TIM13_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;

  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 83;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 3999;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim13) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim13, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim13);

}

/* TIM14 init function */
static void MX_TIM14_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 83;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 15999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim14);

}

/* UART4 init function */
static void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC7   ------> USART6_RX
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA12 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  vszRx();
  //vszdebugTx();
}

void vszdebugTx(void)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)TxDatak, strlen(TxDatak)+1 , HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido
	HAL_UART_Receive_IT(&huart4, (uint8_t*)TxDatak, 200);
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



int32_t toerror(uint32_t dist)
{
	int32_t tavolsag=dist;
	if (tavolsag > 25599 || tavolsag < 0) tavolsag=12799;	//ha nemletezeo tavolsag, akkor kozep tavolsag.
	tavolsag=tavolsag-12799;

	return tavolsag;
}

int32_t szabPD(int32_t elozohibajel, int32_t hibajel)
{
	int32_t beavatkozo=0;

	beavatkozo= (p*(float)hibajel) + d*(((float)hibajel- (float)elozohibajel));


	return beavatkozo;
}

int16_t toPWM(int32_t jel)
{

	int16_t pwm=1500+((20800000/25598)*(jel))/26000; //KORM�?NY

	if(pwm<1100) pwm=1100;
	if(pwm>1900) pwm=1900;

	return pwm;
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



uint8_t engedelyezo(uint32_t pwminput)
{
	if (pwminput>160)
		return 1;
	else
		return 0;
}


void idozito( uint16_t ido, uint16_t *idocount, uint8_t *flag)
{
	  *idocount = *idocount +1;
	  if(*idocount == ido) //x*1ms-kent sebesseg meres
	  {
		  *flag = 1;
		  *idocount = 0;
	  }
}

void speedpos(void)				//sebesseg es pozicio merese
{
	if (flagspeed == 1)
	{
		counterprev = counterpres;
		counterpres = TIM2->CNT;
		speed= counterpres - counterprev;
		flagspeed = 0;
	}


}

void vonalszamlalo(void)	//vonalfigyelo
{
	if (flagvonalszam == 1)
	{
		if(count == 1) egyvonalszam++;
		if(count == 3) haromvonalszam++;
		flagvonalszam = 0;
	}
}

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

		char TxData[16];
		//snprintf(TxData, 16, "%u,%u,%u,%u,%u,%i,%i,%u,%u,%i,%i\n", state, count, egyvonalszam, haromvonalszam, tav, hiba, beavatkozo, pos, counterpres, speed, v); //"2,150000'\0'"
		snprintf(TxData, 16, "%u,%u,%u\n",haromvonalszam, state, count);

		HAL_UART_Transmit(&huart2, (uint8_t *)TxData, (strlen(TxData)+1), HAL_MAX_DELAY); //melyik, mit, mennyi, mennyi ido

	}
	flagbluetooth = 0;
}

void uartprocess(void)
{
	if (flaguartproc == 1)
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

		 flaguartproc = 0;
	}
}

void allapotgep(void)
{
	if (flagallapotgep == 1) {
		switch(state) {
			case 0: 						// lassu, ha 3 vonal -> state 1
				lassu();
				if(haromvonalszam == 3) {
					egyvonalszam   = 0;
					haromvonalszam = 0;
					state = 1;
				}
				break;

			case 1: 						// gyorsit 1, ha 1 vonal -> state 2
				if(egyvonalszam == 3) {
					egyvonalszam   = 0;
					haromvonalszam = 0;
					state = 2;
				}
				break;

			case 2: 						// gyorsit 2, ha 3 vonal -> state 3, gyorsit
				if(haromvonalszam == 3) {
					egyvonalszam   = 0;
					haromvonalszam = 0;
					state = 3;
					lassu(); //gyors();
				}
				break;

			case 3: 						// gyors eleje, 200ms-t var -> state 3
				if(++cntbeav > 20) {
					cntbeav = 0;
					state = 4;
				}
				break;

			case 4: 						// gyors allapot, ha 3 vonal -> fekez, state 5
				if(haromvonalszam == 10) {
					fekez();
					state = 5;
				}
				break;

			case 5: 						// fekezes eleje, var 500ms-ot, state 6, lassu
				if(++cntbeav > 50) {
					state = 6;
					cntbeav = 0;
					lassu();
				}
				break;

			case 6: 						// 1 vonal -> state 0, lassu
				if(egyvonalszam == 10)
				{
					haromvonalszam = 0;
					state = 0;
				}
				break;

			default:						//default: lassu
				state = 0;
				break;
		}
	}

	flagallapotgep = 0;
}

void gyors(void)
{
	p= pgyors;
	d= dgyors;
	v= vgyors;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

void fekez(void)
{
	p= plassu;
	d= dlassu;
	v= vfek;
}

void lassu(void)
{
	p= plassu;
	d= dlassu;
	v= vlassu;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

void control(void)
{
	htim13.Instance->CCR1 	= pos; 		//elso szervo
	htim14.Instance->CCR1 	= posh; 	//hatso szervo
	//htim3.Instance->CCR3 	= v;		//motor
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  UNUSED(htim);

  if(htim->Instance == TIM4)	//1ms
  {

	  idozito( 10, &timespeed, &flagspeed); 				//idozites sebesseg meres 	(ido(ms), szamlalo, flag)

	  idozito( 10, &timevonalszam, &flagvonalszam); 		//idozites vonalszamlalas 	(ido(ms), szamlalo, flag)

	  idozito( 10, &timeallapotgep, &flagallapotgep);		//idozites allapotgep		(ido(ms), szamlalo, flag)

	  idozito( 10, &timebeav, &flagbeav);					//idozites beavatkzosas		(ido(ms), szamlalo, flag)

	  idozito( 500, &timebluetooth, &flagbluetooth);		//idozites bluetooth(ido	(ms), szamlalo, flag)

	  idozito( 10, &timeuartproc, &flaguartproc); 			//idozites uart circ buff feldolg 	(ido(ms), szamlalo, flag)
  }

}

void velocity(int16_t sebesseg)
{
	uint16_t csat1;
	uint16_t csat2;

	if(sebesseg > 524) 		sebesseg = 524;
	if(sebesseg < -523) 	sebesseg = -523;

	csat1 = 525 + sebesseg;
	csat2 = 525 - sebesseg;

	htim3.Instance->CCR3 	= csat1;
	htim3.Instance->CCR4 	= csat2;
}




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
