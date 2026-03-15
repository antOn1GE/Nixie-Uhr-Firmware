/*
 * Author: Steven Kraetzschmar
 * Institution: Technische Universitaet Berlin
 */

#include "main.h"
#include <string.h>
#include "tmp126.h"

// extern SPI_HandleTypeDef hspi2;

void TMP126_init(TMP126_t* TMP126, SPI_HandleTypeDef* SPI_Handler, GPIO_TypeDef* GPIO, uint16_t CS)
{
	TMP126->SPI = SPI_Handler;
	TMP126->GPIO = GPIO;
	TMP126->CSPin = CS;

	memset(TMP126->tx_cmd_buffer,0,2);
	memset(TMP126->tx_buffer,0,2);
	memset(TMP126->rx_buffer,0,2);

	TMP126->TempResultAdr = 0x00;
	TMP126->SlewResultAdr = 0x01;
	TMP126->AlertStatusAdr = 0x02;
	TMP126->ConfigAdr = 0x03;
	TMP126->AlertEnableAdr = 0x04;
	TMP126->TLowLimitAdr = 0x05;
	TMP126->THighLimitAdr = 0x06;
	TMP126->HysterisAdr = 0x07;
	TMP126->SlewLimitAdr = 0x08;
	TMP126->UID1Adr = 0x09;
	TMP126->UID2Adr = 0x0A;
	TMP126->UID3Adr = 0x0B;
	TMP126->DIDAdr = 0x0C;

	TMP126->DefaultCfgHigh = 0x00;
	TMP126->DefaultCfgLow = 0x96;
	TMP126->TempCoeff = 0.03125;
	TMP126->Mode = 0;

	TMP126->ReceivedData = 0;
	TMP126->Temperature = 0.0;

	TMP126->ConfigFlag = 0;
	TMP126->ReadTempFlag = 0;
}

void TMP126_Configuration(TMP126_t* TMP126)
{
	TMP126->Mode = 0x00;
	TMP126->ConfigFlag = 1;

	TMP126->tx_cmd_buffer[1] = TMP126->Mode;
	TMP126->tx_cmd_buffer[0] = TMP126->ConfigAdr;

	TMP126->tx_buffer[1] = TMP126->DefaultCfgHigh;
	TMP126->tx_buffer[0] = TMP126->DefaultCfgLow;

	HAL_GPIO_WritePin(TMP126->GPIO, TMP126->CSPin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(TMP126->SPI, TMP126->tx_cmd_buffer, 1);
}

void TMP126_ReadTemperature(TMP126_t* TMP126)
{
	if (TMP126->ReadTempFlag == 1) return;
	TMP126->Mode = 0x01;
	TMP126->ReadTempFlag = 1;

	TMP126->tx_cmd_buffer[1] = TMP126->Mode;
	TMP126->tx_cmd_buffer[0] = TMP126->TempResultAdr;

	HAL_GPIO_WritePin(TMP126->GPIO, TMP126->CSPin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(TMP126->SPI,TMP126->tx_cmd_buffer, 1);
}

void TMP126_SPI_Transmit_callback(TMP126_t* TMP126)
{
	if (TMP126->ConfigFlag == 1)
	{
		TMP126->ConfigFlag = 2;
		HAL_SPI_Transmit_DMA(TMP126->SPI, TMP126->tx_buffer, 1);
	}
	if (TMP126->ConfigFlag == 2)
	{
		TMP126->ConfigFlag = 0;
		HAL_GPIO_WritePin(TMP126->GPIO, TMP126->CSPin, GPIO_PIN_SET);
	}
	if (TMP126->ReadTempFlag == 1)
	{
		HAL_SPI_Receive_DMA(TMP126->SPI, TMP126->rx_buffer,1);
	}
}

void TMP126_SPI_Receive_callback(TMP126_t* TMP126)
{
	HAL_GPIO_WritePin(TMP126->GPIO, TMP126->CSPin, GPIO_PIN_SET);
	TMP126->ReceivedData = ((uint16_t) TMP126->rx_buffer[1] << 8) | TMP126->rx_buffer[0];
	TMP126->ReceivedData = TMP126->ReceivedData >> 2;
	TMP126->Temperature = (int16_t) TMP126->ReceivedData * TMP126->TempCoeff;
	TMP126->ReadTempFlag = 0;
}
