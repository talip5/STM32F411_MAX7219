/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led_driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// LOAD(CS) pinini kontrol etmek için makro veya fonksiyon

#define MAX7219_CS_LOW() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)

#define MAX7219_CS_HIGH() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

// 8x8 'Gülen Yüz' Bitmap Verisi
const uint8_t SMILEY[8] = {
    0b00111100, // Satır 0:   ****
    0b01000010, // Satır 1:  *    *
    0b10100101, // Satır 2: * *  * * (Gözler)
    0b10000001, // Satır 3: *      *
    0b10100101, // Satır 4: * *  * * (Ağız)
    0b10011001, // Satır 5: *  **  *
    0b01000010, // Satır 6:  *    *
    0b00111100  // Satır 7:   ****
};

// 8x8 matrisin anlık durumunu hafızada tutan ekran tamponu (framebuffer)
uint8_t matrix_buffer[8] = { 0 };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */

void MAX7219_Write(uint8_t reg, uint8_t data);
void MAX7219_Init(void);
void MAX7219_DrawBitmap(const uint8_t bitmap[8]);
void MAX7219_Clear(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  led_Init();
  MAX7219_Init();
  /* USER CODE BEGIN 2 */

  GPIOD->BSRR |=GPIO_BSRR_BS15;

  // Gülen yüzü ekrana bas
      //MAX7219_DrawBitmap(SMILEY);

      // Veya manuel olarak 2. satır, 3. sütundaki pikseli yak
      // MAX7219_SetPixel(3, 2, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	 // MAX7219_Write(0x01, 0x01);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

// 8x8 matrisin anlık durumunu hafızada tutan ekran tamponu (framebuffer)


//max7219'a 16 bit komut/veri gönderme fonksiyonu

void MAX7219_Write(uint8_t reg, uint8_t data) {

	MAX7219_CS_LOW(); // Veri aktarımı için CS Low yap

	uint8_t packet[2]={reg,data};

	//	packet[0] = reg;   // 1.Bayt: Register Adresi
	//	packet[1] = data;  // 2.Bayt: Veri/Değer

	HAL_SPI_Transmit(&hspi1, packet, 2, HAL_MAX_DELAY); // 16 bit veriyi gönder (2 bayt)

	MAX7219_CS_HIGH(); // Veriyi latch'lemek (hafızaya almak) için LOAD'u HIGH yap

}

//MAX7219 Başlatma (Initialization) Fonksiyonu

void MAX7219_Init(void) {

	MAX7219_Write(0x0C, 0x01);  // Shutdown register: Normal çalışma modu
	MAX7219_Write(0x09, 0x00);  // Decode Mode: NO DECODE (Matris için ZORUNLU!)
	MAX7219_Write(0x0A, 0x00); //Intensity: düşük/Orta seviye parlaklık (0x00 - 0x0F arası)
	MAX7219_Write(0x0B, 0x07);  // Scan Limit: 8 satırın tümünü tara (0-7)
	MAX7219_Write(0x0F, 0x00);  // Display Test: Test modunu kapat(Normal)

	MAX7219_Write(0x01, 0x00);  // Ekranın 1. satırını temizleme
	MAX7219_Write(0x02, 0x00);  // Ekranın 2. satırını temizleme
	MAX7219_Write(0x03, 0x00);  // Ekranın 3. satırını temizleme
	MAX7219_Write(0x04, 0x00);  // Ekranın 4. satırını temizleme
	MAX7219_Write(0x05, 0x00);  // Ekranın 5. satırını temizleme
	MAX7219_Write(0x06, 0x00);  // Ekranın 6. satırını temizleme
	MAX7219_Write(0x07, 0x00);  // Ekranın 7. satırını temizleme
	MAX7219_Write(0x08, 0x00);  // Ekranın 8. satırını temizleme
	HAL_Delay(10);
	//MAX7219_Write(0x01, 0x01);
	//MAX7219_Write(0x01, 0x80);
	MAX7219_Write(0x01, 0xFF);
	MAX7219_Write(0x02, 0x81);
	MAX7219_Write(0x03, 0x81);
	MAX7219_Write(0x04, 0x81);
	MAX7219_Write(0x05, 0x81);
	MAX7219_Write(0x06, 0x81);
	MAX7219_Write(0x07, 0x81);
	MAX7219_Write(0x08, 0x0F);
	//MAX7219_Write(0x08, 0x02);
	//MAX7219_Write(0x08, 0x04);


	// Ekranı başlangıçta temizlemek için tüm hanelere 0 (boşluk veya 0 ) basabilirsiniz

	// Ekranı Başlangıçta Temizle
	//MAX7219_Clear();

}

// Tüm ekranı temizleme
void MAX7219_Clear(void) {
	for (uint8_t row = 0; row < 8; row++) {
		matrix_buffer[row] = 0x00;
		MAX7219_Write(row + 1, 0x00); // Satır adresleri 0x01 ile 0x08 arasındadır
	}
}

// Belirli bir koordinattaki (x: 0-7, y: 0-7) pikseli yakma veya söndürme
// state: 1 = YAK, 0 = SÖNDÜR
void MAX7219_SetPixel(uint8_t x, uint8_t y, uint8_t state) {
    if (x > 7 || y > 7) return;

    if (state) {
        // x. sütundaki biti 1 yap (OR mantığı)
        matrix_buffer[y] |= (1 << (7 - x));
    } else {
        // x. sütundaki biti 0 yap (AND NOT mantığı)
        matrix_buffer[y] &= ~(1 << (7 - x));
    }

    // Yalnızca değişen satırı MAX7219'a güncelle (Satır adresi = y + 1)
    MAX7219_Write(y + 1, matrix_buffer[y]);
}

// Hazır bir karakter veya resmi (8 baytlık dizi) ekrana çizdirme
void MAX7219_DrawBitmap(const uint8_t bitmap[8]) {
    for (uint8_t i = 0; i < 8; i++) {
        matrix_buffer[i] = bitmap[i];
        MAX7219_Write(i + 1, bitmap[i]);
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
