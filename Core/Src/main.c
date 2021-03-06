/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void GPIO_SET_MDIO_MODE_INPUT() {
	// Set MIDO pin to it's default status with HAL_GPIO_DeInit
	HAL_GPIO_DeInit(MIIM_MDIO_GPIO_Port, MIIM_MDIO_Pin);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
 	__HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, LED_RED_Pin|MIIM_MDC_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin : LED_BLUE_Pin */
	  GPIO_InitStruct.Pin = LED_BLUE_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(LED_BLUE_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : LED_RED_Pin MIIM_MDC_Pin LED_GREEN_Pin */
	  GPIO_InitStruct.Pin = LED_RED_Pin|MIIM_MDC_Pin|LED_GREEN_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pin : MIIM_MDIO_Pin */
	  GPIO_InitStruct.Pin = MIIM_MDIO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(MIIM_MDIO_GPIO_Port, &GPIO_InitStruct);

}

void GPIO_SET_MDIO_MDC_MODE_INPUT() {
	// Set MIDO pin to it's default status with HAL_GPIO_DeInit
	HAL_GPIO_DeInit(MIIM_MDIO_GPIO_Port, MIIM_MDIO_Pin);

	// Set MDC pin to its default status with HAL_GPIO_Deinit
	HAL_GPIO_DeInit(MIIM_MDC_GPIO_Port, MIIM_MDC_Pin);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
 	__HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin : LED_BLUE_Pin */
	  GPIO_InitStruct.Pin = LED_BLUE_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(LED_BLUE_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : LED_RED_Pin MIIM_MDC_Pin LED_GREEN_Pin */
	  GPIO_InitStruct.Pin = LED_RED_Pin|LED_GREEN_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pin : MIIM_MDIO_Pin */
	  GPIO_InitStruct.Pin = MIIM_MDIO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(MIIM_MDIO_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pin : MIIM_MDC_Pin */
	  GPIO_InitStruct.Pin = MIIM_MDC_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(MIIM_MDC_GPIO_Port, &GPIO_InitStruct);

}


void _MIIM_DRIVER_CLOCK_PULSE() {
	HAL_GPIO_WritePin(GPIOA, MIIM_MDC_Pin, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOA, MIIM_MDC_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
}

void _MIIM_DRIVER_START() {
	// Preamble
	//HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);

	for (uint8_t i=0; i<5; ++i) {
		_MIIM_DRIVER_CLOCK_PULSE();
	}

	// Start clock
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
	_MIIM_DRIVER_CLOCK_PULSE();
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
	_MIIM_DRIVER_CLOCK_PULSE();
}

void _MIIM_DRIVER_OP_CODE_READ() {
	// Data = 0b10
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
	_MIIM_DRIVER_CLOCK_PULSE();
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
	_MIIM_DRIVER_CLOCK_PULSE();
}

void _MIIM_DRIVER_OP_CODE_WRITE() {
	// Data = 0b01
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
	_MIIM_DRIVER_CLOCK_PULSE();
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
	_MIIM_DRIVER_CLOCK_PULSE();
}

void _MIIM_DRIVER_PHY_REG_OUTPUT(uint8_t PHY, uint8_t REG) {
	// Send the PHY address first
	for (uint8_t bitnum = 0; bitnum <= 4; ++bitnum) {
		if ((PHY & (1 << (4-bitnum))) == 0) {
			HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
		} else {
			HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
		}
		_MIIM_DRIVER_CLOCK_PULSE();
	}

	// Send the REG address first
	for (uint8_t bitnum = 0; bitnum <= 4; ++bitnum) {
		if ((REG & (1 << (4-bitnum))) == 0) {
			HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
		} else {
			HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
		}
		_MIIM_DRIVER_CLOCK_PULSE();
	}
}

void _MIIM_DRIVER_TA_WRITE() {
	// The Turnaround bits for write are the same as the op code
	_MIIM_DRIVER_OP_CODE_READ();
}

void _MIIM_DRIVER_TA_READ() {
	// The TA bits for read
	GPIO_SET_MDIO_MODE_INPUT();
	_MIIM_DRIVER_CLOCK_PULSE();
	MX_GPIO_Init();
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
	_MIIM_DRIVER_CLOCK_PULSE();
}

void _MIIM_DRIVER_WRITE_DATA(uint16_t data) {
	for (uint8_t bitnum = 0; bitnum <= 15; ++bitnum) {
		if ((data & (1<<(15-bitnum))) == 0) {
			HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_RESET);
		} else {
			HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
		}
		_MIIM_DRIVER_CLOCK_PULSE();
	}
	// reset clock and data
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, MIIM_MDC_Pin, GPIO_PIN_RESET);
	// final clock pulse afterwards?
}

uint16_t _MIIM_DRIVER_READ_DATA() {
	GPIO_SET_MDIO_MODE_INPUT();
	uint16_t data = 0;
	for (uint8_t bitnum = 0; bitnum <= 15; ++bitnum) {
		data = data + (HAL_GPIO_ReadPin(GPIOA, MIIM_MDIO_Pin) << (15-bitnum));
		_MIIM_DRIVER_CLOCK_PULSE();
	}

	MX_GPIO_Init();
	// Reset clock and data
	HAL_GPIO_WritePin(GPIOA, MIIM_MDIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, MIIM_MDC_Pin, GPIO_PIN_RESET);
	return data;
}



// Define MIIM bit banging functions here
void MIIM_DRIVER_WRITE(uint8_t PHY, uint8_t REG, uint16_t DATA) {
	/*
    Outputs the PHY and REG addresses to the ethernet chip. Writes 2 byte data packet
    to the ethernet chip for that PHY/REG address.
    Parameters:
        PHY (uint8_t):The data to be sent via the MIIM_MDIO pin
        			  values range from 0b00000000-0b00011111
        			  address corresponds to phy address
        REG (uint8_t):The data to be sent via the MIIM_MDIO pin
        			  values range from 0b00000000-0b00011111
        			  address corresponds to reg address
       	DATA (uint16_t): The data to be send to the PHY register
    Returns:
        void
    */
	// Write the preamble + start code
	_MIIM_DRIVER_START();

	// op code
	_MIIM_DRIVER_OP_CODE_WRITE();

	// write PHY and REG
	_MIIM_DRIVER_PHY_REG_OUTPUT(PHY, REG);

	// TA bits
	_MIIM_DRIVER_TA_WRITE();

	// write data
	_MIIM_DRIVER_WRITE_DATA(DATA);

}

uint16_t MIIM_DRIVER_READ(uint8_t PHY, uint8_t REG) {
	/*
	Reads 2 byte data packet from PHY and REG on ethernet chip.
	Parameters:
	    PHY (uint8_t):The data to be sent via the MIIM_MDIO pin
	        			  values range from 0b00000000-0b00011111
	        			  address corresponds to phy address
	    REG (uint8_t):The data to be sent via the MIIM_MDIO pin
	        			  values range from 0b00000000-0b00011111
	        			  address corresponds to reg address
	 Returns:
	    void
	*/
	// Preamble + start code
	_MIIM_DRIVER_START();

	// op code
	_MIIM_DRIVER_OP_CODE_READ();

	// write PHY and REG
	//uint8_t PHY = 0b11111;
	//uint8_t REG = 0b11010;
	_MIIM_DRIVER_PHY_REG_OUTPUT(PHY, REG);

	// TA bits
	_MIIM_DRIVER_TA_READ();

	// read data
	uint16_t data = _MIIM_DRIVER_READ_DATA();

	return data;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Port mirroring settings
  // Please only configure headings marked with '***'
  // I strongly encourage that you have the data sheet for the IP175Gx to hand
  // If you spot a mistake or potential improvement (we already have lots of improvements in mind) - please let us know
  // We have currently configured the default settings to mirror RX traffic on port 3 (labelled port 4 on SwitchBlox) to port 2 (labelled port 3 on SwitchBlox)

  // (0) Declare and define PHY and REG variables,
  uint8_t PHY = 20;
  uint8_t REG3 = 3;
  uint8_t REG4 = 4;


  //***(1) Enable port mirroring by writing to 20.3[15] - PHY=20, REG = 3, bitnum = 15
  uint8_t PORT_MIRROR_EN = 0b1;

  //***(2) Select the port that will be the mirror or 'sniffing' port
  // The SwitchBlox currently has labelled Port X+1 (i.e. Port 0 -> Labelled Port 1 on SwitchBlox)
  // ^This may change on future revisions
  // Only select one port to be the mirror port
  // In order of 'SEL_MIRROR_PORT' declarations: (default: port 4)
  //(1) Port 0
  //(2) Port 1
  //(3) Port 2
  //(4) Port 3
  //(5) Port 4 (default)

  //uint8_t SEL_MIRROR_PORT = 0b010;
  //uint8_t SEL_MIRROR_PORT = 0b011;
  uint8_t SEL_MIRROR_PORT = 0b100;
  //uint8_t SEL_MIRROR_PORT = 0b110;
  //uint8_t SEL_MIRROR_PORT = 0b111;

  //***(3) Select the port mirroring mode
  // Set the conditions that traffic on the mirrored ports must fulfill for it to be mirrored to the sniffer port
  // In order of 'PORT_MIRROR_MODE' declarations: (default: mirror RX)
  //(1) mirror RX (default)
  //(2) mirror TX
  //(3) mirror RX and TX
  //(4) mirror RX or TX

  uint8_t PORT_MIRROR_MODE = 0b00;
  //uint8_t PORT_MIRROR_MODE = 0b01;
  //uint8_t PORT_MIRROR_MODE = 0b10;
  //uint8_t PORT_MIRROR_MODE = 0b11;

  //***(4a) Select the RX source port that will have its RX data flow mirrored to the sniffer port
  // In order of 'SEL_RX_PORT_MIRROR' declarations: (default: None)
  //(1) port 0
  //(2) port 1
  //(3) port 2
  //(4) port 3
  //(5) port 4

  //uint8_t SEL_RX_PORT_MIRROR = 0b00000100;
  //uint8_t SEL_RX_PORT_MIRROR = 0b00001000;
  //uint8_t SEL_RX_PORT_MIRROR = 0b00010000;
  uint8_t SEL_RX_PORT_MIRROR = 0b01000000;
  //uint8_t SEL_RX_PORT_MIRROR = 0b10000000;

  //***(4b) Select the TX source port that will have its TX data flow mirrored to the sniffer port
  // Must configure 'PORT_MIRROR_MODE' to include mirroring TX
  // In order of 'SEL_TX_PORT_MIRROR' declarations: (default: None)
  //(1) port 0
  //(2) port 1
  //(3) port 2
  //(4) port 3
  //(5) port 4

  //uint8_t SEL_TX_PORT_MIRROR = 0b00000100;
  //uint8_t SEL_TX_PORT_MIRROR = 0b00001000;
  //uint8_t SEL_TX_PORT_MIRROR = 0b00010000;
  //uint8_t SEL_TX_PORT_MIRROR = 0b01000000;
  uint8_t SEL_TX_PORT_MIRROR = 0b10000000;

  // (5) Create the data packet to send to 20.3/20.4 PHY.REG
  uint16_t PORT_MIRROR_CONFIG_REG3 = (((uint16_t) PORT_MIRROR_EN) << 15) | (((uint16_t) PORT_MIRROR_MODE) << 13) | (SEL_RX_PORT_MIRROR);
  uint16_t PORT_MIRROR_CONFIG_REG4 = (((uint16_t) SEL_MIRROR_PORT << 13) | (SEL_TX_PORT_MIRROR));

  // (6) Execute the command to write the configurations for port mirroring
  // For some reason, writing only once doesn't work all the time (if you find a solution, please publish an issue to https://github.com/AaronElijah/IP175GPortMirroringConfig
  for (uint8_t j = 0; j <= 10; j++) {
   HAL_Delay(5);
   MIIM_DRIVER_WRITE(PHY, REG3, PORT_MIRROR_CONFIG_REG3);
   HAL_Delay(5);
   MIIM_DRIVER_WRITE(PHY, REG4, PORT_MIRROR_CONFIG_REG4);
  }

  GPIO_SET_MDIO_MDC_MODE_INPUT();

  // Set the colour of the LED purple
  HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, LED_RED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RED_Pin|MIIM_MDC_Pin|LED_GREEN_Pin|MIIM_MDIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_BLUE_Pin */
  GPIO_InitStruct.Pin = LED_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BLUE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RED_Pin MIIM_MDC_Pin LED_GREEN_Pin MIIM_MDIO_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|MIIM_MDC_Pin|LED_GREEN_Pin|MIIM_MDIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
