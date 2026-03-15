/*
 * Author: Steven Kraetzschmar
 * Institution: Technische Universitaet Berlin
 */

#ifndef TMP126_H_
#define TMP126_H_

// #define TMPSPI_CSGP GPIOB
// #define TMPSPI_CSPin GPIO_PIN_12

typedef struct
{
	GPIO_TypeDef* 		GPIO;
	uint16_t	 		CSPin;
	SPI_HandleTypeDef* 	SPI;

	uint8_t				tx_cmd_buffer[2];
	uint8_t				rx_buffer[2];
	uint8_t 			tx_buffer[2];

	uint8_t 			TempResultAdr;
	uint8_t 			SlewResultAdr;
	uint8_t 			AlertStatusAdr;
	uint8_t 			ConfigAdr;
	uint8_t 			AlertEnableAdr;
	uint8_t 			TLowLimitAdr;
	uint8_t 			THighLimitAdr;
	uint8_t 			HysterisAdr;
	uint8_t 			SlewLimitAdr;
	uint8_t 			UID1Adr;
	uint8_t 			UID2Adr;
	uint8_t 			UID3Adr;
	uint8_t 			DIDAdr;

	uint8_t 			DefaultCfgHigh;
	uint8_t 			DefaultCfgLow;
	double				TempCoeff;
	uint8_t				Mode;	// Read = 0, Write = 1

	uint16_t			ReceivedData;
	double				Temperature;

	uint8_t				ConfigFlag;
	uint8_t				ReadTempFlag;

} TMP126_t;

void TMP126_init(TMP126_t* TMP126, SPI_HandleTypeDef* SPI_Handler, GPIO_TypeDef* GPIO, uint16_t CS);
void TMP126_Configuration(TMP126_t* TMP126);
void TMP126_ReadTemperature(TMP126_t* TMP126);
void TMP126_SPI_Transmit_callback(TMP126_t* TMP126);
void TMP126_SPI_Receive_callback(TMP126_t* TMP126);

#endif /* TMP126_H_ */
