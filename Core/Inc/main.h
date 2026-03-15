/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STM_PC13_Pin GPIO_PIN_13
#define STM_PC13_GPIO_Port GPIOC
#define STM_PC14_Pin GPIO_PIN_14
#define STM_PC14_GPIO_Port GPIOC
#define STM_LD2_Pin GPIO_PIN_15
#define STM_LD2_GPIO_Port GPIOC
#define STM_HSE_IN_Pin GPIO_PIN_0
#define STM_HSE_IN_GPIO_Port GPIOH
#define STM_HSE_OUT_Pin GPIO_PIN_1
#define STM_HSE_OUT_GPIO_Port GPIOH
#define SPI_DEC_TMP_NCS2_Pin GPIO_PIN_1
#define SPI_DEC_TMP_NCS2_GPIO_Port GPIOA
#define SPI_DEC_TMP_NDCS_Pin GPIO_PIN_2
#define SPI_DEC_TMP_NDCS_GPIO_Port GPIOA
#define SPI_DEC_TMP_NCS1_Pin GPIO_PIN_3
#define SPI_DEC_TMP_NCS1_GPIO_Port GPIOA
#define SPI_DEC_TMP_SCK_Pin GPIO_PIN_5
#define SPI_DEC_TMP_SCK_GPIO_Port GPIOA
#define SPI_DEC_TMP_MISO_Pin GPIO_PIN_6
#define SPI_DEC_TMP_MISO_GPIO_Port GPIOA
#define SPI_DEC_TMP_MOSI_Pin GPIO_PIN_7
#define SPI_DEC_TMP_MOSI_GPIO_Port GPIOA
#define UART_RNBD_RX_Pin GPIO_PIN_4
#define UART_RNBD_RX_GPIO_Port GPIOC
#define UART_RNBD_TX_Pin GPIO_PIN_5
#define UART_RNBD_TX_GPIO_Port GPIOC
#define STM_DEBUG1_Pin GPIO_PIN_2
#define STM_DEBUG1_GPIO_Port GPIOB
#define RNBD_RX_INDICATION_Pin GPIO_PIN_15
#define RNBD_RX_INDICATION_GPIO_Port GPIOB
#define RNBD_RSSI_INDICATION_Pin GPIO_PIN_6
#define RNBD_RSSI_INDICATION_GPIO_Port GPIOC
#define RNBD_TX_INDICATION_Pin GPIO_PIN_7
#define RNBD_TX_INDICATION_GPIO_Port GPIOC
#define RNBD_UART_MODE_Pin GPIO_PIN_8
#define RNBD_UART_MODE_GPIO_Port GPIOC
#define RNBD_BLE_STATUS2_Pin GPIO_PIN_9
#define RNBD_BLE_STATUS2_GPIO_Port GPIOC
#define RNBD_BLE_STATUS1_Pin GPIO_PIN_8
#define RNBD_BLE_STATUS1_GPIO_Port GPIOA
#define RNBD_NMCLR_Pin GPIO_PIN_10
#define RNBD_NMCLR_GPIO_Port GPIOA
#define STM_SWDIO_Pin GPIO_PIN_13
#define STM_SWDIO_GPIO_Port GPIOA
#define STM_SWCLK_Pin GPIO_PIN_14
#define STM_SWCLK_GPIO_Port GPIOA
#define SPI_LED_NCS_Pin GPIO_PIN_15
#define SPI_LED_NCS_GPIO_Port GPIOA
#define SPI_LED_SCK_Pin GPIO_PIN_10
#define SPI_LED_SCK_GPIO_Port GPIOC
#define STM_PC11_Pin GPIO_PIN_11
#define STM_PC11_GPIO_Port GPIOC
#define SPI_LED_MOSI_Pin GPIO_PIN_12
#define SPI_LED_MOSI_GPIO_Port GPIOC
#define STM_DEBUG3_Pin GPIO_PIN_2
#define STM_DEBUG3_GPIO_Port GPIOD
#define STM_SWO_Pin GPIO_PIN_3
#define STM_SWO_GPIO_Port GPIOB
#define STM_LD1_Pin GPIO_PIN_4
#define STM_LD1_GPIO_Port GPIOB
#define DCF_NEN_Pin GPIO_PIN_5
#define DCF_NEN_GPIO_Port GPIOB
#define DCF_OUT_Pin GPIO_PIN_6
#define DCF_OUT_GPIO_Port GPIOB
#define DCF_OUT_EXTI_IRQn EXTI9_5_IRQn
#define I2C_RTC_SDA_Pin GPIO_PIN_7
#define I2C_RTC_SDA_GPIO_Port GPIOB
#define I2C_RTC_SCL_Pin GPIO_PIN_8
#define I2C_RTC_SCL_GPIO_Port GPIOB
#define STM_DEBUG2_Pin GPIO_PIN_9
#define STM_DEBUG2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
