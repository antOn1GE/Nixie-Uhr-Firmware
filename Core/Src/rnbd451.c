/*
 * Author: Florian Simon
 * Institution: Technische Universitaet Berlin
 */

#include "main.h"
#include <string.h>
#include "rnbd451.h"

void rnbd451_init(rnbd451_t* rnbd451, UART_HandleTypeDef* huart, GPIO_TypeDef* rnbd_uart_mode_port, uint16_t rnbd_uart_mode_pin, GPIO_TypeDef* rnbd_nmclr_port, uint16_t rnbd_nmclr_pin, GPIO_TypeDef* rnbd_ble_status1_port, uint16_t rnbd_ble_status1_pin, GPIO_TypeDef* rnbd_ble_status2_port, uint16_t rnbd_ble_status2_pin, GPIO_TypeDef* rnbd_tx_indication_port, uint16_t rnbd_tx_indication_pin, GPIO_TypeDef* rnbd_rx_indication_port, uint16_t rnbd_rx_indication_pin, GPIO_TypeDef* rnbd_rssi_indication_port, uint16_t rnbd_rssi_indication_pin)
{
	rnbd451->huart = huart;
	rnbd451->rnbd_uart_mode_port = rnbd_uart_mode_port;
	rnbd451->rnbd_uart_mode_pin = rnbd_uart_mode_pin;
	rnbd451->rnbd_nmclr_port = rnbd_nmclr_port;
	rnbd451->rnbd_nmclr_pin = rnbd_nmclr_pin;
	rnbd451->rnbd_ble_status1_port = rnbd_ble_status1_port;
	rnbd451->rnbd_ble_status1_pin = rnbd_ble_status1_pin;
	rnbd451->rnbd_ble_status2_port = rnbd_ble_status2_port;
	rnbd451->rnbd_ble_status2_pin = rnbd_ble_status2_pin;
	rnbd451->rnbd_tx_indication_port = rnbd_tx_indication_port;
	rnbd451->rnbd_tx_indication_pin = rnbd_tx_indication_pin;
	rnbd451->rnbd_rx_indication_port = rnbd_rx_indication_port;
	rnbd451->rnbd_rx_indication_pin = rnbd_rx_indication_pin;
	rnbd451->rnbd_rssi_indication_port = rnbd_rssi_indication_port;
	rnbd451->rnbd_rssi_indication_pin = rnbd_rssi_indication_pin;

	memset(rnbd451->tx_buf, 0, 128);
	memset(rnbd451->rx_buf, 0, 128);

	strcpy(rnbd451->tx_buf, "$$$");
	HAL_UART_Transmit(rnbd451->huart, (uint8_t*) rnbd451->tx_buf, 3, HAL_MAX_DELAY);
	HAL_UART_Receive(rnbd451->huart, (uint8_t*) rnbd451->rx_buf, 128, 500);
	HAL_Delay(100);
	if ((rnbd451->rx_buf[0] != 'C') || (rnbd451->rx_buf[1] != 'M') || (rnbd451->rx_buf[2] != 'D') || (rnbd451->rx_buf[3] != '>'))
	{
		strcpy(rnbd451->tx_buf, "---\r");
		HAL_UART_Transmit(rnbd451->huart, (uint8_t*) rnbd451->tx_buf, 4, HAL_MAX_DELAY);
		HAL_UART_Receive(rnbd451->huart, (uint8_t*) rnbd451->rx_buf, 128, 500);
		HAL_Delay(100);
		if ((rnbd451->rx_buf[0] != 'E') || (rnbd451->rx_buf[1] != 'N') || (rnbd451->rx_buf[2] != 'D'))
		{
			strcpy(rnbd451->tx_buf, "---\r");
			HAL_UART_Transmit(rnbd451->huart, (uint8_t*) rnbd451->tx_buf, 4, HAL_MAX_DELAY);
			HAL_UART_Receive(rnbd451->huart, (uint8_t*) rnbd451->rx_buf, 128, 500);
			HAL_Delay(100);
		}
		strcpy(rnbd451->tx_buf, "$$$");
		HAL_UART_Transmit(rnbd451->huart, (uint8_t*) rnbd451->tx_buf, 3, HAL_MAX_DELAY);
		HAL_UART_Receive(rnbd451->huart, (uint8_t*) rnbd451->rx_buf, 128, 500);
		HAL_Delay(100);
	}
	strcpy(rnbd451->tx_buf, "R,1\r");
	HAL_UART_Transmit(rnbd451->huart, (uint8_t*) rnbd451->tx_buf, 4, HAL_MAX_DELAY);
	HAL_UART_Receive(rnbd451->huart, (uint8_t*) rnbd451->rx_buf, 128, 2000);
	HAL_Delay(100);
}
