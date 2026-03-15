/*
 * Author: Florian Simon
 * Institution: Technische Universitaet Berlin
 */

#ifndef INC_RNBD451_H_
#define INC_RNBD451_H_

typedef struct
{
	UART_HandleTypeDef* huart;
	GPIO_TypeDef* rnbd_uart_mode_port;
	uint16_t rnbd_uart_mode_pin;
	GPIO_TypeDef* rnbd_nmclr_port;
	uint16_t rnbd_nmclr_pin;
	GPIO_TypeDef* rnbd_ble_status1_port;
	uint16_t rnbd_ble_status1_pin;
	GPIO_TypeDef* rnbd_ble_status2_port;
	uint16_t rnbd_ble_status2_pin;
	GPIO_TypeDef* rnbd_tx_indication_port;
	uint16_t rnbd_tx_indication_pin;
	GPIO_TypeDef* rnbd_rx_indication_port;
	uint16_t rnbd_rx_indication_pin;
	GPIO_TypeDef* rnbd_rssi_indication_port;
	uint16_t rnbd_rssi_indication_pin;

	char tx_buf[128];
	char rx_buf[128];
} rnbd451_t;

void rnbd451_init(rnbd451_t* rnbd451, UART_HandleTypeDef* huart, GPIO_TypeDef* rnbd_uart_mode_port, uint16_t rnbd_uart_mode_pin, GPIO_TypeDef* rnbd_nmclr_port, uint16_t rnbd_nmclr_pin, GPIO_TypeDef* rnbd_ble_status1_port, uint16_t rnbd_ble_status1_pin, GPIO_TypeDef* rnbd_ble_status2_port, uint16_t rnbd_ble_status2_pin, GPIO_TypeDef* rnbd_tx_indication_port, uint16_t rnbd_tx_indication_pin, GPIO_TypeDef* rnbd_rx_indication_port, uint16_t rnbd_rx_indication_pin, GPIO_TypeDef* rnbd_rssi_indication_port, uint16_t rnbd_rssi_indication_pin);

#endif /* INC_RNBD451_H_ */
